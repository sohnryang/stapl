#include "parser.h"

#include <stdexcept>
#include <vector>

#include <fmt/core.h>

namespace stapl::parsing {
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

ast::LiteralExprNode<int> Parser::parse_int() {
  ast::LiteralExprNode<int> node(std::stoi(current_token.second));
  next_token();
  return node;
}

ast::LiteralExprNode<double> Parser::parse_float() {
  ast::LiteralExprNode<double> node(std::stod(current_token.second));
  next_token();
  return node;
}

ast::ExprNode Parser::parse_expr() {
  auto lhs = parse_primary();
  return parse_binop_rhs(0, std::move(lhs));
}

ast::ExprNode Parser::parse_paren_expr() {
  next_token();
  auto node = parse_expr();
  if (current_token.second != ")")
    throw std::logic_error("expected )");
  next_token();
  return node;
}

ast::ExprNode Parser::parse_primary() {
  switch (current_token.first) {
  case TokenKind::Identifier:
    return parse_identifier_or_func_call();
  case TokenKind::Int:
    return parse_int();
  case TokenKind::Float:
    return parse_float();
  case TokenKind::Misc:
    if (current_token.second == "(")
      return parse_paren_expr();
  default:
    throw std::logic_error("unknown token");
  }
}

ast::ExprNode Parser::parse_binop_rhs(int expr_prec, ast::ExprNode lhs) {
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
    lhs = std::make_unique<ast::BinaryExprNode>(op.second, std::move(lhs),
                                                std::move(rhs));
  }
}

std::vector<ast::ExprNode> Parser::parse_call_arg_list() {
  std::vector<ast::ExprNode> args;
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
  return args;
}

ast::ExprNode Parser::parse_identifier_or_func_call() {
  std::string identifier = current_token.second;
  next_token();
  if (current_token.second != "(")
    return ast::VariableExprNode(identifier);

  next_token();
  return std::make_unique<ast::CallExprNode>(identifier,
                                             std::move(parse_call_arg_list()));
}

ast::StmtNode Parser::parse_stmt() {
  switch (current_token.first) {
  case TokenKind::Let:
    return parse_let();
  case TokenKind::Identifier:
    return parse_assign_or_call();
  case TokenKind::If:
    return parse_if();
  case TokenKind::Return:
    return parse_return();
  case TokenKind::Misc:
    return parse_compound();
  default:
    throw std::logic_error("unknown token");
  }
}

ast::StmtNode Parser::parse_let() {
  next_token();
  auto var_name = current_token.second;
  next_token();
  next_token();
  auto type_name = current_token.second;
  next_token();
  return ast::LetStmtNode(var_name, type_name);
}

ast::StmtNode Parser::parse_assign_or_call() {
  auto var_name = current_token.second;
  next_token();
  if (current_token.second == "=") {
    next_token();
    auto expr = parse_expr();
    return ast::AssignmentStmtNode(var_name, std::move(expr));
  } else if (current_token.second == "(") {
    next_token();
    return ast::AssignmentStmtNode(
        "_", std::make_unique<ast::CallExprNode>(
                 var_name, std::move(parse_call_arg_list())));
  }
  throw std::logic_error("expected assignment or function call");
}

ast::StmtNode Parser::parse_if() {
  next_token();
  auto condition = parse_expr();
  auto then_stmt = parse_compound();
  if (current_token.first != TokenKind::Else)
    throw std::logic_error(
        fmt::format("expected else, got: {}", current_token.second));
  next_token();
  auto else_stmt = parse_compound();
  return std::make_unique<ast::IfStmtNode>(
      std::move(condition), std::move(then_stmt), std::move(else_stmt));
}

ast::StmtNode Parser::parse_return() {
  next_token();
  auto expr = parse_expr();
  return ast::ReturnStmtNode(std::move(expr));
}

ast::StmtNode Parser::parse_compound() {
  next_token();
  std::vector<ast::StmtNode> stmts;
  while (current_token.second != "}") {
    stmts.push_back(parse_stmt());
  }
  next_token();
  return std::make_unique<ast::CompoundStmtNode>(std::move(stmts));
}

ast::PrototypeNode Parser::parse_proto() {
  if (current_token.first != TokenKind::Identifier)
    throw std::logic_error("expected function name");

  std::string func_name = current_token.second;
  next_token();

  if (current_token.second != "(")
    throw std::logic_error("expected ( in prototype");

  std::vector<std::pair<std::string, std::string>> arg_names;
  while (next_token().first == TokenKind::Identifier) {
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
  return ast::PrototypeNode(func_name, std::move(arg_names), return_type);
}

ast::FunctionDeclNode Parser::parse_def() {
  next_token();
  auto proto = parse_proto();
  auto stmt = parse_compound();
  return ast::FunctionDeclNode(std::move(proto), std::move(stmt));
}

ast::FunctionDeclNode Parser::parse_extern() {
  next_token();
  auto proto = parse_proto();
  auto func = ast::FunctionDeclNode(std::move(proto));
  return func;
}

std::vector<ast::DeclNode> Parser::parse_all() {
  std::vector<ast::DeclNode> decls;
  while (true) {
    if (current_token.first == TokenKind::Eof)
      return decls;
    else if (current_token.first == TokenKind::Def)
      decls.push_back(std::move(parse_def()));
    else if (current_token.first == TokenKind::Extern)
      decls.push_back(std::move(parse_extern()));
    else
      throw std::logic_error("expected declaration");
  }
}
} // namespace stapl::parsing
