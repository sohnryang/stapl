#include <ast_printer.h>

#include <variant>

#include <fmt/core.h>

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

std::string
ASTPrinter::operator()(const std::unique_ptr<IfExprNode> &node) const {
  return fmt::format("If({}, {}, {})", std::visit(*this, node->condition),
                     std::visit(*this, node->then_expr),
                     std::visit(*this, node->else_expr));
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

std::string ASTPrinter::operator()(const FunctionNode &node) const {
  if (node.extern_func)
    return fmt::format("ExternFunc({})", (*this)(node.proto));
  return fmt::format("Func({}, {})", (*this)(node.proto),
                     std::visit(*this, node.func_body.value()));
}
