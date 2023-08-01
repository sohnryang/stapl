#include "ast.h"
#include "parser.h"
#include "util.h"

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

using namespace stapl::ast;
using namespace stapl::parsing;
using namespace stapl::util;

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
  auto args = make_vector<ExprNode>(
      LiteralExprNode<int>(42),
      std::make_unique<BinaryExprNode>("+", VariableExprNode("x"),
                                       VariableExprNode("y")),
      std::make_unique<CallExprNode>(
          "g", make_vector<ExprNode>(LiteralExprNode<int>(128))));
  ExprNode expected = std::make_unique<CallExprNode>("f", std::move(args)),
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
  StmtNode expected(std::make_unique<CompoundStmtNode>(make_vector<StmtNode>(
      std::make_unique<IfStmtNode>(
          std::make_unique<BinaryExprNode>("==", VariableExprNode("x"),
                                           VariableExprNode("y")),
          std::make_unique<CompoundStmtNode>(make_vector<StmtNode>(
              LetStmtNode("z", "int"),
              AssignmentStmtNode("z", LiteralExprNode<int>(42)))),
          std::make_unique<CompoundStmtNode>(std::vector<StmtNode>())),
      LetStmtNode("w", "float"),
      AssignmentStmtNode(
          "w", std::make_unique<CallExprNode>(
                   "f", make_vector<ExprNode>(VariableExprNode("x"),
                                              VariableExprNode("y"))))))),
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
  auto call_args =
      make_vector<ExprNode>(VariableExprNode("x"), VariableExprNode("y"));
  auto then_stmt_vec = make_vector<StmtNode>(
           LetStmtNode("z", "int"),
           AssignmentStmtNode("z", LiteralExprNode<int>(42))),
       else_stmt_vec = make_vector<StmtNode>(
           LetStmtNode("w", "float"),
           AssignmentStmtNode(
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
  auto call_args =
      make_vector<ExprNode>(VariableExprNode("x"), VariableExprNode("y"));
  auto then_stmt_vec = make_vector<StmtNode>(
           LetStmtNode("z", "int"),
           AssignmentStmtNode("z", LiteralExprNode<int>(42))),
       else_if_stmt_vec = make_vector<StmtNode>(
           LetStmtNode("w", "float"),
           AssignmentStmtNode(
               "w", std::make_unique<CallExprNode>("f", std::move(call_args)))),
       else_else_stmt_vec = make_vector<StmtNode>(
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
  StmtNode expected(ReturnStmtNode(std::make_unique<BinaryExprNode>(
      "-",
      std::make_unique<BinaryExprNode>(
          "+",
          std::make_unique<CallExprNode>(
              "f", make_vector<ExprNode>(LiteralExprNode<int>(42))),
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
  auto stmts = make_vector<StmtNode>(
      LetStmtNode("x", "int"),
      AssignmentStmtNode("x", LiteralExprNode<int>(42)),
      LetStmtNode("y", "float"),
      AssignmentStmtNode(
          "y", std::make_unique<CallExprNode>(
                   "f", make_vector<ExprNode>(VariableExprNode("x")))));
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
  auto then_stmts = make_vector<StmtNode>(AssignmentStmtNode(
           "_", std::make_unique<CallExprNode>(
                    "h", make_vector<ExprNode>(VariableExprNode("x"))))),
       else_stmts = make_vector<StmtNode>(AssignmentStmtNode(
           "_", std::make_unique<CallExprNode>(
                    "h", make_vector<ExprNode>(VariableExprNode("z"))))),
       stmts = make_vector<StmtNode>(
           LetStmtNode("z", "int"),
           AssignmentStmtNode(
               "z", std::make_unique<CallExprNode>(
                        "g", make_vector<ExprNode>(VariableExprNode("y")))),
           std::make_unique<IfStmtNode>(
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
