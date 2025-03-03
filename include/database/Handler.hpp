#include <list>
#include <mutex>
#include <string>
#include <utility>

#include "sqlite3.h"

using namespace std::chrono_literals;

namespace sb::database {
using Buffer = std::list<std::pair<std::string, std::string>>;

auto busy_handler(void* ptr, int retry_count) -> int;

class Handler {
 public:
  auto static Instance() -> Handler& {
    static Handler instance;
    return instance;
  }

  ~Handler();

 private:
  Handler();

  auto Mutex() -> std::mutex&;

  sqlite3* db_;
  static Buffer buffer_;
  constexpr static char db_name_[256] = "database.db";
};
}  // namespace sb::database
