#pragma once

#include <ast.h>
#include <memory>

namespace stapl::ast {
/**
  A visitor for comparing AST nodes.
 */
class ASTComparator {
public:
  bool operator()(const LiteralExprNode<int> &node1,
                  const LiteralExprNode<int> &node2) const;
  bool operator()(const LiteralExprNode<double> &node1,
                  const LiteralExprNode<double> &node2) const;
  bool operator()(const VariableExprNode &node1,
                  const VariableExprNode &node2) const;
  bool operator()(const std::unique_ptr<BinaryExprNode> &node1,
                  const std::unique_ptr<BinaryExprNode> &node2) const;
  bool operator()(const std::unique_ptr<CallExprNode> &node1,
                  const std::unique_ptr<CallExprNode> &node2) const;
  bool operator()(const PrototypeNode &node1, const PrototypeNode &node2) const;
  bool operator()(const LetStmtNode &node1, const LetStmtNode &node2) const;
  bool operator()(const AssignmentStmtNode &node1,
                  const AssignmentStmtNode &node2) const;
  bool operator()(const std::unique_ptr<IfStmtNode> &node1,
                  const std::unique_ptr<IfStmtNode> &node2) const;
  bool operator()(const ReturnStmtNode &node1,
                  const ReturnStmtNode &node2) const;
  bool operator()(const std::unique_ptr<CompoundStmtNode> &node1,
                  const std::unique_ptr<CompoundStmtNode> &node2) const;
  bool operator()(const FunctionDeclNode &node1,
                  const FunctionDeclNode &node2) const;

  template <typename T1, typename T2>
  bool operator()(const T1 &, const T2 &) const {
    return false;
  }
};
} // namespace stapl::ast
