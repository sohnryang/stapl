#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include "annotator.h"
#include "ast.h"

using namespace stapl::ast;
using namespace stapl::types;

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

TEST(TypeCheckerTest, BinaryExpr) {
  std::vector<StmtNode> let_stmts;
  let_stmts.push_back(LetStmtNode("x", "int"));
  let_stmts.push_back(LetStmtNode("y", "int"));
  let_stmts.push_back(LetStmtNode("z", "int"));
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
  std::vector<StmtNode> func_body;
  func_body.push_back(ReturnStmtNode(std::make_unique<BinaryExprNode>(
      "+", VariableExprNode("x"),
      std::make_unique<BinaryExprNode>("+", VariableExprNode("y"),
                                       VariableExprNode("z")))));
  DeclNode func_decl = FunctionDeclNode(
      PrototypeNode("add", {{"x", "int"}, {"y", "int"}, {"z", "int"}}, "int"),
      std::make_unique<CompoundStmtNode>(std::move(func_body)));
  std::vector<ExprNode> args;
  args.push_back(LiteralExprNode<int>(1));
  args.push_back(LiteralExprNode<int>(2));
  args.push_back(LiteralExprNode<int>(3));
  ExprNode call_expr = std::make_unique<CallExprNode>("add", std::move(args));
  TypeAnnotator a;
  std::visit(a, func_decl);
  EXPECT_EQ(std::visit(a, call_expr), "int");
}
