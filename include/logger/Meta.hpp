#pragma once

#include <concepts>

namespace sb::logger::meta {

template <typename Base, auto Predicate, typename... Types>
struct TypeFinder;

template <auto F, typename T>
concept PredicateLambda = requires {
  { F.template operator()<T>() } -> std::convertible_to<bool>;
};

template <auto F, typename T>
concept PredicateIsTrue = PredicateLambda<F, T> && F.template operator()<T>();

template <auto F, typename T>
concept PredicateIsFalse = PredicateLambda<F, T> && !F.template operator()<T>();

template <typename Base, auto Predicate, typename First, typename... Rest>
  requires PredicateIsTrue<Predicate, First>
struct TypeFinder<Base, Predicate, First, Rest...> {
  using type = First;
};

template <typename Base, auto Predicate, typename First, typename... Rest>
  requires PredicateIsFalse<Predicate, First>
struct TypeFinder<Base, Predicate, First, Rest...> {
  using type = typename TypeFinder<Base, Predicate, Rest...>::type;
};

template <typename Base, auto Predicate>
struct TypeFinder<Base, Predicate> {
  using type = typename Base::Default;
};

template <typename Base, auto Predicate, typename... Types>
using TypeFinder_t = typename TypeFinder<Base, Predicate, Types...>::type;

}  // namespace sb::logger::meta
