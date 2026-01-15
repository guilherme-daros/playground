#ifndef SB_TYPES_RESULT_HPP
#define SB_TYPES_RESULT_HPP

#include <expected>
#include <string>

namespace sb {
template <typename T>
using Result = std::expected<T, std::string>;
}  // namespace sb

#endif