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

/**
  A class for generating IR from AST.
 */
class IRGen {
private:
  std::map<std::string, Type> globals;
  std::map<std::string, llvm::Value *> global_symbols;
  std::map<std::string, std::pair<Type, std::vector<Type>>> functions;
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;

public:
  IRGen();
  /**
    Generate LLVM IR and write it to supplied path.
   */
  void codegen(std::string const &filename, std::vector<StatementNode> &ast);

  void operator()(FunctionNode &node);

  llvm::Value *operator()(LiteralExprNode<int> &node);
  llvm::Value *operator()(LiteralExprNode<double> &node);
  llvm::Value *operator()(VariableExprNode &node);
  llvm::Value *operator()(std::unique_ptr<BinaryExprNode> &node);
  llvm::Value *operator()(std::unique_ptr<CallExprNode> &node);
  llvm::Value *operator()(std::unique_ptr<IfExprNode> &node);
};
