#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "logger/Logger.hpp"
#include "sqlite3.h"

namespace sb::database {

using Database = sb::logger::Logger<"Database">;

auto busy_handler(void* ptr, int retry_count) -> int;

class Handler {
  // Custom deleter for the sqlite3 unique_ptr
  struct Sqlite3Deleter {
    void operator()(sqlite3* db) const {
      if (db) {
        Database::Info() << "Closing DB connection." << std::endl;
        sqlite3_close(db);
      }
    }
  };

 public:
  explicit Handler(const std::string& db_path) : db_path_{db_path} {
    sqlite3* db_ptr = nullptr;
    auto rc = sqlite3_open(db_path.c_str(), &db_ptr);
    db_.reset(db_ptr);

    if (rc) {
      Database::Error() << "Error opening " << db_path << ": " << sqlite3_errmsg(db_.get()) << std::endl;
      db_.reset();  // Release pointer on failure
    } else {
      Database::Info() << "Successfully opened DB: " << db_path << std::endl;
      sqlite3_busy_handler(db_.get(), busy_handler, NULL);
      sqlite3_busy_timeout(db_.get(), 100);
    }
  }

  // Deleted copy and move semantics to prevent issues with the DB handle
  Handler(const Handler&) = delete;
  auto operator=(const Handler&) -> Handler& = delete;
  Handler(Handler&&) = delete;
  auto operator=(Handler&&) -> Handler& = delete;

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
