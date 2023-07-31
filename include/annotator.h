#pragma once

#include "ast.h"
#include "types.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace stapl::types {
class TypeAnnotator {
private:
  std::unordered_map<std::string, std::string> variable_type_names = {};
  std::unordered_map<std::string, std::vector<FuncTypeInfo>> func_types = {};

public:
  TypeAnnotator();

  std::string operator()(ast::LiteralExprNode<int> &node);
  std::string operator()(ast::LiteralExprNode<double> &node);
  std::string operator()(ast::LiteralExprNode<bool> &node);
  std::string operator()(ast::VariableExprNode &node);
  std::string operator()(std::unique_ptr<ast::BinaryExprNode> &node);
  std::string operator()(std::unique_ptr<ast::CallExprNode> &node);
  void operator()(ast::LetStmtNode &node);
  void operator()(ast::AssignmentStmtNode &node);
  void operator()(std::unique_ptr<ast::IfStmtNode> &node);
  void operator()(ast::ReturnStmtNode &node);
  void operator()(std::unique_ptr<ast::CompoundStmtNode> &node);
  void operator()(ast::FunctionDeclNode &node);
};
}; // namespace stapl::types
