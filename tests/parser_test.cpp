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
