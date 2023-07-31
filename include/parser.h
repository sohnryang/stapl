#pragma once

#include "ast.h"
#include "lexer.h"

#include <map>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Classes and types related to parsing.
 */
namespace stapl::parsing {
/**
 * @brief Parser for stapl.
 * This is a combination of [Recursive Descent
 * Parsing](https://en.wikipedia.org/wiki/Recursive_descent_parser) and
 * [Operator-Precedence
 * Parsing](https://en.wikipedia.org/wiki/Operator-precedence_parser).
 */
class Parser {
private:
  /**
   * @brief ``Lexer`` object for getting tokens.
   */
  Lexer lexer;

  /**
   * @brief Currently parsing token.
   */
  Token current_token;

  /**
   * @brief Get token from lexer and update ``current_token``.
   */
  Token next_token();

  /**
   * @brief Precedence table for binary operators.
   */
  std::map<std::string, int> binop_prec;

  /**
   * @brief Get precedence of ``current_token``.
   */
  int get_prec();

public:
  Parser(std::string code);

  /**
   * @brief Parse integer literals.
   */
  ast::LiteralExprNode<int> parse_int();

  /**
   * @brief Parse floating-point literals.
   */
  ast::LiteralExprNode<double> parse_float();

  /**
   * @brief Parse expression and create an ``ExprNode``.
   */
  ast::ExprNode parse_expr();

  /**
   * @brief Parse an expression inside a paren.
   */
  ast::ExprNode parse_paren_expr();

  /**
   * @brief Parse primary, which is one of identifier, literal and paren
   * expression.
   */
  ast::ExprNode parse_primary();

  /**
   * @brief Parse a binary operator and rhs.
   */
  ast::ExprNode parse_binop_rhs(int prec, ast::ExprNode lhs);

  /**
   * @brief Parse argument list in function call.
   */
  std::vector<ast::ExprNode> parse_call_arg_list();

  /**
   * @brief Parse identifier or a function call.
   */
  ast::ExprNode parse_identifier_or_func_call();

  /**
   * @brief Parse a statement.
   */
  ast::StmtNode parse_stmt();

  /**
   * @brief Parse a ``let`` statement.
   */
  ast::StmtNode parse_let();

  /**
   * @brief Parse an assignment statement.
   */
  ast::StmtNode parse_assign_or_call();

  /**
   * @brief Parse an ``if`` statement.
   */
  ast::StmtNode parse_if();

  /**
   * @brief Parse a ``return`` statement.
   */
  ast::StmtNode parse_return();

  /**
   * @brief Parse a compound statement.
   */
  ast::StmtNode parse_compound();

  /**
   * @brief Parse a function prototype.
   */
  ast::PrototypeNode parse_proto();

  /**
   * @brief Parse a ``def`` statement.
   */
  ast::FunctionDeclNode parse_def();

  /**
   * @brief Parse an extern function.
   */
  ast::FunctionDeclNode parse_extern();

  /**
   * @brief Parse all declarations.
   */
  std::vector<ast::DeclNode> parse_all();

  /**
   * @brief Parse a module.
   */
  ast::Module parse_module();
};
} // namespace stapl::parsing
