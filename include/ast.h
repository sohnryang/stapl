#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

/**
 * @brief Namespace of AST nodes and utility functions.
 */
namespace stapl::ast {
/**
 * @brief Comparision operator overload for unique pointers of AST nodes.
 * @param p1 ``std::unique_ptr`` on the LHS.
 * @param p2 ``std::unique_ptr`` on the RHS.
 * @return Whether the objects referenced by ``p1`` and ``p2`` are equal.
 */
template <typename T>
inline bool operator==(const std::unique_ptr<T> &p1,
                       const std::unique_ptr<T> &p2) {
  if (p1 == nullptr || p2 == nullptr)
    return p1 == p2;
  return *p1 == *p2;
}

/**
 * @brief AST node for literal expressions, such as numbers.
 * @todo Add support for types other than ``int``, ``double`` and ``bool``.
 */
template <typename T> struct LiteralExprNode {
  /**
   * @brief Literal value.
   */
  T value;

  /**
   * @brief Type of the literal expression.
   */
  std::optional<std::string> expr_type = {};

  /**
   * @brief Move constructor.
   */
  LiteralExprNode(LiteralExprNode<T> &&) = default;

  /**
   * @brief Instantiate from literal value.
   * @param value Literal value.
   */
  explicit LiteralExprNode(T value);

  /**
   * @brief Move assignment operator.
   */
  LiteralExprNode<T> &operator=(LiteralExprNode<T> &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``LiteralExprNode<T>`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const LiteralExprNode<T> &rhs) const = default;
};

/**
 * @brief AST node for variable expressions.
 */
struct VariableExprNode {
  /**
   * @brief Variable name.
   */
  std::string name;

  /**
   * @brief Type of the variable expression.
   */
  std::optional<std::string> expr_type = {};

  /**
   * @brief Move constructor.
   */
  VariableExprNode(VariableExprNode &&) = default;

  /**
   * @brief Instantiate from variable name.
   * @param name Variable name.
   */
  explicit VariableExprNode(const std::string &name);

  /**
   * @brief Move assignment operator.
   */
  VariableExprNode &operator=(VariableExprNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``VariableExprNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const VariableExprNode &rhs) const = default;
};

/**
 * @brief Variant for expression nodes.
 */
using ExprNode = std::variant<LiteralExprNode<int>, LiteralExprNode<double>,
                              LiteralExprNode<bool>, VariableExprNode,
                              std::unique_ptr<struct UnaryExprNode>,
                              std::unique_ptr<struct BinaryExprNode>,
                              std::unique_ptr<struct CallExprNode>>;

/**
 * @brief AST node for unary expressions.
 */
struct UnaryExprNode {
  /**
   * @brief Operator of the unary expression.
   */
  std::string op;

  /**
   * @brief Operand of the unary expression.
   */
  ExprNode rhs;

  /**
   * @brief Type of the unary expression.
   */
  std::optional<std::string> expr_type = {};

  /**
   * @brief Move constructor.
   */
  UnaryExprNode(UnaryExprNode &&) = default;

  /**
   * @brief Instantiate from operator and operand.
   * @param op Operator of the unary expression.
   * @param rhs Operand of the unary expression.
   */
  explicit UnaryExprNode(const std::string &op, ExprNode rhs);

  /**
   * @brief Move assignment operator.
   */
  UnaryExprNode &operator=(UnaryExprNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``UnaryExprNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const UnaryExprNode &rhs) const = default;
};

/**
 * @brief AST node for binary expressions.
 */
struct BinaryExprNode {
  /**
   * @brief Operator of the binary expression.
   */
  std::string op;

  /**
   * @brief LHS of the binary expression.
   */
  ExprNode lhs;

  /**
   * @brief RHS of the binary expression.
   */
  ExprNode rhs;

  /**
   * @brief Type of the binary expression.
   */
  std::optional<std::string> expr_type = {};

  /**
   * @brief Move constructor.
   */
  BinaryExprNode(BinaryExprNode &&) = default;

  /**
   * @brief Instantiate from operator and operands.
   * @param op Operator of the binary expression.
   * @param lhs LHS of the binary expression.
   * @param rhs RHS of the binary expression.
   */
  explicit BinaryExprNode(const std::string &op, ExprNode lhs, ExprNode rhs);

  /**
   * @brief Move assignment operator.
   */
  BinaryExprNode &operator=(BinaryExprNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``BinaryExprNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const BinaryExprNode &rhs) const = default;
};

/**
 * @brief AST node for function call expressions.
 */
struct CallExprNode {
  /**
   * @brief Function to call.
   */
  std::string callee;

  /**
   * @brief Arguments of the function call.
   */
  std::vector<ExprNode> args;

  /**
   * @brief Type of the function call expression.
   */
  std::optional<std::string> expr_type = {};

  /**
   * @brief Move constructor.
   */
  CallExprNode(CallExprNode &&) = default;

  /**
   * @brief Instantiate from function name and arguments.
   * @param callee Function to call.
   * @param args Arguments of the function call.
   */
  explicit CallExprNode(const std::string &callee, std::vector<ExprNode> args);

  /**
   * @brief Move assignment operator.
   */
  CallExprNode &operator=(CallExprNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``CallExprNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const CallExprNode &rhs) const = default;
};

/**
 * @brief AST node for basic types.
 */
struct BasicTypeNode {
  /**
   * @brief Name of the type.
   */
  std::string name;

  /**
   * @brief Move constructor.
   */
  BasicTypeNode(BasicTypeNode &&) = default;

  /**
   * @brief Instantiate from name of the type.
   * @param name Name of the type.
   */
  explicit BasicTypeNode(const std::string &name);

  /**
   * @brief Move assignment operator.
   */
  BasicTypeNode &operator=(BasicTypeNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``BasicTypeNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const BasicTypeNode &rhs) const = default;
};

/**
 * @brief AST node for types.
 */
using TypeNode =
    std::variant<std::unique_ptr<struct TaggedTypeNode>, BasicTypeNode>;

/**
 * @brief AST node for type tag.
 */
template <typename T> struct LiteralTypeTag {
  /**
   * @brief Literal value.
   */
  T literal;

  /**
   * @brief Move constructor.
   */
  LiteralTypeTag(LiteralTypeTag &&) = default;

  /**
   * @brief Instantiate from literal value.
   * @param literal Literal value.
   */
  explicit LiteralTypeTag(const T &literal);

  /**
   * @brief Move assignment operator.
   */
  LiteralTypeTag &operator=(LiteralTypeTag &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``LiteralTypeTag<T>`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const LiteralTypeTag &rhs) const = default;
};

/**
 * @brief Variant for type tags.
 */
using TypeTag = std::variant<TypeNode, LiteralTypeTag<int>>;

/**
 * @brief AST node for tagged types.
 */
struct TaggedTypeNode {
  /**
   * @brief Tags of the type.
   */
  std::vector<TypeTag> tags;

