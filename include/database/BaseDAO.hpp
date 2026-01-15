#pragma once

#include <functional>  // For std::function
#include <list>
#include <mutex>
#include <optional>
#include <string>

#include "database/Handler.hpp"
#include "database/Interface.hpp"
#include "logger/Logger.hpp"
#include "types/Result.hpp"

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
  auto DeleteAll() -> Result<void> override {
    std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
    std::string sql = "DELETE FROM " + table_name_ + ";";
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db_handler_.get(), sql.c_str(), 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
      std::string err = "Failed to execute statement: " + std::string(err_msg);
      Database::Error() << err << std::endl;
      sqlite3_free(err_msg);
      return std::unexpected(err);
    }
    return {};
  }

  // Virtual methods to be implemented by derived classes for DTO-specific logic
  virtual auto CreateTable() -> Result<void> = 0;
  virtual auto Create(const T& obj) -> Result<void> override = 0;
  virtual auto Read(const PrimaryKeyType& key) -> Result<std::optional<T>> override = 0;
  virtual auto ReadAll() -> Result<std::list<T>> override = 0;
  virtual auto Update(const T& obj) -> Result<void> override = 0;
  virtual auto Delete(const PrimaryKeyType& key) -> Result<void> override = 0;

 protected:
  // Helper for preparing and executing statements (e.g., for Create, Update, Delete)
  auto execute_statement(const std::string& sql, std::function<void(sqlite3_stmt*)> binder) -> Result<void> {
    std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
      std::string err = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db_handler_.get()));
      Database::Error() << err << std::endl;
      return std::unexpected(err);
    }
    binder(stmt);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
      std::string err = "Failed to execute statement: " + std::string(sqlite3_errmsg(db_handler_.get()));
      Database::Error() << err << std::endl;
      return std::unexpected(err);
    }
    return {};
  }

  // Helper for reading a single row
  auto query_single(const std::string& sql, std::function<void(sqlite3_stmt*)> binder,
                    std::function<T(sqlite3_stmt*)> extractor) -> Result<std::optional<T>> {
    std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
      std::string err = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db_handler_.get()));
      Database::Error() << err << std::endl;
      return std::unexpected(err);
    }

    // Helper for reading multiple rows
    auto query_all(const std::string& sql, std::function<T(sqlite3_stmt*)> extractor) -> Result<std::list<T>> {
      std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
      std::list<T> results;
      sqlite3_stmt* stmt = nullptr;
      int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
      if (rc != SQLITE_OK) {
        std::string err = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db_handler_.get()));
        Database::Error() << err << std::endl;
        return std::unexpected(err);
      }
      while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        results.emplace_back(extractor(stmt));
      }
      if (rc != SQLITE_DONE) {
        std::string err = "Failed to execute query: " + std::string(sqlite3_errmsg(db_handler_.get()));
        Database::Error() << err << std::endl;
        sqlite3_finalize(stmt);
        return std::unexpected(err);
      }
      sqlite3_finalize(stmt);
      return results;
    }
  }

};  // namespace sb::database