#include <ast_printer.h>

#include <variant>

#include <fmt/core.h>

namespace stapl::ast {
std::string ASTPrinter::operator()(const LiteralExprNode<int> &node) const {
  return fmt::format("Literal({}, int)", node.value);
}

std::string ASTPrinter::operator()(const LiteralExprNode<double> &node) const {
  return fmt::format("Literal({}, float)", node.value);
}

std::string ASTPrinter::operator()(const VariableExprNode &node) const {
  return fmt::format("Variable({})", node.name);
}

std::string
ASTPrinter::operator()(const std::unique_ptr<BinaryExprNode> &node) const {

  return fmt::format("BinaryExpr({}, {}, {})", node->op,
                     std::visit(*this, node->lhs),
                     std::visit(*this, node->rhs));
}

std::string
ASTPrinter::operator()(const std::unique_ptr<CallExprNode> &node) const {
  std::string arg_str;
  for (auto &arg : node->args) {
    if (!arg_str.empty())
      arg_str.append(", ");
    arg_str.append(std::visit(*this, arg));
  }
  return fmt::format("CallExpr({}, [{}])", node->callee, arg_str);
}

std::string ASTPrinter::operator()(const PrototypeNode &node) const {
  std::string arg_str;
  for (auto &arg : node.args) {
    if (!arg_str.empty())
      arg_str.append(", ");
    arg_str.append(fmt::format("Arg({}, {})", arg.first, arg.second));
  }
  return fmt::format("Prototype({}, [{}], {})", node.name, arg_str,
                     node.return_type);
}

std::string ASTPrinter::operator()(const LetStmtNode &node) const {
  return fmt::format("Let({}, {})", node.var_name, node.var_type);
}

std::string ASTPrinter::operator()(const AssignmentStmtNode &node) const {
  return fmt::format("Assign({}, {})", node.var_name,
                     std::visit(*this, node.assign_expr));
}

std::string
ASTPrinter::operator()(const std::unique_ptr<IfStmtNode> &node) const {
  return fmt::format("If({}, {}, {})", std::visit(*this, node->condition),
                     std::visit(*this, node->then_stmt),
                     std::visit(*this, node->else_stmt));
}

std::string ASTPrinter::operator()(const ReturnStmtNode &node) const {
  return fmt::format("Return({})", std::visit(*this, node.return_expr));
}

std::string
ASTPrinter::operator()(const std::unique_ptr<CompoundStmtNode> &node) const {
  std::string stmt_str;
  for (auto &stmt : node->stmts) {
    if (!stmt_str.empty())
      stmt_str.append(", ");
    stmt_str.append(std::visit(*this, stmt));
  }
  return fmt::format("Compound([{}])", stmt_str);
}

std::string ASTPrinter::operator()(const FunctionDeclNode &node) const {
  if (!node.func_body.has_value())
    return fmt::format("Func({})", (*this)(node.proto));
  return fmt::format("Func({}, {})", (*this)(node.proto),
                     std::visit(*this, node.func_body.value()));
}
} // namespace stapl::ast
