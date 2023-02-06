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
  Parser for stapl.
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

    ```ebnf
    intliteral = number ;
    literal = intliteral | floatliteral ;
    ```
   */
  ast::LiteralExprNode<int> parse_int();

  /**
    Parse floating-point literals.

    ```ebnf
    floatliteral = floatnumber ;
    literal = intliteral | floatliteral ;
    ```
   */
  ast::LiteralExprNode<double> parse_float();

  /**
    Parse expression and create an ``ExprNode``.

    ```ebnf
    expr = primary , binoprhs ;
    ```
   */
  ast::ExprNode parse_expr();

  /**
    Parse an expression inside a paren.

    ```ebnf
    parenexpr = "(" , expr , ")" ;
    ```
   */
  ast::ExprNode parse_paren_expr();

  /**
    Parse an identifier.

    ```ebnf
    identifier = identifier , "(" , [ expr ] , ")"
               | identifier , "(" , expr , { "," , expr } , ")" ;
    ```
   */
  ast::ExprNode parse_identifier();

  /**
    Parse a primary expression, which is one of identifier, literal and paren
    expression.

    ```ebnf
    primary = identifier | literal | parenexpr ;
    ```
   */
  ast::ExprNode parse_primary();

  /**
    Parse a binary operator and rhs.

    ```ebnf
    operator = "+" | "-" | "*" | "/" | "==" | "!=" | ">" | ">=" | "<" | "<=" ;
    binoprhs = { operator , primary }- ;
    ```
   */
  ast::ExprNode parse_binop_rhs(int prec, ast::ExprNode lhs);

  /**
    Parse an ``if`` expression.

    ```ebnf
    ifexpr = "if" | expr | "then" | expr | "else" | expr ;
    ```
   */
  ast::ExprNode parse_if();

  /**
    Parse a function prototype.

    ```ebnf
    typename = "int" | "float" ;
    proto
      = id , "(" , [ id , ":" , typename ] , ")" , ":" , typename
      | id , "(" , id , ":" , typename , { "," , id , ":" , typename } , ")" ,
        ":" , typename;
    ```
   */
  ast::PrototypeNode parse_proto();

  /**
    Parse a ``def`` statement.

    ```ebnf
    funcdef = "def" , proto , expr ;
    ```
   */
  ast::FunctionNode parse_def();

  /**
    Parse toplevel expression.

    ```ebnf
    toplevelexpr = expr ;
    ```
   */
  ast::FunctionNode parse_toplevel_expr();

  /**
    Parse an extern function.

    ```ebnf
    externfunc = "extern" , proto ;
    ```
   */
  ast::FunctionNode parse_extern();

  std::vector<ast::StatementNode> parse_all();
};
} // namespace stapl::parsing
