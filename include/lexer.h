#pragma once

#include <istream>
#include <map>
#include <set>
#include <streambuf>
#include <string>
#include <utility>

namespace stapl::parsing {
/**
 * @brief Tokens of stapl.
 */
enum class TokenKind {
  /**
   * @brief Token kind for EOF.
   */
  Eof,

  /**
   * @brief Token kind for "def" keyword.
   */
  Def,

  /**
   * @brief Token kind for "extern" keyword.
   */
  Extern,

  /**
   * @brief Token kind for "if" keyword.
   */
  If,

  /**
   * @brief Token kind for "else" keyword.
   */
  Else,

  /**
   * @brief Token kind for identifier.
   */
  Identifier,

  /**
   * @brief Token kind for int literal.
   */
  Int,

  /**
   * @brief Token kind for float literal.
   */
  Float,

  /**
   * @brief Token kind for boolean literal.
   */
  Bool,

  /**
   * @brief Token kind for "let" keyword.
   */
  Let,

  /**
   * @brief Token kind for "return" keyword.
   */
  Return,

  /**
   * @brief Token kind for "module" keyword.
   */
  Module,

  /**
   * @brief Token kind for operator.
   */
  Op,

  /**
   * @brief Token kind for various punctuations.
   */
  Misc,
};

/**
 * @brief Lexed token object.
 */
typedef std::pair<TokenKind, std::string> Token;

/**
 * @brief Lexer for stapl.
 */
class Lexer {
private:
  /**
   * @brief Code string that is currently lexed.
   */
  std::string code;

  /**
   * @brief Identifier that is currently lexed.
   */
  std::string identifier;

  /**
   * @brief Operator that is currently lexed.
   */
  std::string op;

  /**
   * @brief Iterator of currently lexed code.
   */
  std::string::iterator it;

  /**
   * @brief DFA table for parsing operators.
   */
  std::map<int, std::set<int>> operator_dfa;

  /**
   * @brief Mapping for recognizing keywords.
   */
  std::map<std::string, TokenKind> token_table;

  /**
   * @brief Last character seen.
   */
  int last_char;

public:
  /**
   * @brief Constructor for lexer.
   */
  Lexer(std::string code);

  /**
   * @brief Return a single lexed token.
   */
  std::pair<TokenKind, std::string> get_token();
};
} // namespace stapl::parsing
