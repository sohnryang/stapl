#include "types.h"
#include <string>
#include <vector>

namespace stapl::types {
FuncTypeInfo::FuncTypeInfo(const std::vector<std::string> &arg_types,
                           const std::string &return_type)
    : arg_types(arg_types), return_type(return_type) {}
} // namespace stapl::types
