#pragma once

#include <cstdint>
#include <list>
#include <mutex>
#include <string>
#include <utility>

#include "logger/Logger.hpp"
#include "sqlite3.h"

#include "types/StringLiteral.hpp"

using namespace std::chrono_literals;

namespace sb::database {

using Database = sb::logger::Logger<"Database">;

using Buffer = std::list<std::pair<std::string, std::string>>;

auto busy_handler(void* ptr, int retry_count) -> int;

template <types::StringLiteral db_name>
class Handler {
 public:
  auto static Instance() -> Handler& {
    static Handler instance;
    ref_counter++;
    return instance;
  }

  auto name() -> const std::string& { return db_name_; }

  ~Handler() {
    if (ref_counter == 0) {
      auto rc = sqlite3_close(db_);
      Database::Info() << "Close Db: " << rc << std::endl;
    } else {
      ref_counter--;
    }
  }

 private:
  Handler() {
    auto rc = sqlite3_open(db_name.data, &db_);
    if (rc) {
      Database::Error() << "Error opening " << db_name.data << ": " << sqlite3_errmsg(db_) << std::endl;
    }

    sqlite3_busy_handler(db_, busy_handler, NULL);
    sqlite3_busy_timeout(db_, 100);
  }

  auto Mutex() -> std::mutex& {
    static std::mutex mtx;
    return mtx;
  }

  sqlite3* db_;
  static Buffer buffer_;

  static uint32_t ref_counter;
  constexpr static std::string db_name_{db_name.data};
};

template <types::StringLiteral db_name>
uint32_t Handler<db_name>::ref_counter = 0;

}  // namespace sb::database
