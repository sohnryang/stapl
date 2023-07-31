#pragma once

#include <string>
#include <vector>

namespace stapl::types {
/**
 * @brief Type information of a function.
 */
struct FuncTypeInfo {
  /**
   * @brief The type names of arguments.
   */
  std::vector<std::string> arg_types;

  /**
   * @brief The type name of returned value.
   */
  std::string return_type;

  /**
   * @brief Default constructor.
   */
  FuncTypeInfo() = default;

  /**
   * @brief Copy constructor.
   */
  FuncTypeInfo(const FuncTypeInfo &) = default;

  /**
   * @brief Move constructor.
   */
  FuncTypeInfo(FuncTypeInfo &&) = default;

  /**
   * @brief Instantiate from argument types and return type.
   * @param arg_types The types of arguments.
   * @param return_type The return type.
   */
  FuncTypeInfo(const std::vector<std::string> &arg_types,
               const std::string &return_type);

  /**
   * @brief Copy assignment operator.
   */
  FuncTypeInfo &operator=(const FuncTypeInfo &) = default;
};
} // namespace stapl::types
