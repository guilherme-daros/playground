#pragma once

#include <list>
#include <optional>
#include <string>
#include <mutex>
#include <functional> // For std::function

#include "database/Handler.hpp"
#include "database/Interface.hpp"
#include "logger/Logger.hpp"

namespace sb::database {

template <typename T, typename PrimaryKeyType = std::string>
class BaseDAO : public Interface<T, PrimaryKeyType> {
protected:
    Handler& db_handler_;
    std::string table_name_;

public:
    explicit BaseDAO(Handler& db_handler, std::string table_name)
        : db_handler_{db_handler}, table_name_{std::move(table_name)} {}

    // Common methods with generic implementation
    auto DeleteAll() -> bool override {
        std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
        std::string sql = "DELETE FROM " + table_name_ + ";";
        char* err_msg = nullptr;
        int rc = sqlite3_exec(db_handler_.get(), sql.c_str(), 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            Database::Error() << "Failed to execute statement: " << err_msg << std::endl;
            sqlite3_free(err_msg);
            return false;
        }
        return true;
    }

    // Virtual methods to be implemented by derived classes for DTO-specific logic
    virtual void CreateTable() = 0;
    virtual auto Create(const T& obj) -> bool override = 0;
    virtual auto Read(const PrimaryKeyType& key) -> std::optional<T> override = 0;
    virtual auto ReadAll() -> std::list<T> override = 0;
    virtual auto Update(const T& obj) -> bool override = 0;
    virtual auto Delete(const PrimaryKeyType& key) -> bool override = 0;

protected:
    // Helper for preparing and executing statements (e.g., for Create, Update, Delete)
    auto execute_statement(const std::string& sql,
                           std::function<void(sqlite3_stmt*)> binder) -> bool {
        std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
            return false;
        }
        binder(stmt);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            Database::Error() << "Failed to execute statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
            return false;
        }
        return true;
    }

    // Helper for reading a single row
    auto query_single(const std::string& sql,
                      std::function<void(sqlite3_stmt*)> binder,
                      std::function<T(sqlite3_stmt*)> extractor) -> std::optional<T> {
        std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
            return std::nullopt;
        }
        binder(stmt);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            T result = extractor(stmt);
            sqlite3_finalize(stmt);
            return result;
        }
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    // Helper for reading multiple rows
    auto query_all(const std::string& sql,
                   std::function<T(sqlite3_stmt*)> extractor) -> std::list<T> {
        std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
        std::list<T> results;
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
            return results;
        }
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            results.emplace_back(extractor(stmt));
        }
        sqlite3_finalize(stmt);
        return results;
    }
};

} // namespace sb::database