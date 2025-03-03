#include "database/Handler.hpp"

#include <cstddef>
#include <cstdio>
#include <mutex>

namespace sb::database {

auto busy_handler(void* ptr, int retry_count) -> int {
  if (retry_count < 5) {
    printf("Database is busy, retrying %d...\n", retry_count);
    return 1;
  }
  (void)ptr;
  return 0;
}

Handler::~Handler() {
  auto rc = sqlite3_close(db_);
  printf("Close Db: %i\n", rc);
}

Handler::Handler() {
  auto rc = sqlite3_open(db_name_, &db_);
  if (rc) {
    printf("Error opening %s: %s\n", db_name_, sqlite3_errmsg(db_));
  }

  sqlite3_busy_handler(db_, busy_handler, NULL);
  sqlite3_busy_timeout(db_, 100);
}

auto Handler::Mutex() -> std::mutex& {
  static std::mutex mtx;
  return mtx;
}

}  // namespace sb::database
