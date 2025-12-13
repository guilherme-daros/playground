#include "logger/Level.hpp"
#include "logger/Logger.hpp"
#include "logger/Output.hpp"

namespace {
using console_logger = sb::logger::Logger<"Console">;
using termina_logger = sb::logger::Logger<"Termina", sb::logger::output::Terminal>;

}  // namespace

int main() {
  console_logger::Info() << "HOLY THIS WORKS" << std::endl;
  termina_logger::Info() << "HOLY THIS WORKS" << std::endl;
  return 0;
}
