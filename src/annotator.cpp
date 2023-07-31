#include "annotator.h"
#include "ast.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include <fmt/core.h>

namespace stapl::types {
TypeAnnotator::TypeAnnotator() {
  func_types["+"] = func_types["-"] = func_types["*"] = func_types["/"] = {
      {{"int", "int"}, "int"}, {{"float", "float"}, "float"}};
  func_types["=="] = func_types["!="] = func_types["<"] = func_types[">"] =
      func_types["<="] = func_types[">="] = {{{"int", "int"}, "bool"},
                                             {{"float", "float"}, "bool"},
                                             {{"bool", "bool"}, "bool"}};
}

std::string TypeAnnotator::operator()(ast::LiteralExprNode<int> &node) {
  node.expr_type = "int";
  return "int";
}

std::string TypeAnnotator::operator()(ast::LiteralExprNode<double> &node) {
  node.expr_type = "float";
  return "float";
}

std::string TypeAnnotator::operator()(ast::LiteralExprNode<bool> &node) {
  node.expr_type = "bool";
  return "bool";
}

std::string TypeAnnotator::operator()(ast::VariableExprNode &node) {
  node.expr_type = variable_type_names.at(node.name);
  return node.expr_type.value();
}

std::string
TypeAnnotator::operator()(std::unique_ptr<ast::BinaryExprNode> &node) {
  if (node->expr_type.has_value())
    return node->expr_type.value();

  std::string lhs_type = std::visit(*this, node->lhs),
              rhs_type = std::visit(*this, node->rhs);
  for (const auto &func_type : func_types.at(node->op)) {
    if (func_type.arg_types != std::vector<std::string>{lhs_type, rhs_type})
      continue;
    node->expr_type = func_type.return_type;
  }
  return node->expr_type.value();
}

std::string
TypeAnnotator::operator()(std::unique_ptr<ast::CallExprNode> &node) {
  if (node->expr_type.has_value())
    return node->expr_type.value();

  std::vector<std::string> arg_types;
  for (auto &arg : node->args)
    arg_types.push_back(std::visit(*this, arg));
  for (const auto &func_type : func_types.at(node->callee)) {
    if (func_type.arg_types != arg_types)
      continue;
    node->expr_type = func_type.return_type;
  }
  return node->expr_type.value();
}

void TypeAnnotator::operator()(ast::LetStmtNode &node) {
  variable_type_names[node.var_name] = node.var_type;
}

void TypeAnnotator::operator()(ast::AssignmentStmtNode &node) {
  std::string rhs_type = std::visit(*this, node.assign_expr),
              var_type = variable_type_names.at(node.var_name);

  // TODO: add dedicated exception for type error
  if (rhs_type != var_type)
    throw std::logic_error(fmt::format(
        "type mismatch: variable is {} but rhs is {}", var_type, rhs_type));
}

void TypeAnnotator::operator()(std::unique_ptr<ast::IfStmtNode> &node) {
  std::string condition_type = std::visit(*this, node->condition);

  // TODO: add dedicated exception for type error
  if (condition_type != "bool")
    throw std::logic_error(fmt::format(
        "type mismatch: condition must be bool but {}", condition_type));
  std::visit(*this, node->then_stmt);
  std::visit(*this, node->else_stmt);
}

void TypeAnnotator::operator()(ast::ReturnStmtNode &node) {
  std::visit(*this, node.return_expr);
}

void TypeAnnotator::operator()(std::unique_ptr<ast::CompoundStmtNode> &node) {
  for (auto &stmt : node->stmts)
    std::visit(*this, stmt);
}

void TypeAnnotator::operator()(ast::FunctionDeclNode &node) {
  std::string return_type = node.proto.return_type;
  std::vector<std::string> arg_types;
  variable_type_names.clear();
  for (const auto &[arg_name, arg_type] : node.proto.args) {
    arg_types.push_back(arg_type);
    if (variable_type_names.count(arg_name))
      throw std::logic_error(
          fmt::format("redefinition of argument {}", arg_name));
    variable_type_names[arg_name] = arg_type;
  }
  if (!func_types.count(node.proto.name))
    func_types[node.proto.name] = {};
  func_types[node.proto.name].push_back({arg_types, return_type});
  if (node.func_body.has_value())
    std::visit(*this, node.func_body.value());
}
} // namespace stapl::types
