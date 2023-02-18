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
  AST node for literal expressions, such as numbers.
 */
template <typename T> struct LiteralExprNode {
  T value;

  LiteralExprNode(LiteralExprNode<T> &&) = default;
  /**
    Instantiate from literal value.
   */
  LiteralExprNode(T value);
  LiteralExprNode<T> &operator=(LiteralExprNode<T> &&) = default;
};

/**
  AST node for variable expressions.
 */
struct VariableExprNode {
  std::string name;

  VariableExprNode(VariableExprNode &&) = default;
  /**
    Instantiate from variable name.
   */
  VariableExprNode(const std::string &name);
  VariableExprNode &operator=(VariableExprNode &&) = default;
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

  BinaryExprNode(BinaryExprNode &&) = default;
  BinaryExprNode(const std::string &op, ExprNode lhs, ExprNode rhs);
  BinaryExprNode &operator=(BinaryExprNode &&) = default;
};

/**
  AST node for function call expressions.
 */
struct CallExprNode {
  std::string callee;
  std::vector<ExprNode> args;

  CallExprNode(CallExprNode &&) = default;
  CallExprNode(const std::string &callee, std::vector<ExprNode> args);
  CallExprNode &operator=(CallExprNode &&) = default;
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
};

/**
  AST node for let statement.
 */
struct LetStmtNode {
  std::string var_name, var_type;

  LetStmtNode(LetStmtNode &&) = default;
  LetStmtNode(const std::string &var_name, const std::string &var_type);
  LetStmtNode &operator=(LetStmtNode &&) = default;
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
};

/**
  AST node for return statement.
 */
struct ReturnStmtNode {
  ExprNode return_expr;
  ReturnStmtNode(ReturnStmtNode &&) = default;
  ReturnStmtNode(ExprNode return_expr);
  ReturnStmtNode &operator=(ReturnStmtNode &&) = default;
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
};

/**
  AST node for compound statement.
 */
struct CompoundStmtNode {
  std::vector<std::unique_ptr<StmtNode>> stmts;

  CompoundStmtNode(CompoundStmtNode &&) = default;
  CompoundStmtNode(std::vector<std::unique_ptr<StmtNode>> stmts);
  CompoundStmtNode &operator=(CompoundStmtNode &&) = default;
};

/**
  AST node for function definition.
 */
struct FunctionNode {
  PrototypeNode proto;
  std::optional<CompoundStmtNode> func_body;

  FunctionNode(FunctionNode &&) = default;
  FunctionNode(PrototypeNode proto, CompoundStmtNode func_body);
  FunctionNode(PrototypeNode proto);
  FunctionNode &operator=(FunctionNode &&) = default;
};
} // namespace stapl::ast
