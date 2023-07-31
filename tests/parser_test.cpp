#include "ast.h"
#include "parser.h"

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

using namespace stapl::ast;
using namespace stapl::parsing;

TEST(ParserTest, Int) {
  Parser parser("42");
  auto node = parser.parse_int();
  EXPECT_EQ(node.value, 42);
}

TEST(ParserTest, Float) {
  std::string num = "2.71828";
  Parser parser(num);
  auto node = parser.parse_float();
  EXPECT_EQ(node.value, std::stod(num));
}

TEST(ParserTest, Bool) {
  Parser parser("true false");
  auto node = parser.parse_bool();
  EXPECT_TRUE(node.value);
  node = parser.parse_bool();
  EXPECT_FALSE(node.value);
}

TEST(ParserTest, Prototype) {
  Parser parser("func(a: int, b: float, c: int): int");
  PrototypeNode expected("func", {{"a", "int"}, {"b", "float"}, {"c", "int"}},
                         "int"),
      parsed = parser.parse_proto();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, Expr) {
  Parser parser("a*a + b*b - c*c + x%m + y/q");
  ExprNode
      expected = std::make_unique<BinaryExprNode>(
          "+",
          std::make_unique<BinaryExprNode>(
              "+",
              std::make_unique<BinaryExprNode>(
                  "-",
                  std::make_unique<BinaryExprNode>(
                      "+",
                      std::make_unique<BinaryExprNode>(
                          "*", VariableExprNode("a"), VariableExprNode("a")),
                      std::make_unique<BinaryExprNode>(
                          "*", VariableExprNode("b"), VariableExprNode("b"))),
                  std::make_unique<BinaryExprNode>("*", VariableExprNode("c"),
                                                   VariableExprNode("c"))),
              std::make_unique<BinaryExprNode>("%", VariableExprNode("x"),
                                               VariableExprNode("m"))),
          std::make_unique<BinaryExprNode>("/", VariableExprNode("y"),
                                           VariableExprNode("q"))),
      parsed = parser.parse_expr();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, CallExpr) {
  Parser parser("f(42, x + y, g(128))");
  std::vector<ExprNode> args_g;
  args_g.push_back(LiteralExprNode<int>(128));
  std::vector<ExprNode> args_f;
  args_f.push_back(LiteralExprNode<int>(42));
  args_f.push_back(std::make_unique<BinaryExprNode>("+", VariableExprNode("x"),
                                                    VariableExprNode("y")));
  args_f.push_back(std::make_unique<CallExprNode>("g", std::move(args_g)));
  ExprNode expected = std::make_unique<CallExprNode>("f", std::move(args_f)),
           parsed = parser.parse_expr();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, ParenExpr) {
  Parser parser("(a + b)*(a + b)");
  ExprNode expected = std::make_unique<BinaryExprNode>(
               "*",
               std::make_unique<BinaryExprNode>("+", VariableExprNode("a"),
                                                VariableExprNode("b")),
               std::make_unique<BinaryExprNode>("+", VariableExprNode("a"),
                                                VariableExprNode("b"))),
           parsed = parser.parse_expr();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, Let) {
  Parser parser("let x: int");
  StmtNode expected(LetStmtNode("x", "int")), parsed = parser.parse_stmt();
  EXPECT_EQ(parsed, expected);
}

TEST(ParserTest, Assign) {
  Parser parser("x = 1 + y");
  StmtNode expected(AssignmentStmtNode(
      "x", ExprNode(std::make_unique<BinaryExprNode>(
               "+", LiteralExprNode<int>(1), VariableExprNode("y"))))),
      parsed = parser.parse_stmt();
  EXPECT_EQ(parsed, expected);
}

