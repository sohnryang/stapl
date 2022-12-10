#pragma once

#include <ast.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

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

  Type operator()(const LiteralExprNode<int> &node) const;
  Type operator()(const LiteralExprNode<double> &node) const;
  Type operator()(const VariableExprNode &node) const;
  Type operator()(const std::unique_ptr<BinaryExprNode> &node) const;
  Type operator()(const std::unique_ptr<CallExprNode> &node) const;
  Type operator()(const std::unique_ptr<IfExprNode> &node) const;
};
