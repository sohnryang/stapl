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

/**
 * @brief Classes related to LLVM IR generation.
 */
namespace stapl::ir {
/**
 * @brief A visitor for generating LLVM IR.
 */
class IRGen {
private:
  /**
   * @brief The LLVM context.
   */
  std::unique_ptr<llvm::LLVMContext> context;

  /**
   * @brief The LLVM module.
   */
  std::unique_ptr<llvm::Module> module;

  /**
   * @brief The LLVM IR builder.
   */
  std::unique_ptr<llvm::IRBuilder<>> builder;

  /**
   * @brief The current scope's symbols.
   */
  std::unordered_map<std::string, llvm::AllocaInst *> current_scope_symbols =
      {};

  /**
   * @brief Gernerate IR for addition of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the addition.
   */
  llvm::Value *binary_op_add(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for subtraction of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the subtraction.
   */
  llvm::Value *binary_op_sub(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for multiplication of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the multiplication.
   */
  llvm::Value *binary_op_mul(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for division of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the division.
   */
  llvm::Value *binary_op_div(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for equality of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the modulo.
   */
  llvm::Value *binary_op_eq(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for inequality of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the modulo.
   */
  llvm::Value *binary_op_neq(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for less-than comparision of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the modulo.
   */
  llvm::Value *binary_op_lt(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for greater-than comparision of two ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the modulo.
   */
  llvm::Value *binary_op_gt(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for less-than-or-equal comparision of two
   * ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the modulo.
   */
  llvm::Value *binary_op_le(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Gernerate IR for greater-than-or-equal comparision of two
   * ``llvm::Value *``.
   * @param lhs_val The value on LHS.
   * @param rhs_val The value on RHS.
   * @return The result of the modulo.
   */
  llvm::Value *binary_op_ge(llvm::Value *lhs_val, llvm::Value *rhs_val);

  /**
   * @brief Create an alloca instruction in the entry block of the function.
   * @param func The function to create the alloca in.
   * @param name The name of the alloca.
   * @param type The LLVM type of the alloca.
   * @return The created alloca instruction.
   *
   * This function is needed to create IR that can be optimized by LLVM mem2reg
   * optimization pass.
   */
  llvm::AllocaInst *create_entry_block_alloc(llvm::Function *func,
                                             llvm::StringRef name,
                                             llvm::Type *type);

  /**
   * @brief Get the LLVM type from a type name.
   * @param name The name of the type.
   * @return The LLVM type.
   */
  llvm::Type *type_from_typename(const std::string &name);

public:
  /**
   * @brief Default constructor.
   */
  IRGen();

  /**
   * @brief Generate IR for a module.
   * @param module_node The module to generate IR for.
   */
  void codegen(ast::Module &module_node);

  /**
   * @brief Write the generated IR to a stream.
   * @param os The ``std::ostream`` to write to.
   */
  void write_ir(std::ostream &os);

  /**
   * @brief Generate IR for integer literal.
   * @param node The node to generate IR for.
   * @return The generated IR.
   */
  llvm::Value *operator()(ast::LiteralExprNode<int> &node);

  /**
   * @brief Generate IR for float literal.
   * @param node The node to generate IR for.
   * @return The generated IR.
   */
  llvm::Value *operator()(ast::LiteralExprNode<double> &node);

  /**
   * @brief Generate IR for boolean literal.
   * @param node The node to generate IR for.
   * @return The generated IR.
   */
  llvm::Value *operator()(ast::LiteralExprNode<bool> &node);

  /**
   * @brief Generate IR for variable expression node.
   * @param node The node to generate IR for.
   * @return The generated IR.
   */
  llvm::Value *operator()(ast::VariableExprNode &node);

  /**
   * @brief Generate IR for binary expression node.
   * @param node The node to generate IR for.
   * @return The generated IR.
   */
  llvm::Value *operator()(std::unique_ptr<ast::BinaryExprNode> &node);

  /**
   * @brief Generate IR for call expression node.
   * @param node The node to generate IR for.
   * @return The generated IR.
   */
  llvm::Value *operator()(std::unique_ptr<ast::CallExprNode> &node);

  /**
   * @brief Generate IR for let statement node and add to current block.
   * @param node The node to generate IR for.
   */
  void operator()(ast::LetStmtNode &node);

  /**
   * @brief Generate IR for assignment statement node and add to current block.
   * @param node The node to generate IR for.
   */
  void operator()(ast::AssignmentStmtNode &node);

  /**
   * @brief Generate IR for if statement node and add to current block.
   * @param node The node to generate IR for.
   */
  void operator()(std::unique_ptr<ast::IfStmtNode> &node);

  /**
   * @brief Generate IR for return statement node and add to current block.
   * @param node The node to generate IR for.
   */
  void operator()(ast::ReturnStmtNode &node);

  /**
   * @brief Generate IR for compound statement node and add to current block.
   * @param node The node to generate IR for.
   */
  void operator()(std::unique_ptr<ast::CompoundStmtNode> &node);

  /**
   * @brief Generate IR for function statement node and add to current block.
   * @param node The node to generate IR for.
   */
  void operator()(ast::FunctionDeclNode &node);
};
} // namespace stapl::ir
