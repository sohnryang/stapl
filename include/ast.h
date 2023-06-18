#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace stapl::ast {
/**
  Comparision operator overload for unique pointers of AST nodes.
 */
template <typename T>
inline bool operator==(const std::unique_ptr<T> &p1,
                       const std::unique_ptr<T> &p2) {
  if (p1 == nullptr || p2 == nullptr)
    return p1 == p2;
  return *p1 == *p2;
}

/**
  AST node for literal expressions, such as numbers.
 */
template <typename T> struct LiteralExprNode {
  T value;
  std::optional<std::string> expr_type = {};

  LiteralExprNode(LiteralExprNode<T> &&) = default;
  /**
    Instantiate from literal value.
   */
  LiteralExprNode(T value);
  LiteralExprNode<T> &operator=(LiteralExprNode<T> &&) = default;
  bool operator==(const LiteralExprNode<T> &rhs) const = default;
};

/**
  AST node for variable expressions.
 */
struct VariableExprNode {
  std::string name;
  std::optional<std::string> expr_type = {};

  VariableExprNode(VariableExprNode &&) = default;
  /**
    Instantiate from variable name.
   */
  VariableExprNode(const std::string &name);
  VariableExprNode &operator=(VariableExprNode &&) = default;
  bool operator==(const VariableExprNode &rhs) const = default;
};

/**
  Variant for expression nodes.
 */
using ExprNode =
    std::variant<LiteralExprNode<int>, LiteralExprNode<double>,
                 VariableExprNode, std::unique_ptr<struct BinaryExprNode>,
                 std::unique_ptr<struct CallExprNode>>;

/**
  AST node for binary expressions.
 */
struct BinaryExprNode {
  std::string op;
  ExprNode lhs, rhs;
  std::optional<std::string> expr_type = {};

  BinaryExprNode(BinaryExprNode &&) = default;
  BinaryExprNode(const std::string &op, ExprNode lhs, ExprNode rhs);
  BinaryExprNode &operator=(BinaryExprNode &&) = default;
  bool operator==(const BinaryExprNode &rhs) const = default;
};

/**
  AST node for function call expressions.
 */
struct CallExprNode {
  std::string callee;
  std::vector<ExprNode> args;
  std::optional<std::string> expr_type = {};

  CallExprNode(CallExprNode &&) = default;
  CallExprNode(const std::string &callee, std::vector<ExprNode> args);
  CallExprNode &operator=(CallExprNode &&) = default;
  bool operator==(const CallExprNode &rhs) const = default;
};

/**
  AST node for function prototype.
 */
struct PrototypeNode {
  std::string name, return_type;
  std::vector<std::pair<std::string, std::string>> args;

  PrototypeNode(PrototypeNode &&) = default;
  PrototypeNode(const std::string &name,
                std::vector<std::pair<std::string, std::string>> args,
                const std::string &return_type);
  PrototypeNode &operator=(PrototypeNode &&) = default;
  bool operator==(const PrototypeNode &rhs) const = default;
};

/**
  AST node for let statement.
 */
struct LetStmtNode {
  std::string var_name, var_type;

  LetStmtNode(LetStmtNode &&) = default;
  LetStmtNode(const std::string &var_name, const std::string &var_type);
  LetStmtNode &operator=(LetStmtNode &&) = default;
  bool operator==(const LetStmtNode &rhs) const = default;
};

/**
  AST node for assignment statement.
 */
struct AssignmentStmtNode {
  std::string var_name;
  ExprNode assign_expr;

  AssignmentStmtNode(AssignmentStmtNode &&) = default;
  AssignmentStmtNode(const std::string &var_name, ExprNode assign_expr);
  AssignmentStmtNode &operator=(AssignmentStmtNode &&) = default;
  bool operator==(const AssignmentStmtNode &rhs) const = default;
};

/**
  AST node for return statement.
 */
struct ReturnStmtNode {
  ExprNode return_expr;
  ReturnStmtNode(ReturnStmtNode &&) = default;
  ReturnStmtNode(ExprNode return_expr);
  ReturnStmtNode &operator=(ReturnStmtNode &&) = default;
  bool operator==(const ReturnStmtNode &rhs) const = default;
};

/**
  Variant for statement nodes.
 */
using StmtNode =
    std::variant<LetStmtNode, AssignmentStmtNode,
                 std::unique_ptr<struct IfStmtNode>, ReturnStmtNode,
                 std::unique_ptr<struct CompoundStmtNode>>;

/**
  AST node for if statement.
 */
struct IfStmtNode {
  ExprNode condition;
  StmtNode then_stmt, else_stmt;

  IfStmtNode(IfStmtNode &&) = default;
  IfStmtNode(ExprNode condition, StmtNode then_stmt, StmtNode else_stmt);
  IfStmtNode &operator=(IfStmtNode &&) = default;
  bool operator==(const IfStmtNode &rhs) const = default;
};

/**
  AST node for compound statement.
 */
struct CompoundStmtNode {
  std::vector<StmtNode> stmts;

  CompoundStmtNode(CompoundStmtNode &&) = default;
  CompoundStmtNode(std::vector<StmtNode> stmts);
  CompoundStmtNode &operator=(CompoundStmtNode &&) = default;
  bool operator==(const CompoundStmtNode &rhs) const = default;
};

/**
  AST node for function definition.
 */
struct FunctionDeclNode {
  PrototypeNode proto;
  std::optional<StmtNode> func_body;

  FunctionDeclNode(FunctionDeclNode &&) = default;
  FunctionDeclNode(PrototypeNode proto, StmtNode func_body);
  FunctionDeclNode(PrototypeNode proto);
  FunctionDeclNode &operator=(FunctionDeclNode &&) = default;
  bool operator==(const FunctionDeclNode &rhs) const = default;
};

/**
  Variant for declaration nodes.
 */
using DeclNode = std::variant<FunctionDeclNode>;
} // namespace stapl::ast
