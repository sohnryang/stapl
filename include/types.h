#pragma once

#include <string>
#include <vector>

namespace stapl::types {
struct FuncTypeInfo {
  std::vector<std::string> arg_types;
  std::string return_type;

  FuncTypeInfo() = default;
  FuncTypeInfo(const FuncTypeInfo &) = default;
  FuncTypeInfo(FuncTypeInfo &&) = default;
  FuncTypeInfo(const std::vector<std::string> &arg_types,
               const std::string &return_type);
  FuncTypeInfo &operator=(const FuncTypeInfo &) = default;
};
} // namespace stapl::types
