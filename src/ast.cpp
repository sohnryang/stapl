#include "ast.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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

PrototypeNode::PrototypeNode(const std::string &name,
                             std::vector<std::pair<std::string, TypeNode>> args,
                             TypeNode return_type)
    : name(name), return_type(std::move(return_type)), args(std::move(args)) {}

LetStmtNode::LetStmtNode(const std::string &var_name, TypeNode var_type)
    : var_name(var_name), var_type(std::move(var_type)) {}

AssignmentStmtNode::AssignmentStmtNode(const std::string &var_name,
                                       ExprNode assign_expr)
    : var_name(var_name), assign_expr(std::move(assign_expr)) {}

ReturnStmtNode::ReturnStmtNode(ExprNode return_expr)
    : return_expr(std::move(return_expr)) {}

IfStmtNode::IfStmtNode(ExprNode condition, StmtNode then_stmt,
                       StmtNode else_stmt)
    : condition(std::move(condition)), then_stmt(std::move(then_stmt)),
      else_stmt(std::move(else_stmt)) {}

CompoundStmtNode::CompoundStmtNode(std::vector<StmtNode> stmts)
    : stmts(std::move(stmts)) {}

FunctionDeclNode::FunctionDeclNode(PrototypeNode proto, StmtNode func_body)
    : proto(std::move(proto)), func_body(std::move(func_body)) {}

FunctionDeclNode::FunctionDeclNode(PrototypeNode proto)
    : proto(std::move(proto)), func_body({}) {}

Module::Module(const std::string &name, std::vector<DeclNode> decls)
    : name(name), decls(std::move(decls)) {}

BasicTypeNode::BasicTypeNode(const std::string &name) : name(name) {}

template <typename T>
LiteralTypeTag<T>::LiteralTypeTag(const T &literal) : literal(literal) {}

TaggedTypeNode::TaggedTypeNode(const std::string &name,
                               std::vector<TypeTag> tags)
    : name(name), tags(std::move(tags)) {}

template class LiteralExprNode<int>;
template class LiteralExprNode<double>;
template class LiteralExprNode<bool>;

template class LiteralTypeTag<int>;
} // namespace stapl::ast
