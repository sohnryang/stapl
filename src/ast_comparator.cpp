#include <ast_comparator.h>
#include <cstddef>

namespace stapl::ast {
bool ASTComparator::operator()(const LiteralExprNode<int> &node1,
                               const LiteralExprNode<int> &node2) const {
  return node1.value == node2.value;
}

bool ASTComparator::operator()(const LiteralExprNode<double> &node1,
                               const LiteralExprNode<double> &node2) const {
  return node1.value == node2.value;
}

bool ASTComparator::operator()(const VariableExprNode &node1,
                               const VariableExprNode &node2) const {
  return node1.name == node2.name;
}

bool ASTComparator::operator()(
    const std::unique_ptr<BinaryExprNode> &node1,
    const std::unique_ptr<BinaryExprNode> &node2) const {
  return node1->op == node2->op && std::visit(*this, node1->lhs, node2->lhs) &&
         std::visit(*this, node1->rhs, node2->rhs);
}

bool ASTComparator::operator()(
    const std::unique_ptr<CallExprNode> &node1,
    const std::unique_ptr<CallExprNode> &node2) const {
  if (node1->callee != node2->callee)
    return false;
  if (node1->args.size() != node2->args.size())
    return false;
  for (size_t i = 0; i < node1->args.size(); i++)
    if (!std::visit(*this, node1->args[i], node2->args[i]))
      return false;
  return true;
}

bool ASTComparator::operator()(const PrototypeNode &node1,
                               const PrototypeNode &node2) const {
  if (node1.name != node2.name)
    return false;
  if (node1.return_type != node2.return_type)
    return false;
  if (node1.args.size() != node2.args.size())
    return false;
  for (size_t i = 0; i < node1.args.size(); i++)
    if (node1.args[i] != node2.args[i])
      return false;
  return true;
}

bool ASTComparator::operator()(const LetStmtNode &node1,
                               const LetStmtNode &node2) const {
  return node1.var_name == node2.var_name && node1.var_type == node2.var_type;
}

bool ASTComparator::operator()(const AssignmentStmtNode &node1,
                               const AssignmentStmtNode &node2) const {
  return node1.var_name == node2.var_name &&
         std::visit(*this, node1.assign_expr, node2.assign_expr);
}

bool ASTComparator::operator()(const std::unique_ptr<IfStmtNode> &node1,
                               const std::unique_ptr<IfStmtNode> &node2) const {
  return std::visit(*this, node1->condition, node2->condition) &&
         std::visit(*this, node1->then_stmt, node2->then_stmt) &&
         std::visit(*this, node1->else_stmt, node2->else_stmt);
}

bool ASTComparator::operator()(const ReturnStmtNode &node1,
                               const ReturnStmtNode &node2) const {
  return std::visit(*this, node1.return_expr, node2.return_expr);
}

bool ASTComparator::operator()(
    const std::unique_ptr<CompoundStmtNode> &node1,
    const std::unique_ptr<CompoundStmtNode> &node2) const {
  if (node1->stmts.size() != node2->stmts.size())
    return false;
  for (size_t i = 0; i < node1->stmts.size(); i++)
    if (node1->stmts[i] != node2->stmts[i])
      return false;
  return true;
}

bool ASTComparator::operator()(const FunctionDeclNode &node1,
                               const FunctionDeclNode &node2) const {
  if (!(*this)(node1.proto, node2.proto))
    return false;
  if (node1.func_body.has_value() != node2.func_body.has_value())
    return false;
  if (!node1.func_body.has_value())
    return true;
  return std::visit(*this, node1.func_body.value(), node2.func_body.value());
}

bool expr_equals(const ExprNode &expr1, const ExprNode &expr2) {
  ASTComparator cmp;
  return std::visit(cmp, expr1, expr2);
}

bool stmt_equals(const StmtNode &stmt1, const StmtNode &stmt2) {
  ASTComparator cmp;
  return std::visit(cmp, stmt1, stmt2);
}

bool decl_equals(const DeclNode &decl1, const DeclNode &decl2) {
  ASTComparator cmp;
  return std::visit(cmp, decl1, decl2);
}
} // namespace stapl::ast
