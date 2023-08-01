#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

/**
 * @brief Utility functions.
 */
namespace stapl::util {
/**
 * @brief Make ``std::vector`` from ``std::array``.
 * @tparam T Type of elements.
 * @tparam N Size of ``std::array``.
 * @param arr ``std::array`` to be converted.
 * @return ``std::vector`` with elements from ``arr``.
 *
 * Copied from [stack overflow](https://stackoverflow.com/a/42915152/5769640).
 */
template <typename T, size_t N>
std::vector<T> make_vector(std::array<T, N> &&arr) {
  return {std::make_move_iterator(arr.begin()),
          std::make_move_iterator(arr.end())};
}

/**
 * @brief Make ``std::vector`` from variadic arguments.
 * @tparam T Type of created ``std::vector``.
 * @tparam U Template parameter pack of varadic arguments.
 * @param t Variadic arguments.
 * @return ``std::vector<T>`` with elements from ``t``.
 *
 * Copied from [stack overflow](https://stackoverflow.com/a/42915152/5769640).
 * This function is useful when creating ``std::vector<T>`` of move-only types.
 */
template <typename T, typename... U> auto make_vector(U &&...t) {
  return make_vector(std::to_array<T>({std::forward<U>(t)...}));
}
} // namespace stapl::util
