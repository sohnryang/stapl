#pragma once

#include <istream>
#include <map>
#include <set>
#include <streambuf>
#include <string>
#include <utility>

namespace stapl::parsing {
enum class TokenKind {
  kEof,
  kDef,
  kExtern,
  kIf,
  kElse,
  kIdentifier,
  kInt,
  kFloat,
  kBool,
  kLet,
  kReturn,
  kOp,
  kMisc,
};
typedef std::pair<TokenKind, std::string> Token;

class Lexer {
private:
  std::string identifier, op, code;
  std::string::iterator it;
  std::map<int, std::set<int>> operator_dfa;
  std::map<std::string, TokenKind> token_table;
  int last_char;

public:
  Lexer(std::string code);
  std::pair<TokenKind, std::string> get_token();
};
} // namespace stapl::parsing
