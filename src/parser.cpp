#include <parser.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include <fmt/core.h>

Parser::Parser(std::string code) : lexer(code) {
  binop_prec["<"] = 10;
  binop_prec["<="] = 10;
  binop_prec[">"] = 10;
  binop_prec[">="] = 10;
  binop_prec["=="] = 10;
  binop_prec["+"] = 20;
  binop_prec["-"] = 20;
  binop_prec["*"] = 40;
  binop_prec["/"] = 40;
  next_token();
}

int Parser::get_prec() {
  if (binop_prec.count(current_token.second) == 0)
    return -1;
  int prec = binop_prec[current_token.second];
  return prec;
}

Token Parser::next_token() { return current_token = lexer.get_token(); }

LiteralExprNode<int> Parser::parse_int() {
  LiteralExprNode<int> node(std::stoi(current_token.second));
  next_token();
  return node;
}

LiteralExprNode<double> Parser::parse_float() {
  LiteralExprNode<double> node(std::stod(current_token.second));
  next_token();
  return node;
}

ExprNode Parser::parse_paren_expr() {
  next_token();
  auto node = parse_expr();
  if (current_token.second != ")")
    throw std::logic_error("expected )");
  next_token();
  return node;
}

ExprNode Parser::parse_identifier() {
  std::string identifier = current_token.second;
  next_token();
  if (current_token.second != "(")
    return VariableExprNode(identifier);

  next_token();
  std::vector<ExprNode> args;
  if (current_token.second != ")") {
    while (true) {
      auto arg = parse_expr();
      args.push_back(std::move(arg));

      if (current_token.second == ")")
        break;
      if (current_token.second != ",")
        throw std::logic_error("expected ) or , in arg list");
      next_token();
    }
  }
  next_token();
  return std::make_unique<CallExprNode>(identifier, std::move(args));
}

ExprNode Parser::parse_primary() {
  switch (current_token.first) {
  case TokenKind::kIdentifier:
    return parse_identifier();
  case TokenKind::kInt:
    return parse_int();
  case TokenKind::kFloat:
    return parse_float();
  case TokenKind::kMisc:
    if (current_token.second == "(")
      return parse_paren_expr();
  case TokenKind::kIf:
    return parse_if();
  default:
    throw std::logic_error("unknown token");
  }
}

ExprNode Parser::parse_binop_rhs(int expr_prec, ExprNode lhs) {
  while (true) {
    int token_prec = get_prec();
    if (token_prec < expr_prec)
      return lhs;

    auto op = current_token;
    next_token();

    auto rhs = std::move(parse_primary());
    int next_prec = get_prec();
    if (token_prec < next_prec)
      rhs = parse_binop_rhs(token_prec + 1, std::move(rhs));
    lhs = std::make_unique<BinaryExprNode>(op.second, std::move(lhs),
                                           std::move(rhs));
  }
}

ExprNode Parser::parse_if() {
  next_token();
  auto condition = parse_expr();
  if (current_token.first != TokenKind::kThen)
    throw std::logic_error(
        fmt::format("expected then, got: {}", current_token.second));
  next_token();
  auto then_expr = parse_expr();
  if (current_token.first != TokenKind::kElse)
    throw std::logic_error(
        fmt::format("expected else, got: {}", current_token.second));
  next_token();
  auto else_expr = parse_expr();
  return std::make_unique<IfExprNode>(
      std::move(condition), std::move(then_expr), std::move(else_expr));
}

ExprNode Parser::parse_expr() {
  auto lhs = parse_primary();
  return parse_binop_rhs(0, std::move(lhs));
}

PrototypeNode Parser::parse_proto() {
  if (current_token.first != TokenKind::kIdentifier)
    throw std::logic_error("expected function name");

  std::string func_name = current_token.second;
  next_token();

  if (current_token.second != "(")
    throw std::logic_error("expected ( in prototype");

  std::vector<std::pair<std::string, std::string>> arg_names;
  while (next_token().first == TokenKind::kIdentifier) {
    auto var_name = current_token.second;
    if (next_token().second != ":")
      throw std::logic_error("expected : after arg name");
    auto type_name = next_token().second;
    arg_names.push_back({var_name, type_name});
    if (next_token().second != ",")
      break;
  }
  if (current_token.second != ")")
    throw std::logic_error("expected ) in prototype");
  if (next_token().second != ":")
    throw std::logic_error("expected : after args");
  auto return_type = next_token().second;
  next_token();
  return PrototypeNode(func_name, std::move(arg_names), return_type);
}

FunctionNode Parser::parse_def() {
  next_token();
  auto proto = parse_proto();
  auto expr = parse_expr();
  return FunctionNode(std::move(proto), std::move(expr));
}

FunctionNode Parser::parse_toplevel_expr() {
  auto expr = parse_expr();
  auto proto =
      PrototypeNode("__anon_expr",
                    std::vector<std::pair<std::string, std::string>>(), "void");
  return FunctionNode(std::move(proto), std::move(expr));
}

FunctionNode Parser::parse_extern() {
  next_token();
  auto proto = parse_proto();
  auto func = FunctionNode(std::move(proto));
  return func;
}

std::vector<StatementNode> Parser::parse_all() {
  std::vector<StatementNode> ast;
  while (true) {
    if (current_token.first == TokenKind::kEof)
      return ast;
    else if (current_token.second == ";")
      next_token();
    else if (current_token.first == TokenKind::kDef)
      ast.push_back(std::move(parse_def()));
    else if (current_token.first == TokenKind::kExtern)
      ast.push_back(std::move(parse_extern()));
    else
      ast.push_back(std::move(parse_toplevel_expr()));
  }
}
