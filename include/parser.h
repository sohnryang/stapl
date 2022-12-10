#pragma once

#include <ast.h>
#include <lexer.h>

#include <map>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

class Parser {
private:
  Lexer lexer;
  Token current_token;
  Token next_token();
  std::map<std::string, int> binop_prec;
  int get_prec();

public:
  Parser(std::string code);
  LiteralExprNode<int> parse_int();
  LiteralExprNode<double> parse_float();
  ExprNode parse_expr();
  ExprNode parse_paren_expr();
  ExprNode parse_identifier();
  ExprNode parse_primary();
  ExprNode parse_binop_rhs(int prec, ExprNode lhs);
  ExprNode parse_if();
  PrototypeNode parse_proto();
  FunctionNode parse_def();
  FunctionNode parse_toplevel_expr();
  FunctionNode parse_extern();

  std::vector<StatementNode> parse_all();
};
