#include "typechecker.h"

#include <stdexcept>
#include <variant>

namespace stapl::types {
ExprTypeChecker::ExprTypeChecker() : globals(), functions() { init_table(); }

ExprTypeChecker::ExprTypeChecker(
    std::map<std::string, Type> &globals,
    std::map<std::string, std::pair<Type, std::vector<Type>>> functions)
    : globals(globals), functions(functions) {
  init_table();
}

void ExprTypeChecker::init_table() {
  binary_op_types["+"] = binary_op_types["-"] = binary_op_types["*"] =
      binary_op_types["/"] = {{{Type::kInt, Type::kInt}, Type::kInt},
                              {{Type::kInt, Type::kFloat}, Type::kFloat},
                              {{Type::kFloat, Type::kInt}, Type::kFloat},
                              {{Type::kFloat, Type::kFloat}, Type::kFloat}};
  binary_op_types["=="] = binary_op_types["<"] = binary_op_types["<="] =
      binary_op_types[">"] =
          binary_op_types[">="] = {{{Type::kInt, Type::kInt}, Type::kBool},
                                   {{Type::kInt, Type::kFloat}, Type::kBool},
                                   {{Type::kFloat, Type::kInt}, Type::kBool},
                                   {{Type::kFloat, Type::kFloat}, Type::kBool},
                                   {{Type::kBool, Type::kBool}, Type::kBool}};
  binary_op_types["if"] = {
      {{Type::kInt, Type::kInt}, Type::kInt},
      {{Type::kInt, Type::kFloat}, Type::kFloat},
      {{Type::kFloat, Type::kInt}, Type::kFloat},
      {{Type::kFloat, Type::kFloat}, Type::kFloat},
      {{Type::kBool, Type::kBool}, Type::kBool},
      {{Type::kVoid, Type::kBool}, Type::kVoid},
      {{Type::kVoid, Type::kInt}, Type::kVoid},
      {{Type::kVoid, Type::kFloat}, Type::kVoid},
      {{Type::kBool, Type::kVoid}, Type::kVoid},
      {{Type::kInt, Type::kVoid}, Type::kVoid},
      {{Type::kFloat, Type::kVoid}, Type::kVoid},
      {{Type::kVoid, Type::kVoid}, Type::kVoid},
  };
}

Type ExprTypeChecker::operator()(const ast::LiteralExprNode<int> &node) const {
  return Type::kInt;
}

Type ExprTypeChecker::operator()(
    const ast::LiteralExprNode<double> &node) const {
  return Type::kFloat;
}

Type ExprTypeChecker::operator()(const ast::VariableExprNode &node) const {
  return globals.at(node.name);
}

Type ExprTypeChecker::operator()(
    const std::unique_ptr<ast::BinaryExprNode> &node) const {
  auto lhs_type = std::visit(*this, node->lhs),
       rhs_type = std::visit(*this, node->rhs);
  return binary_op_types.at(node->op).at({lhs_type, rhs_type});
}

Type ExprTypeChecker::operator()(
    const std::unique_ptr<ast::CallExprNode> &node) const {
  for (auto &arg : node->args) {
    auto arg_type = std::visit(*this, arg);
    if (arg_type == Type::kVoid)
      throw std::logic_error("void not allowed here");
  }
  return functions.at(node->callee).first;
}
} // namespace stapl::types
