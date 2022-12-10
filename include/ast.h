#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

template <typename T> struct LiteralExprNode {
  T value;

  LiteralExprNode(LiteralExprNode<T> &&) = default;
  LiteralExprNode(T value);
  LiteralExprNode<T> &operator=(LiteralExprNode<T> &&) = default;
};

struct VariableExprNode {
  std::string name;

  VariableExprNode(VariableExprNode &&) = default;
  VariableExprNode(const std::string &name);
  VariableExprNode &operator=(VariableExprNode &&) = default;
};

using ExprNode =
    std::variant<LiteralExprNode<int>, LiteralExprNode<double>,
                 VariableExprNode, std::unique_ptr<struct BinaryExprNode>,
                 std::unique_ptr<struct CallExprNode>,
                 std::unique_ptr<struct IfExprNode>>;

struct BinaryExprNode {
  std::string op;
  ExprNode lhs, rhs;

  BinaryExprNode(BinaryExprNode &&) = default;
  BinaryExprNode(const std::string &op, ExprNode lhs, ExprNode rhs);
  BinaryExprNode &operator=(BinaryExprNode &&) = default;
};

struct CallExprNode {
  std::string callee;
  std::vector<ExprNode> args;

  CallExprNode(CallExprNode &&) = default;
  CallExprNode(const std::string &callee, std::vector<ExprNode> args);
  CallExprNode &operator=(CallExprNode &&) = default;
};

struct IfExprNode {
  ExprNode condition, then_expr, else_expr;

  IfExprNode(IfExprNode &&) = default;
  IfExprNode(ExprNode condition, ExprNode then_expr, ExprNode else_expr);
  IfExprNode &operator=(IfExprNode &&) = default;
};

struct PrototypeNode {
  std::string name;
  std::string return_type;
  std::vector<std::pair<std::string, std::string>> args;

  PrototypeNode(PrototypeNode &&) = default;
  PrototypeNode(const std::string &name,
                std::vector<std::pair<std::string, std::string>> args,
                std::string return_type);
  PrototypeNode &operator=(PrototypeNode &&) = default;
};

struct FunctionNode {
  PrototypeNode proto;
  std::optional<ExprNode> func_body;
  bool extern_func;

  FunctionNode(FunctionNode &&) = default;
  FunctionNode(PrototypeNode proto, ExprNode func_body);
  FunctionNode(PrototypeNode proto);
  FunctionNode &operator=(FunctionNode &&) = default;
};

using StatementNode = std::variant<FunctionNode>;
