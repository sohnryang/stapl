#pragma once

#include <ast.h>
#include <lexer.h>

#include <map>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

namespace stapl::parsing {
/**
  Parser for stapl. This is a combination of [Recursive Descent
  Parsing](https://en.wikipedia.org/wiki/Recursive_descent_parser) and
  [Operator-Precedence
  Parsing](https://en.wikipedia.org/wiki/Operator-precedence_parser).
 */
class Parser {
private:
  Lexer lexer;
  Token current_token;
  Token next_token();
  std::map<std::string, int> binop_prec;
  int get_prec();

public:
  Parser(std::string code);

  /**
    Parse integer literals.
   */
  ast::LiteralExprNode<int> parse_int();

  /**
    Parse floating-point literals.
   */
  ast::LiteralExprNode<double> parse_float();

  /**
    Parse expression and create an ``ExprNode``.
   */
  ast::ExprNode parse_expr();

  /**
    Parse an expression inside a paren.
   */
  ast::ExprNode parse_paren_expr();

  /**
    Parse an identifier.
   */
  ast::ExprNode parse_identifier();

  /**
    Parse a primary expression, which is one of identifier, literal and paren
    expression.
   */
  ast::ExprNode parse_primary();

  /**
    Parse a binary operator and rhs.
   */
  ast::ExprNode parse_binop_rhs(int prec, ast::ExprNode lhs);

  /**
    Parse an ``if`` expression.
   */
  ast::ExprNode parse_if();

  /**
    Parse a function prototype.
   */
  ast::PrototypeNode parse_proto();

  /**
    Parse a ``def`` statement.
   */
  ast::FunctionNode parse_def();

  /**
    Parse toplevel expression.
   */
  ast::FunctionNode parse_toplevel_expr();

  /**
    Parse an extern function.
   */
  ast::FunctionNode parse_extern();

  std::vector<ast::StatementNode> parse_all();
};
} // namespace stapl::parsing
