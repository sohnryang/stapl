#include <ast.h>

#include <cstdint>
#include <stdexcept>

#include <fmt/core.h>

namespace stapl::ast {
template <typename T>
LiteralExprNode<T>::LiteralExprNode(T value) : value(value) {}

VariableExprNode::VariableExprNode(const std::string &name) : name(name) {}

BinaryExprNode::BinaryExprNode(const std::string &op, ExprNode lhs,
                               ExprNode rhs)
    : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

CallExprNode::CallExprNode(const std::string &callee,
                           std::vector<ExprNode> args)
    : callee(callee), args(std::move(args)) {}

PrototypeNode::PrototypeNode(
    const std::string &name,
    std::vector<std::pair<std::string, std::string>> args,
    const std::string &return_type)
    : name(name), return_type(return_type), args(args) {}

LetStmtNode::LetStmtNode(const std::string &var_name,
                         const std::string &var_type)
    : var_name(var_name), var_type(var_type) {}

AssignmentStmtNode::AssignmentStmtNode(const std::string &var_name,
                                       ExprNode assign_expr)
    : var_name(var_name), assign_expr(std::move(assign_expr)) {}

ReturnStmtNode::ReturnStmtNode(ExprNode return_expr)
    : return_expr(std::move(return_expr)) {}

IfStmtNode::IfStmtNode(ExprNode condition, StmtNode then_stmt,
                       StmtNode else_stmt)
    : condition(std::move(condition)), then_stmt(std::move(then_stmt)),
      else_stmt(std::move(else_stmt)) {}

CompoundStmtNode::CompoundStmtNode(std::vector<std::unique_ptr<StmtNode>> stmts)
    : stmts(std::move(stmts)) {}

FunctionDeclNode::FunctionDeclNode(PrototypeNode proto, StmtNode func_body)
    : proto(std::move(proto)), func_body(std::move(func_body)) {}

FunctionDeclNode::FunctionDeclNode(PrototypeNode proto)
    : proto(std::move(proto)), func_body({}) {}

template class LiteralExprNode<int>;
template class LiteralExprNode<double>;
} // namespace stapl::ast
