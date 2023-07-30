#include "lexer.h"

#include <cctype>

namespace stapl::parsing {
Lexer::Lexer(std::string code)
    : code(code), last_char(' '), operator_dfa({{'<', {'\0', '='}},
                                                {'>', {'\0', '='}},
                                                {'=', {'\0', '='}},
                                                {'+', {'\0'}},
                                                {'-', {'\0'}},
                                                {'*', {'\0'}},
                                                {'/', {'\0'}},
                                                {'%', {'\0'}}}),
      token_table({{"def", TokenKind::Def},
                   {"extern", TokenKind::Extern},
                   {"if", TokenKind::If},
                   {"else", TokenKind::Else},
                   {"let", TokenKind::Let},
                   {"return", TokenKind::Return},
                   {"true", TokenKind::Bool},
                   {"false", TokenKind::Bool},
                   {"module", TokenKind::Module}}) {
  it = this->code.begin();
}

Token Lexer::get_token() {
  while (std::isspace(last_char))
    last_char = *(it++);
  if (std::isalpha(last_char)) {
    identifier = last_char;
    while (std::isalnum(last_char = *(it++)) || last_char == '_')
      identifier += last_char;
    auto kind = TokenKind::Identifier;
    if (token_table.contains(identifier))
      kind = token_table[identifier];
    return {kind, identifier};
  }

  if (std::isdigit(last_char) || last_char == '.') {
    std::string num_str;
    do {
      num_str += last_char;
      last_char = *(it++);
    } while (std::isdigit(last_char) || last_char == '.');
    if (num_str.find('.') != std::string::npos)
      return {TokenKind::Float, num_str};
    return {TokenKind::Int, num_str};
  }

  if (last_char == '#') {
    do
      last_char = *(it++);
    while (last_char != '\0' && last_char != '\n' && last_char != '\r');
    if (last_char != '\0')
      return get_token();
  }

  if (last_char == '\0')
    return {TokenKind::Eof, ""};

  int this_char = last_char;
  last_char = *(it++);
  if (!operator_dfa.count(this_char))
    return {TokenKind::Misc, std::string(1, this_char)};
  op = this_char;
  while (operator_dfa[op.back()].contains(last_char)) {
    op += last_char;
    last_char = *(it++);
  }
  return {TokenKind::Op, op};
}
} // namespace stapl::parsing
