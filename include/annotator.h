#pragma once

#include "ast.h"
#include "types.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Classes and types related to type checking.
 */
namespace stapl::types {
/**
 * @brief A visitor for annotating AST nodes with types.
 * @todo Add support for block scopes.
 */
class TypeAnnotator {
private:
  /**
   * @brief Type names of variables in current function.
   * @todo Add support for generics and user-defined types.
   */
  std::unordered_map<std::string, std::string> variable_type_names = {};

  /**
   * @brief Type information of functions.
   */
  std::unordered_map<std::string, std::vector<FuncTypeInfo>> func_types = {};

public:
  /**
   * @brief Default constructor.
   */
  TypeAnnotator();

  /**
   * @brief Annotate the type of a integer literal expression node and return
   * the annotated type name.
   * @param node The node to annotate.
   * @return The annotated type of the node.
   */
  std::string operator()(ast::LiteralExprNode<int> &node);

  /**
   * @brief Annotate the type of a float literal expression node and return
   * the annotated type name.
   * @param node The node to annotate.
   * @return The annotated type of the node.
   */
  std::string operator()(ast::LiteralExprNode<double> &node);

  /**
   * @brief Annotate the type of a boolean literal expression node and return
   * the annotated type name.
   * @param node The node to annotate.
   * @return The annotated type of the node.
   */
  std::string operator()(ast::LiteralExprNode<bool> &node);

  /**
   * @brief Annotate the type of a variable expression node and return the
   * annotated type name.
   * @param node The node to annotate.
   * @return The annotated type of the node.
   */
  std::string operator()(ast::VariableExprNode &node);

  /**
   * @brief Annotate the type of a binary expression node and return the
   * annotated type name.
   * @param node The node to annotate.
   * @return The annotated type of the node.
   */
  std::string operator()(std::unique_ptr<ast::BinaryExprNode> &node);

  /**
   * @brief Annotate the type of a call expression node and return the annotated
   * type name.
   * @param node The node to annotate.
   * @return The annotated type of the node.
   */
  std::string operator()(std::unique_ptr<ast::CallExprNode> &node);

  /**
   * @brief Register the type of the variable declared by ``let`` statement.
   * @param node The node to annotate.
   */
  void operator()(ast::LetStmtNode &node);

  /**
   * @brief Annotate the type of a variable assignment statement node.
   * @param node The node to annotate.
   */
  void operator()(ast::AssignmentStmtNode &node);

  /**
   * @brief Annotate the type of a if statement node.
   * @param node The node to annotate.
   */
  void operator()(std::unique_ptr<ast::IfStmtNode> &node);

  /**
   * @brief Annotate the type of a return statement node.
   * @param node The node to annotate.
   */
  void operator()(ast::ReturnStmtNode &node);

  /**
   * @brief Annotate the type of a compound statement node.
   * @param node The node to annotate.
   */
  void operator()(std::unique_ptr<ast::CompoundStmtNode> &node);

  /**
   * @brief Annotate the type of a function declaration node.
   * @param node The node to annotate.
   */
  void operator()(ast::FunctionDeclNode &node);
};
}; // namespace stapl::types
