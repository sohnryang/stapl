#pragma once

#include "ast.h"

#include <memory>
#include <string>

namespace stapl::ast {
/**
 * @brief A visitor for printing AST nodes.
 */
class ASTPrinter {
public:
  /**
   * @brief Represent ``LiteralExprNode<int>`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const LiteralExprNode<int> &node) const;

  /**
   * @brief Represent ``LiteralExprNode<double>`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const LiteralExprNode<double> &node) const;

  /**
   * @brief Represent ``LiteralExprNode<bool>`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const LiteralExprNode<bool> &node) const;

  /**
   * @brief Represent ``VariableExprNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const VariableExprNode &node) const;

  /**
   * @brief Represent ``BinaryExprNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const std::unique_ptr<BinaryExprNode> &node) const;

  /**
   * @brief Represent ``CallExprNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const std::unique_ptr<CallExprNode> &node) const;

  /**
   * @brief Represent ``PrototypeNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const PrototypeNode &node) const;

  /**
   * @brief Represent ``LetStmtNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const LetStmtNode &node) const;

  /**
   * @brief Represent ``AssignmentStmtNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const AssignmentStmtNode &node) const;

  /**
   * @brief Represent ``IfStmtNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const std::unique_ptr<IfStmtNode> &node) const;

  /**
   * @brief Represent ``ReturnStmtNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const ReturnStmtNode &node) const;

  /**
   * @brief Represent ``CompoundStmtNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const std::unique_ptr<CompoundStmtNode> &node) const;

  /**
   * @brief Represent ``FunctionDeclNode`` as string.
   * @param node The node to represent.
   * @return The string representation of the node.
   */
  std::string operator()(const FunctionDeclNode &node) const;
};
} // namespace stapl::ast
