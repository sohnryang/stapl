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
    Parse primary, which is one of identifier, literal and paren expression.
   */
  ast::ExprNode parse_primary();

  /**
    Parse a binary operator and rhs.
   */
  ast::ExprNode parse_binop_rhs(int prec, ast::ExprNode lhs);

  /**
    Parse identifier or a function call.
   */
  ast::ExprNode parse_identifier_or_func_call();

  /**
    Parse a statement.
   */
  ast::StmtNode parse_stmt();

  /**
    Parse a ``let`` statement.
   */
  ast::StmtNode parse_let();

  /**
    Parse an assignment statement.
   */
  ast::StmtNode parse_assign();

  /**
    Parse an ``if`` statement.
   */
  ast::StmtNode parse_if();

  /**
    Parse a ``return`` statement.
   */
  ast::StmtNode parse_return();

  /**
    Parse a compound statement.
   */
  ast::StmtNode parse_compound();

  /**
    Parse a function prototype.
   */
  ast::PrototypeNode parse_proto();

  /**
    Parse a ``def`` statement.
   */
  ast::FunctionDeclNode parse_def();

  /**
    Parse an extern function.
   */
  ast::FunctionDeclNode parse_extern();

  /**
    Parse all declarations.
   */
  std::vector<ast::DeclNode> parse_all();
};
} // namespace stapl::parsing