TEST(ParserTest, If) {
  Parser parser(R"({
  if x == y {
    let z: int
    z = 42
  }
  let w: float
  w = f(x, y)
})");
  std::vector<ExprNode> call_args;
  call_args.push_back(VariableExprNode("x"));
  call_args.push_back(VariableExprNode("y"));
  std::vector<StmtNode> stmt_vec, then_stmt_vec, else_stmt_vec;
  then_stmt_vec.push_back(LetStmtNode("z", "int"));
  then_stmt_vec.push_back(AssignmentStmtNode("z", LiteralExprNode<int>(42)));
  stmt_vec.push_back(std::make_unique<IfStmtNode>(
      std::make_unique<BinaryExprNode>("==", VariableExprNode("x"),
                                       VariableExprNode("y")),
      std::make_unique<CompoundStmtNode>(std::move(then_stmt_vec)),
      std::make_unique<CompoundStmtNode>(std::vector<StmtNode>())));
  stmt_vec.push_back(LetStmtNode("w", "float"));
  stmt_vec.push_back(AssignmentStmtNode(
      "w", std::make_unique<CallExprNode>("f", std::move(call_args))));
  StmtNode expected(std::make_unique<CompoundStmtNode>(std::move(stmt_vec))),
      parsed = parser.parse_stmt();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, IfElse) {
  Parser parser(R"(if x == y {
  let z: int
  z = 42
} else {
  let w: float
  w = f(x, y)
})");
  std::vector<ExprNode> call_args;
  call_args.push_back(VariableExprNode("x"));
  call_args.push_back(VariableExprNode("y"));
  std::vector<StmtNode> then_stmt_vec, else_stmt_vec;
  then_stmt_vec.push_back(LetStmtNode("z", "int"));
  then_stmt_vec.push_back(AssignmentStmtNode("z", LiteralExprNode<int>(42)));
  else_stmt_vec.push_back(LetStmtNode("w", "float"));
  else_stmt_vec.push_back(AssignmentStmtNode(
      "w", std::make_unique<CallExprNode>("f", std::move(call_args))));
  StmtNode expected(std::make_unique<IfStmtNode>(
      std::make_unique<BinaryExprNode>("==", VariableExprNode("x"),
                                       VariableExprNode("y")),
      std::make_unique<CompoundStmtNode>(std::move(then_stmt_vec)),
      std::make_unique<CompoundStmtNode>(std::move(else_stmt_vec)))),
      parsed = parser.parse_stmt();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, IfElseifElse) {
  Parser parser(R"(if x == y {
  let z: int
  z = 42
} else if x > y {
  let w: float
  w = f(x, y)
} else {
  y = 0
})");
  std::vector<ExprNode> call_args;
  call_args.push_back(VariableExprNode("x"));
  call_args.push_back(VariableExprNode("y"));
  std::vector<StmtNode> then_stmt_vec, else_stmt_vec, else_if_stmt_vec,
      else_else_stmt_vec;
  then_stmt_vec.push_back(LetStmtNode("z", "int"));
  then_stmt_vec.push_back(AssignmentStmtNode("z", LiteralExprNode<int>(42)));
  else_if_stmt_vec.push_back(LetStmtNode("w", "float"));
  else_if_stmt_vec.push_back(AssignmentStmtNode(
      "w", std::make_unique<CallExprNode>("f", std::move(call_args))));
  else_else_stmt_vec.push_back(
      AssignmentStmtNode("y", LiteralExprNode<int>(0)));
  StmtNode expected(std::make_unique<IfStmtNode>(
      std::make_unique<BinaryExprNode>("==", VariableExprNode("x"),
                                       VariableExprNode("y")),
      std::make_unique<CompoundStmtNode>(std::move(then_stmt_vec)),
      std::make_unique<IfStmtNode>(
          std::make_unique<BinaryExprNode>(">", VariableExprNode("x"),
                                           VariableExprNode("y")),
          std::make_unique<CompoundStmtNode>(std::move(else_if_stmt_vec)),
          std::make_unique<CompoundStmtNode>(std::move(else_else_stmt_vec))))),
      parsed = parser.parse_stmt();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, Return) {
  Parser parser("return f(42) + 10 - x");
  std::vector<ExprNode> call_args;
  call_args.push_back(LiteralExprNode<int>(42));
  StmtNode expected(ReturnStmtNode(std::make_unique<BinaryExprNode>(
      "-",
      std::make_unique<BinaryExprNode>(
          "+", std::make_unique<CallExprNode>("f", std::move(call_args)),
          LiteralExprNode<int>(10)),
      VariableExprNode("x")))),
      parsed = parser.parse_stmt();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, Compound) {
  Parser parser(R"({
  let x: int
  x = 42
  let y: float
  y = f(x)
})");
  std::vector<StmtNode> stmts;
  stmts.push_back(LetStmtNode("x", "int"));
  stmts.push_back(AssignmentStmtNode("x", LiteralExprNode<int>(42)));
  stmts.push_back(LetStmtNode("y", "float"));
  std::vector<ExprNode> call_args;
  call_args.push_back(VariableExprNode("x"));
  stmts.push_back(AssignmentStmtNode(
      "y", std::make_unique<CallExprNode>("f", std::move(call_args))));
  StmtNode expected(std::make_unique<CompoundStmtNode>(std::move(stmts))),
      parsed = parser.parse_stmt();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, Extern) {
  Parser parser("extern f(x: int, y: float): void");
  DeclNode expected(FunctionDeclNode(
      PrototypeNode("f", {{"x", "int"}, {"y", "float"}}, "void"))),
      parsed = parser.parse_extern();
  EXPECT_EQ(expected, parsed);
}

TEST(ParserTest, Def) {
  Parser parser(R"(def f(x: int, y: float): void {
  let z: int
  z = g(y)
  if x >= z {
    h(x)
  } else {
    h(z)
  }
})");
  std::vector<ExprNode> g_call_args, h_call_args_then, h_call_args_else;
  h_call_args_then.push_back(VariableExprNode("x"));
  h_call_args_else.push_back(VariableExprNode("z"));
  g_call_args.push_back(VariableExprNode("y"));
  std::vector<StmtNode> stmts, then_stmts, else_stmts;
  stmts.push_back(LetStmtNode("z", "int"));
  stmts.push_back(AssignmentStmtNode(
      "z", std::make_unique<CallExprNode>("g", std::move(g_call_args))));
  then_stmts.push_back(AssignmentStmtNode(
      "_", std::make_unique<CallExprNode>("h", std::move(h_call_args_then))));
  else_stmts.push_back(AssignmentStmtNode(
      "_", std::make_unique<CallExprNode>("h", std::move(h_call_args_else))));
  stmts.push_back(std::make_unique<IfStmtNode>(
      std::make_unique<BinaryExprNode>(">=", VariableExprNode("x"),
                                       VariableExprNode("z")),
      std::make_unique<CompoundStmtNode>(std::move(then_stmts)),
      std::make_unique<CompoundStmtNode>(std::move(else_stmts))));
  DeclNode expected(FunctionDeclNode(
      PrototypeNode("f", {{"x", "int"}, {"y", "float"}}, "void"),
      std::make_unique<CompoundStmtNode>(std::move(stmts)))),
      parsed = parser.parse_def();
  EXPECT_EQ(expected, parsed);
}
