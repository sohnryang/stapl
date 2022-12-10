#include <ast.h>

#include <cstdint>
#include <stdexcept>

#include <fmt/core.h>

template <typename T>
LiteralExprNode<T>::LiteralExprNode(T value) : value(value) {}

VariableExprNode::VariableExprNode(const std::string &name) : name(name) {}

BinaryExprNode::BinaryExprNode(const std::string &op, ExprNode lhs,
                               ExprNode rhs)
    : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

CallExprNode::CallExprNode(const std::string &callee,
                           std::vector<ExprNode> args)
    : callee(callee), args(std::move(args)) {}

IfExprNode::IfExprNode(ExprNode condition, ExprNode then_expr,
                       ExprNode else_expr)
    : condition(std::move(condition)), then_expr(std::move(then_expr)),
      else_expr(std::move(else_expr)) {}

PrototypeNode::PrototypeNode(
    const std::string &name,
    std::vector<std::pair<std::string, std::string>> args,
    std::string return_type)
    : name(name), args(std::move(args)), return_type(return_type) {}

FunctionNode::FunctionNode(PrototypeNode proto, ExprNode func_body)
    : proto(std::move(proto)), func_body(std::move(func_body)),
      extern_func(false) {}

FunctionNode::FunctionNode(PrototypeNode proto)
    : proto(std::move(proto)), func_body({}), extern_func(true) {}

template class LiteralExprNode<int>;
template class LiteralExprNode<double>;
