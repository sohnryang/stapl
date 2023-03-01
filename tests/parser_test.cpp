#include <ast.h>
#include <ast_comparator.h>
#include <parser.h>

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

TEST(ParserTest, Prototype) {
  Parser parser("func(a: int, b: float, c: int): int");
  PrototypeNode expected("func", {{"a", "int"}, {"b", "float"}, {"c", "int"}},
                         "int"),
      parsed = parser.parse_proto();
  ASTComparator cmp;
  EXPECT_TRUE(cmp(expected, parsed));
}

TEST(ParserTest, Expr) {
  Parser parser("a*a + b*b - c*c");
  ExprNode expected = std::make_unique<BinaryExprNode>(
               "-",
               std::make_unique<BinaryExprNode>(
                   "+",
                   std::make_unique<BinaryExprNode>("*", VariableExprNode("a"),
                                                    VariableExprNode("a")),
                   std::make_unique<BinaryExprNode>("*", VariableExprNode("b"),
                                                    VariableExprNode("b"))),
               std::make_unique<BinaryExprNode>("*", VariableExprNode("c"),
                                                VariableExprNode("c"))),
           parsed = parser.parse_expr();
  EXPECT_TRUE(expr_equals(expected, parsed));
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
  EXPECT_TRUE(expr_equals(expected, parsed));
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
  EXPECT_TRUE(expr_equals(expected, parsed));
}

TEST(ParserTest, Let) {
  Parser parser("let x: int");
  StmtNode expected(LetStmtNode("x", "int")), parsed = parser.parse_stmt();
  EXPECT_TRUE(stmt_equals(parsed, expected));
}

TEST(ParserTest, Assign) {
  Parser parser("x = 1 + y");
  StmtNode expected(AssignmentStmtNode(
      "x", ExprNode(std::make_unique<BinaryExprNode>(
               "+", LiteralExprNode<int>(1), VariableExprNode("y"))))),
      parsed = parser.parse_stmt();
  EXPECT_TRUE(stmt_equals(parsed, expected));
}

TEST(ParserTest, If) {
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
  EXPECT_TRUE(stmt_equals(expected, parsed));
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
  EXPECT_TRUE(stmt_equals(expected, parsed));
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
  EXPECT_TRUE(stmt_equals(expected, parsed));
}

TEST(ParserTest, Extern) {
  Parser parser("extern f(x: int, y: float): void");
  DeclNode expected(FunctionDeclNode(
      PrototypeNode("f", {{"x", "int"}, {"y", "float"}}, "void"))),
      parsed = parser.parse_extern();
  EXPECT_TRUE(decl_equals(expected, parsed));
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
  EXPECT_TRUE(decl_equals(expected, parsed));
}
