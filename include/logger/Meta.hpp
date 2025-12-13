#pragma once

#include <concepts>

namespace sb::logger::meta {

// Helper concept to verify that the Predicate is a trait-like struct
// with a boolean `value` member for a given type.
template <template <typename> class Trait, typename T>
concept PredicateTrait = requires {
  { Trait<T>::value } -> std::convertible_to<bool>;
};

template <typename Base, template <typename> typename Predicate, typename... Types>
struct TypeFinder;

// Recursive case: The first type in the list satisfies the predicate.
// The `requires` clause checks this condition.
template <typename Base, template <typename> typename Predicate, typename First, typename... Rest>
  requires PredicateTrait<Predicate, First> && Predicate<First>::value
struct TypeFinder<Base, Predicate, First, Rest...> {
  using type = First;
};

// Recursive case: The first type does not satisfy the predicate.
// The `requires` clause checks this, and we recurse on the rest of the types.
template <typename Base, template <typename> typename Predicate, typename First, typename... Rest>
  requires PredicateTrait<Predicate, First> && (!Predicate<First>::value)
struct TypeFinder<Base, Predicate, First, Rest...> {
  using type = typename TypeFinder<Base, Predicate, Rest...>::type;
};

// Base case: The list of types is empty, so we fall back to the default type.
template <typename Base, template <typename> typename Predicate>
struct TypeFinder<Base, Predicate> {
  using type = typename Base::Default;
};

template <typename Base, template <typename> typename Predicate, typename... Types>
using TypeFinder_t = typename TypeFinder<Base, Predicate, Types...>::type;
}  // namespace sb::logger::meta
