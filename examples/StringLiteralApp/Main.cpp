#include "types/StringLiteral.hpp"

template <sb::types::StringLiteral domain>
class Example {};

namespace {
using Ex = Example<"Example">;
}

int main(int argc, char *argv[]) {
  auto ex = Ex();
  return 0;
}
