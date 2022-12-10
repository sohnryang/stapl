#pragma once

#include <istream>
#include <map>
#include <set>
#include <streambuf>
#include <string>
#include <utility>

enum class TokenKind {
  kEof,
  kDef,
  kExtern,
  kIf,
  kThen,
  kElse,
  kFor,
  kIdentifier,
  kInt,
  kFloat,
  kOp,
  kMisc,
};
typedef std::pair<TokenKind, std::string> Token;

class Lexer {
private:
  std::string identifier, op, code;
  std::string::iterator it;
  std::map<int, std::set<int>> operator_dfa;
  int last_char;

public:
  Lexer(std::string code);
  std::pair<TokenKind, std::string> get_token();
};
