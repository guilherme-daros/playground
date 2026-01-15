#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "logger/Logger.hpp"
#include "sqlite3.h"
#include "types/Result.hpp"

namespace sb::database {

using Database = sb::logger::Logger<"Database">;

auto busy_handler(void* ptr, int retry_count) -> int;

class Handler {
  // Custom deleter for the sqlite3 unique_ptr
  struct Sqlite3Deleter {
    void operator()(sqlite3* db) const {
      if (db) {
        Database::Info() << "Closing DB connection.";
        sqlite3_close(db);
      }
    }
  };

 private:
  explicit Handler(const std::string& db_path, std::unique_ptr<sqlite3, Sqlite3Deleter> db)
      : db_path_{db_path}, db_{std::move(db)} {
    Database::Info() << "Successfully opened DB: " << db_path;
    sqlite3_busy_handler(db_.get(), busy_handler, NULL);
    sqlite3_busy_timeout(db_.get(), 100);
  }

 public:
  static auto create(const std::string& db_path) -> Result<Handler>;

  // Deleted copy semantics to prevent issues with the DB handle
  Handler(const Handler&) = delete;
  auto operator=(const Handler&) -> Handler& = delete;

  ~Handler() = default;  // The unique_ptr will handle closing the DB

  auto get() const -> sqlite3* { return db_.get(); }

  auto path() const -> const std::string& { return db_path_; }

  auto get_mutex() -> std::mutex& { return mtx_; }

 private:
  std::string db_path_;
  std::unique_ptr<sqlite3, Sqlite3Deleter> db_;
  std::mutex mtx_;
};

}  // namespace sb::database