  /**
   * @brief Name of the type.
   */
  std::string name;

  /**
   * @brief Move constructor.
   */
  TaggedTypeNode(TaggedTypeNode &&) = default;

  /**
   * @brief Instantiate from name and tags.
   * @param name Name of the type.
   * @param tags Tags of the type.
   */
  explicit TaggedTypeNode(const std::string &name, std::vector<TypeTag> tags);

  /**
   * @brief Move assignment operator.
   */
  TaggedTypeNode &operator=(TaggedTypeNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``TaggedTypeNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const TaggedTypeNode &rhs) const = default;
};

/**
 * @brief AST node for function prototype.
 */
struct PrototypeNode {
  /**
   * @brief Function name.
   */
  std::string name;

  /**
   * @brief Return type name of the function.
   */
  TypeNode return_type;

  /**
   * @brief Arguments names and types of the function.
   */
  std::vector<std::pair<std::string, TypeNode>> args;

  /**
   * @brief Move constructor.
   */
  PrototypeNode(PrototypeNode &&) = default;

  /**
   * @brief Instantiate from function name, pair of argument names and types,
   * and return type.
   * @param name Function name.
   * @param args Arguments names and types of the function.
   * @param return_type Return type name of the function.
   */
  explicit PrototypeNode(const std::string &name,
                         std::vector<std::pair<std::string, TypeNode>> args,
                         TypeNode return_type);

  /**
   * @brief Move assignment operator.
   */
  PrototypeNode &operator=(PrototypeNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``PrototypeNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const PrototypeNode &rhs) const = default;
};

/**
 * @brief AST node for let statement.
 */
struct LetStmtNode {
  /**
   * @brief Variable name.
   */
  std::string var_name;

  /**
   * @brief Type name of the variable.
   */
  TypeNode var_type;

  /**
   * @brief Move constructor.
   */
  LetStmtNode(LetStmtNode &&) = default;

  /**
   * @brief Instantiate from variable name and type.
   * @param var_name Variable name.
   * @param var_type Type name of the variable.
   */
  explicit LetStmtNode(const std::string &var_name, TypeNode var_type);

  /**
   * @brief Move assignment operator.
   */
  LetStmtNode &operator=(LetStmtNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``LetStmtNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const LetStmtNode &rhs) const = default;
};

/**
 * @brief AST node for assignment statement.
 */
struct AssignmentStmtNode {
  /**
   * @brief Name of the variable to be assigned.
   */
  std::string var_name;

  /**
   * @brief Expression to be assigned.
   */
  ExprNode assign_expr;

  /**
   * @brief Move constructor.
   */
  AssignmentStmtNode(AssignmentStmtNode &&) = default;

