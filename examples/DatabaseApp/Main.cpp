#include "database/Handler.hpp"
#include "logger/Logger.hpp"

using Main = sb::logger::Logger<"Main">;

int main(int argc, char *argv[]) {
  auto h = sb::database::Handler<"Test.db">::Instance();

  (void)argc;
  (void)argv;
  return 0;
}
