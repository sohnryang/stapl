#pragma once

#include <ast.h>

#include <memory>
#include <string>

namespace stapl::ast {
/**
  A visitor for printing AST nodes.
 */
class ASTPrinter {
public:
  std::string operator()(const LiteralExprNode<int> &node) const;
  std::string operator()(const LiteralExprNode<double> &node) const;
  std::string operator()(const VariableExprNode &node) const;
  std::string operator()(const std::unique_ptr<BinaryExprNode> &node) const;
  std::string operator()(const std::unique_ptr<CallExprNode> &node) const;
  std::string operator()(const std::unique_ptr<IfExprNode> &node) const;
  std::string operator()(const PrototypeNode &node) const;
  std::string operator()(const FunctionNode &node) const;
};
} // namespace stapl::ast
