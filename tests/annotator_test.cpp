#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include "annotator.h"
#include "ast.h"
#include "util.h"

using namespace stapl::ast;
using namespace stapl::types;
using namespace stapl::util;

TEST(TypeCheckerTest, Literals) {
  ExprNode literal_int = LiteralExprNode<int>(42),
           literal_float = LiteralExprNode<double>(3.14);
  TypeAnnotator a;
  EXPECT_EQ(std::visit(a, literal_int), "int");
  EXPECT_EQ(std::get<LiteralExprNode<int>>(literal_int).expr_type.value(),
            "int");
  EXPECT_EQ(std::visit(a, literal_float), "float");
  EXPECT_EQ(std::get<LiteralExprNode<double>>(literal_float).expr_type.value(),
            "float");
}

TEST(TypeCheckerTest, Variables) {
  StmtNode let_stmt = LetStmtNode("x", "int");
  ExprNode var_x_expr = VariableExprNode("x"),
           var_y_expr = VariableExprNode("y");
  TypeAnnotator a;
  std::visit(a, let_stmt);
  EXPECT_EQ(std::visit(a, var_x_expr), "int");
  EXPECT_EQ(std::get<VariableExprNode>(var_x_expr).expr_type.value(), "int");
  EXPECT_THROW(std::visit(a, var_y_expr), std::logic_error);
}

TEST(TypeCheckerTest, UnaryExpr) {
  auto let_stmts =
      make_vector<StmtNode>(LetStmtNode("x", "int"), LetStmtNode("y", "int"));
  StmtNode block_stmt =
      std::make_unique<CompoundStmtNode>(std::move(let_stmts));
  ExprNode expr = std::make_unique<UnaryExprNode>(
      "!",
      std::make_unique<BinaryExprNode>(
          "==", std::make_unique<UnaryExprNode>("-", VariableExprNode("x")),
          std::make_unique<UnaryExprNode>("+", VariableExprNode("y"))));
  TypeAnnotator a;
  std::visit(a, block_stmt);
  EXPECT_EQ(std::visit(a, expr), "bool");
  EXPECT_EQ(std::get<std::unique_ptr<UnaryExprNode>>(expr)->expr_type.value(),
            "bool");
  EXPECT_EQ(std::get<std::unique_ptr<BinaryExprNode>>(
                std::get<std::unique_ptr<UnaryExprNode>>(expr)->rhs)
                ->expr_type.value(),
            "bool");
  EXPECT_EQ(std::get<std::unique_ptr<UnaryExprNode>>(
                std::get<std::unique_ptr<BinaryExprNode>>(
                    std::get<std::unique_ptr<UnaryExprNode>>(expr)->rhs)
                    ->lhs)
                ->expr_type.value(),
            "int");
  EXPECT_EQ(std::get<std::unique_ptr<UnaryExprNode>>(
                std::get<std::unique_ptr<BinaryExprNode>>(
                    std::get<std::unique_ptr<UnaryExprNode>>(expr)->rhs)
                    ->rhs)
                ->expr_type.value(),
            "int");
}

TEST(TypeCheckerTest, BinaryExpr) {
  auto let_stmts =
      make_vector<StmtNode>(LetStmtNode("x", "int"), LetStmtNode("y", "int"),
                            LetStmtNode("z", "int"));
  StmtNode block_stmt =
      std::make_unique<CompoundStmtNode>(std::move(let_stmts));
  ExprNode expr = std::make_unique<BinaryExprNode>(
      "+",
      std::make_unique<BinaryExprNode>("*", LiteralExprNode<int>(42),
                                       VariableExprNode("x")),
      std::make_unique<BinaryExprNode>("/", VariableExprNode("y"),
                                       VariableExprNode("z")));
  TypeAnnotator a;
  std::visit(a, block_stmt);
  EXPECT_EQ(std::visit(a, expr), "int");
  EXPECT_EQ(std::get<std::unique_ptr<BinaryExprNode>>(expr)->expr_type.value(),
            "int");
  EXPECT_EQ(std::get<std::unique_ptr<BinaryExprNode>>(
                std::get<std::unique_ptr<BinaryExprNode>>(expr)->lhs)
                ->expr_type.value(),
            "int");
  EXPECT_EQ(std::get<std::unique_ptr<BinaryExprNode>>(
                std::get<std::unique_ptr<BinaryExprNode>>(expr)->rhs)
                ->expr_type.value(),
            "int");
}

TEST(TypeCheckerTest, CallExpr) {
  auto func_body =
      make_vector<StmtNode>(ReturnStmtNode(std::make_unique<BinaryExprNode>(
          "+", VariableExprNode("x"),
          std::make_unique<BinaryExprNode>("+", VariableExprNode("y"),
                                           VariableExprNode("z")))));
  DeclNode func_decl = FunctionDeclNode(
      PrototypeNode("add", {{"x", "int"}, {"y", "int"}, {"z", "int"}}, "int"),
      std::make_unique<CompoundStmtNode>(std::move(func_body)));
  std::vector<ExprNode> args =
      make_vector<ExprNode>(LiteralExprNode<int>(1), LiteralExprNode<int>(2),
                            LiteralExprNode<int>(3));
  ExprNode call_expr = std::make_unique<CallExprNode>("add", std::move(args));
  TypeAnnotator a;
  std::visit(a, func_decl);
  EXPECT_EQ(std::visit(a, call_expr), "int");
}

TEST(TypeCheckerTest, FunctionScope) {
  auto func_body_f = make_vector<StmtNode>(
      LetStmtNode("x", "int"), LetStmtNode("y", "int"),
      AssignmentStmtNode(
          "x", std::make_unique<BinaryExprNode>("+", VariableExprNode("a"),
                                                VariableExprNode("b"))),
      AssignmentStmtNode(
          "y", std::make_unique<BinaryExprNode>("+", VariableExprNode("a"),
                                                VariableExprNode("b"))),
      ReturnStmtNode(std::make_unique<BinaryExprNode>(
          "*", VariableExprNode("x"), VariableExprNode("y"))));
  DeclNode func_decl_f = FunctionDeclNode(
      PrototypeNode("f", {{"a", "int"}, {"b", "int"}}, "int"),
      std::make_unique<CompoundStmtNode>(std::move(func_body_f)));

  auto func_body_g =
      make_vector<StmtNode>(ReturnStmtNode(VariableExprNode("x")));
  DeclNode func_decl_g = FunctionDeclNode(
      PrototypeNode("g", {}, "int"),
      std::make_unique<CompoundStmtNode>(std::move(func_body_g)));

  TypeAnnotator a;
  std::visit(a, func_decl_f);
  EXPECT_THROW(std::visit(a, func_decl_g), std::logic_error);
}
