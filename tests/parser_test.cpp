#include <ast.h>
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
  auto node = parser.parse_proto();
  EXPECT_EQ(node.name, "func");
  std::vector<std::pair<std::string, std::string>> args{
      {"a", "int"}, {"b", "float"}, {"c", "int"}};
  EXPECT_EQ(node.args, args);
  EXPECT_EQ(node.return_type, "int");
}

TEST(ParserTest, Expr) {
  Parser parser("a*a + b*b - c*c");
  auto node = parser.parse_expr();
  EXPECT_TRUE(std::holds_alternative<std::unique_ptr<BinaryExprNode>>(node));
  auto toplevel_binexpr =
      std::move(std::get<std::unique_ptr<BinaryExprNode>>(node));
  EXPECT_EQ(toplevel_binexpr->op, "-");
  EXPECT_TRUE(std::holds_alternative<std::unique_ptr<BinaryExprNode>>(
      toplevel_binexpr->lhs));
  auto toplevel_lhs = std::move(
      std::get<std::unique_ptr<BinaryExprNode>>(toplevel_binexpr->lhs));
  EXPECT_EQ(toplevel_lhs->op, "+");
  EXPECT_TRUE(std::holds_alternative<std::unique_ptr<BinaryExprNode>>(
      toplevel_binexpr->rhs));
  auto toplevel_rhs = std::move(
      std::get<std::unique_ptr<BinaryExprNode>>(toplevel_binexpr->rhs));
  EXPECT_EQ("*", toplevel_rhs->op);
  auto toplevel_rhs_var1 =
      std::move(std::get<VariableExprNode>(toplevel_rhs->lhs));
  EXPECT_EQ(toplevel_rhs_var1.name, "c");
  auto toplevel_rhs_var2 =
      std::move(std::get<VariableExprNode>(toplevel_rhs->rhs));
  EXPECT_EQ(toplevel_rhs_var2.name, "c");
}

TEST(ParserTest, ParenExpr) {
  Parser parser("(a + b)*(a + b)");
  auto node = parser.parse_expr();
  EXPECT_TRUE(std::holds_alternative<std::unique_ptr<BinaryExprNode>>(node));
  auto toplevel_binexpr =
      std::move(std::get<std::unique_ptr<BinaryExprNode>>(node));
  EXPECT_EQ(toplevel_binexpr->op, "*");
  EXPECT_TRUE(std::holds_alternative<std::unique_ptr<BinaryExprNode>>(
      toplevel_binexpr->lhs));
  auto toplevel_lhs = std::move(
      std::get<std::unique_ptr<BinaryExprNode>>(toplevel_binexpr->lhs));
  EXPECT_EQ(toplevel_lhs->op, "+");
}
