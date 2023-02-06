#pragma once

#include <ast.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace stapl::types {
enum class Type {
  kInt,
  kFloat,
  kBool,
  kVoid,
};

class ExprTypeChecker {
private:
  std::map<std::string, Type> globals;
  std::map<std::string, std::pair<Type, std::vector<Type>>> functions;
  std::map<std::string, std::map<std::pair<Type, Type>, Type>> binary_op_types;
  void init_table();

public:
  ExprTypeChecker();
  ExprTypeChecker(
      std::map<std::string, Type> &globals,
      std::map<std::string, std::pair<Type, std::vector<Type>>> functions);

  Type operator()(const ast::LiteralExprNode<int> &node) const;
  Type operator()(const ast::LiteralExprNode<double> &node) const;
  Type operator()(const ast::VariableExprNode &node) const;
  Type operator()(const std::unique_ptr<ast::BinaryExprNode> &node) const;
  Type operator()(const std::unique_ptr<ast::CallExprNode> &node) const;
  Type operator()(const std::unique_ptr<ast::IfExprNode> &node) const;
};
} // namespace stapl::types
