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
 *
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
  std::map<std::string, int> binop_prec = {
      {"<", 10}, {"<=", 10}, {">", 10}, {">=", 10}, {"==", 10},
      {"+", 20}, {"-", 20},  {"*", 40}, {"/", 40},  {"%", 40}};

  /**
   * @brief Get precedence of ``current_token``.
   * @return Precedence of ``current_token``.
   */
  int get_prec();

public:
  /**
   * @brief Constructor for parser.
   * @param code Code string to be parsed.
   */
  Parser(std::string code);

  /**
   * @brief Parse integer literals.
   * @return A parsed integer literal.
   */
  ast::LiteralExprNode<int> parse_int();

  /**
   * @brief Parse floating-point literals.
   * @return A parsed float literal.
   */
  ast::LiteralExprNode<double> parse_float();

  /**
   * @brief Parse boolean literals.
   * @return A parsed boolean literal.
   */
  ast::LiteralExprNode<bool> parse_bool();

  /**
   * @brief Parse expression and create an ``ExprNode``.
   * @return A parsed expression.
   */
  ast::ExprNode parse_expr();

  /**
   * @brief Parse an expression inside a paren.
   * @return A parsed expression in parens.
   */
  ast::ExprNode parse_paren_expr();

  /**
   * @brief Parse primary, which is one of identifier, literal and paren
   * expression.
   * @return A parsed primary node.
   */
  ast::ExprNode parse_primary();

  /**
   * @brief Parse a binary operator and rhs.
   * @param prec Precedence of the operator.
   * @param lhs LHS of the operator.
   * @return A parsed binary expression node.
   */
  ast::ExprNode parse_binop_rhs(int prec, ast::ExprNode lhs);

  /**
   * @brief Parse argument list in function call.
   * @return A parsed argument list.
   */
  std::vector<ast::ExprNode> parse_call_arg_list();

  /**
   * @brief Parse identifier or a function call.
   * @return A parsed expression, which is either an identifier or a function
   * call.
   */
  ast::ExprNode parse_identifier_or_func_call();

  /**
   * @brief Parse a statement.
   * @return A parsed statement.
   */
  ast::StmtNode parse_stmt();

  /**
   * @brief Parse a ``let`` statement.
   * @return A parsed ``let`` statement.
   */
  ast::StmtNode parse_let();

  /**
   * @brief Parse an assignment statement or function call.
   * @return A parsed statement node of assignment statement or function call.
   */
  ast::StmtNode parse_assign_or_call();

  /**
   * @brief Parse an ``if`` statement.
   * @return A parsed ``if`` statement.
   */
  ast::StmtNode parse_if();

  /**
   * @brief Parse a ``return`` statement.
   * @return A parsed ``return`` statement.
   */
  ast::StmtNode parse_return();

  /**
   * @brief Parse a compound statement.
   * @return A parsed compound statement.
   */
  ast::StmtNode parse_compound();

  /**
   * @brief Parse a function prototype.
   * @return A parsed function prototype.
   */
  ast::PrototypeNode parse_proto();

  /**
   * @brief Parse a ``def`` statement.
   * @return A parsed ``def`` statement.
   */
  ast::FunctionDeclNode parse_def();

  /**
   * @brief Parse an extern function.
   * @return A parsed extern function.
   */
  ast::FunctionDeclNode parse_extern();

  /**
   * @brief Parse all declarations.
   * @return A vector of parsed declarations.
   */
  std::vector<ast::DeclNode> parse_all();

  /**
   * @brief Parse a module.
   * @return A parsed ``Module`` object containing all declarations.
   */
  ast::Module parse_module();
};
} // namespace stapl::parsing
