#pragma once

#include "ast.h"

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace stapl::ir {
class IRGen {
private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unordered_map<std::string, llvm::AllocaInst *> current_scope_symbols =
      {};

  llvm::Value *binary_op_add(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_sub(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_mul(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_div(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_eq(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_neq(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_lt(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_gt(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_le(llvm::Value *lhs_val, llvm::Value *rhs_val);
  llvm::Value *binary_op_ge(llvm::Value *lhs_val, llvm::Value *rhs_val);

  llvm::AllocaInst *create_entry_block_alloc(llvm::Function *func,
                                             llvm::StringRef name,
                                             llvm::Type *type);

  llvm::Type *type_from_typename(const std::string &name);

public:
  IRGen();
  void codegen(ast::Module &module_node);
  void write_ir(std::ostream &os);

  llvm::Value *operator()(ast::LiteralExprNode<int> &node);
  llvm::Value *operator()(ast::LiteralExprNode<double> &node);
  llvm::Value *operator()(ast::LiteralExprNode<bool> &node);
  llvm::Value *operator()(ast::VariableExprNode &node);
  llvm::Value *operator()(std::unique_ptr<ast::BinaryExprNode> &node);
  llvm::Value *operator()(std::unique_ptr<ast::CallExprNode> &node);
  void operator()(ast::LetStmtNode &node);
  void operator()(ast::AssignmentStmtNode &node);
  void operator()(std::unique_ptr<ast::IfStmtNode> &node);
  void operator()(ast::ReturnStmtNode &node);
  void operator()(std::unique_ptr<ast::CompoundStmtNode> &node);
  void operator()(ast::FunctionDeclNode &node);
};
} // namespace stapl::ir
