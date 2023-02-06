#include <gtest/gtest.h>

#include <ast.h>
#include <typechecker.h>

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using namespace stapl::ast;
using namespace stapl::types;

TEST(ExprTypeCheckerTest, Int) {
  ExprNode node = LiteralExprNode<int>(42);
  ExprTypeChecker checker;
  EXPECT_EQ(std::visit(checker, node), Type::kInt);
}

TEST(ExprTypeCheckerTest, Float) {
  ExprNode node = LiteralExprNode<double>(3.14);
  ExprTypeChecker checker;
  EXPECT_EQ(std::visit(checker, node), Type::kFloat);
}

TEST(ExprTypeCheckerTest, Variable) {
  ExprNode node = VariableExprNode("x");
  std::map<std::string, Type> globals = {{"x", Type::kFloat}};
  ExprTypeChecker checker(globals, {});
  EXPECT_EQ(std::visit(checker, node), Type::kFloat);
}

TEST(ExprTypeCheckerTest, BinaryExpr) {
  ExprNode var_node = VariableExprNode("x");
  ExprNode literal_node = LiteralExprNode<int>(42);
  ExprNode binary_node = std::move(std::make_unique<BinaryExprNode>(
      "*", std::move(var_node), std::move(literal_node)));
  std::map<std::string, Type> globals = {{"x", Type::kFloat}};
  ExprTypeChecker checker(globals, {});
  EXPECT_EQ(std::visit(checker, binary_node), Type::kFloat);
}

TEST(ExprTypeChecker, Void) {
  ExprNode arg_node = VariableExprNode("x");
  std::vector<ExprNode> args;
  args.push_back(std::move(arg_node));
  ExprNode call_node = std::make_unique<CallExprNode>("f", std::move(args));
  std::map<std::string, Type> globals = {{"x", Type::kFloat}};
  std::map<std::string, std::pair<Type, std::vector<Type>>> functions = {
      {"f", {Type::kVoid, {Type::kFloat}}}};
  ExprTypeChecker checker(globals, functions);
  EXPECT_NO_THROW(std::visit(checker, call_node));
  ExprNode binary_node = std::make_unique<BinaryExprNode>(
      "+", LiteralExprNode<int>(42), std::move(call_node));
  EXPECT_THROW(std::visit(checker, binary_node), std::logic_error);
}

TEST(ExprTypeCheckerTest, If) {
  ExprNode if_node = std::make_unique<IfExprNode>(
      std::make_unique<BinaryExprNode>("==", LiteralExprNode<int>(0),
                                       LiteralExprNode<int>(1)),
      LiteralExprNode<int>(1), LiteralExprNode<double>(0));
  ExprTypeChecker checker;
  EXPECT_EQ(std::visit(checker, if_node), Type::kFloat);
}
