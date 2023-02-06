#pragma once

#include <ast.h>
#include <typechecker.h>

#include <map>
#include <memory>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace stapl::ir {
/**
  A class for generating IR from AST.
 */
class IRGen {
private:
  std::map<std::string, types::Type> globals;
  std::map<std::string, llvm::Value *> global_symbols;
  std::map<std::string, std::pair<types::Type, std::vector<types::Type>>>
      functions;
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;

public:
  IRGen();
  /**
    Generate LLVM IR and write it to supplied path.
   */
  void codegen(std::string const &filename,
               std::vector<ast::StatementNode> &ast);

  void operator()(ast::FunctionNode &node);

  llvm::Value *operator()(ast::LiteralExprNode<int> &node);
  llvm::Value *operator()(ast::LiteralExprNode<double> &node);
  llvm::Value *operator()(ast::VariableExprNode &node);
  llvm::Value *operator()(std::unique_ptr<ast::BinaryExprNode> &node);
  llvm::Value *operator()(std::unique_ptr<ast::CallExprNode> &node);
  llvm::Value *operator()(std::unique_ptr<ast::IfExprNode> &node);
};
} // namespace stapl::ir
