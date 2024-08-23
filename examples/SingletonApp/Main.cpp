#include "types/Singleton.hpp"

class Example : public sb::types::Singleton<Example> {};

int main(int argc, char *argv[]) {
  auto &ex = Example::Instance();
  return 0;
}
