#include "database/Handler.hpp"
#include <expected>

namespace sb::database {

auto Handler::create(const std::string& db_path) -> Result<Handler> {
  sqlite3* db_ptr = nullptr;
  auto rc = sqlite3_open(db_path.c_str(), &db_ptr);
  if (rc) {
    std::string err = "Error opening " + db_path + ": " + sqlite3_errmsg(db_ptr);
    sqlite3_close(db_ptr);
    return std::unexpected(err);
  }
  auto db = std::unique_ptr<sqlite3, Sqlite3Deleter>(db_ptr);
  return Handler(db_path, std::move(db));
}

auto busy_handler(void* ptr, int retry_count) -> int {
  if (retry_count < 5) {
    Database::Warning() << "Database is busy, retrying " << retry_count;
    return 1;
  }
  (void)ptr;
  return 0;
}

}  // namespace sb::database