  /**
   * @brief Instantiate from variable name and assignment expression.
   * @param var_name Name of the variable to be assigned.
   * @param assign_expr Expression to be assigned.
   */
  explicit AssignmentStmtNode(const std::string &var_name,
                              ExprNode assign_expr);

  /**
   * @brief Move assignment operator.
   */
  AssignmentStmtNode &operator=(AssignmentStmtNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``AssignmentStmtNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const AssignmentStmtNode &rhs) const = default;
};

/**
 * @brief AST node for return statement.
 */
struct ReturnStmtNode {
  /**
   * @brief Expression to be returned.
   */
  ExprNode return_expr;

  /**
   * @brief Move constructor.
   */
  ReturnStmtNode(ReturnStmtNode &&) = default;

  /**
   * @brief Instantiate from expression to be returned.
   * @param return_expr Expression to be returned.
   */
  explicit ReturnStmtNode(ExprNode return_expr);

  /**
   * @brief Move assignment operator.
   */
  ReturnStmtNode &operator=(ReturnStmtNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``ReturnStmtNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const ReturnStmtNode &rhs) const = default;
};

/**
 * @brief Variant for statement nodes.
 */
using StmtNode =
    std::variant<LetStmtNode, AssignmentStmtNode,
                 std::unique_ptr<struct IfStmtNode>, ReturnStmtNode,
                 std::unique_ptr<struct CompoundStmtNode>>;

/**
 * @brief AST node for if statement.
 */
struct IfStmtNode {
  /**
   * @brief Condition expression.
   */
  ExprNode condition;

  /**
   * @brief Statement to be executed if the condition is true.
   */
  StmtNode then_stmt;

  /**
   * @brief Statement to be executed if the condition is false.
   */
  StmtNode else_stmt;

  /**
   * @brief Move constructor.
   */
  IfStmtNode(IfStmtNode &&) = default;

  /**
   * @brief Instantiate from condition expression, statement to be executed if
   * the condition is true, and statement to be executed if the condition is
   * false.
   * @param condition Condition expression.
   * @param then_stmt Statement to be executed if the condition is true.
   * @param else_stmt Statement to be executed if the condition is false.
   */
  explicit IfStmtNode(ExprNode condition, StmtNode then_stmt,
                      StmtNode else_stmt);

  /**
   * @brief Move assignment operator.
   */
  IfStmtNode &operator=(IfStmtNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``IfStmtNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const IfStmtNode &rhs) const = default;
};

/**
 * @brief AST node for compound statement.
 */
struct CompoundStmtNode {
  /**
   * @brief Statements in the compound statement.
   */
  std::vector<StmtNode> stmts;

  /**
   * @brief Move constructor.
   */
  CompoundStmtNode(CompoundStmtNode &&) = default;

  /**
   * @brief Instantiate from statements in the compound statement.
   * @param stmts Statements in the compound statement.
   */
  explicit CompoundStmtNode(std::vector<StmtNode> stmts);

  /**
   * @brief Move assignment operator.
   */
  CompoundStmtNode &operator=(CompoundStmtNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``CompoundStmtNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const CompoundStmtNode &rhs) const = default;
};

/**
 * @brief AST node for function definition.
 */
struct FunctionDeclNode {
  /**
   * @brief Prototype of the function.
   */
  PrototypeNode proto;

  /**
   * @brief Body of the function.
   */
  std::optional<StmtNode> func_body;

  /**
   * @brief Move constructor.
   */
  FunctionDeclNode(FunctionDeclNode &&) = default;

  /**
   * @brief Instantiate from prototype and function body.
   * @param proto Prototype of the function.
   * @param func_body Body of the function.
   */
  explicit FunctionDeclNode(PrototypeNode proto, StmtNode func_body);

  /**
   * @brief Constructor for extern functions.
   */
  explicit FunctionDeclNode(PrototypeNode proto);

  /**
   * @brief Move assignment operator.
   */
  FunctionDeclNode &operator=(FunctionDeclNode &&) = default;

  /**
   * @brief Comparision operator overload.
   * @param rhs ``FunctionDeclNode`` on the RHS.
   * @return Whether the objects referenced by ``this`` and ``rhs`` are equal.
   */
  bool operator==(const FunctionDeclNode &rhs) const = default;
};

/**
 * @brief Variant for declaration nodes.
 */
using DeclNode = std::variant<FunctionDeclNode>;

/**
 * @brief Module struct.
 */
struct Module {
  /**
   * @brief Name of the module.
   */
  std::string name;

  /**
   * @brief Declarations in the module.
   */
  std::vector<DeclNode> decls;

  /**
   * @brief Move constructor.
   */
  Module(Module &&) = default;

  /**
   * @brief Instantiate from name and declarations.
   * @param name Name of the module.
   * @param decls Declarations in the module.
   */
  explicit Module(const std::string &name, std::vector<DeclNode> decls);

  /**
   * @brief Move assignment operator.
   */
  Module &operator=(Module &&) = default;
};
} // namespace stapl::ast
