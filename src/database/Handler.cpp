#include "database/Handler.hpp"

namespace sb::database {

auto busy_handler(void* ptr, int retry_count) -> int {
  if (retry_count < 5) {
    Database::Warning() << "Database is busy, retrying " << retry_count;
    return 1;
  }
  (void)ptr;
  return 0;
}

}  // namespace sb::database
