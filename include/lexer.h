#pragma once

#include <istream>
#include <map>
#include <set>
#include <streambuf>
#include <string>
#include <utility>

namespace stapl::parsing {
/**
  Tokens of stapl.
 */
enum class TokenKind {
  /**
    Token kind for EOF.
   */
  Eof,

  /**
    Token kind for "def" keyword.
   */
  Def,

  /**
    Token kind for "extern" keyword.
   */
  Extern,

  /**
    Token kind for "if" keyword.
   */
  If,

  /**
    Token kind for "else" keyword.
   */
  Else,

  /**
    Token kind for identifier.
   */
  Identifier,

  /**
    Token kind for int literal.
   */
  Int,

  /**
    Token kind for float literal.
   */
  Float,

  /**
    Token kind for boolean literal.
   */
  Bool,

  /**
    Token kind for "let" keyword.
   */
  Let,

  /**
    Token kind for "return" keyword.
   */
  Return,

  /**
    Token kind for operator.
   */
  Op,

  /**
    Token kind for various punctuations.
   */
  Misc,
};

/**
  Lexed token object.
 */
typedef std::pair<TokenKind, std::string> Token;

/**
  Lexer for stapl.
 */
class Lexer {
private:
  /**
    Code string that is currently lexed.
   */
  std::string code;

  /**
    Identifier that is currently lexed.
   */
  std::string identifier;

  /**
    Operator that is currently lexed.
   */
  std::string op;

  /**
    Iterator of currently lexed code.
   */
  std::string::iterator it;

  /**
    DFA table for parsing operators.
   */
  std::map<int, std::set<int>> operator_dfa;

  /**
    Mapping for recognizing keywords.
   */
  std::map<std::string, TokenKind> token_table;

  /**
    Last character seen.
   */
  int last_char;

public:
  /**
    Constructor for lexer.
   */
  Lexer(std::string code);

  /**
    Return a single lexed token.
   */
  std::pair<TokenKind, std::string> get_token();
};
} // namespace stapl::parsing
