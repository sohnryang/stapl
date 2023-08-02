#include "irgen.h"
#include "ast.h"

#include <cstdint>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>

#include <fmt/core.h>

namespace stapl::ir {
IRGen::IRGen()
    : context(new llvm::LLVMContext()), module(new llvm::Module("", *context)),
      builder(new llvm::IRBuilder<>(*context)) {}

void IRGen::codegen(ast::Module &module_node) {
  module->setModuleIdentifier(module_node.name);
  for (auto &decl : module_node.decls)
    std::visit(*this, decl);
}

void IRGen::write_ir(std::ostream &os) {
  llvm::raw_os_ostream out_stream(os);
  module->print(out_stream, nullptr);
}

llvm::Value *IRGen::unary_op_pos(llvm::Value *rhs_val) { return rhs_val; }

llvm::Value *IRGen::unary_op_neg(llvm::Value *rhs_val) {
  if (rhs_val->getType()->isDoubleTy())
    return builder->CreateFNeg(rhs_val);
  if (rhs_val->getType()->isIntegerTy())
    return builder->CreateNeg(rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::unary_op_not(llvm::Value *rhs_val) {
  if (rhs_val->getType()->isIntegerTy())
    return builder->CreateNot(rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_add(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFAdd(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateAdd(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_sub(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFSub(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateSub(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_mul(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFMul(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateMul(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_div(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFDiv(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy()) {
    llvm::Value *lhs_casted_val, *rhs_casted_val, *div_fp;
    lhs_casted_val = builder->CreateSIToFP(lhs_val, builder->getDoubleTy());
    rhs_casted_val = builder->CreateSIToFP(rhs_val, builder->getDoubleTy());
    div_fp = builder->CreateFDiv(lhs_casted_val, rhs_casted_val);
    return builder->CreateFPToSI(div_fp, builder->getInt32Ty());
  }
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_eq(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFCmpOEQ(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateICmpEQ(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_neq(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFCmpONE(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateICmpNE(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_lt(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFCmpOLT(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateICmpSLT(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_gt(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFCmpOGT(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateICmpSGT(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_le(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFCmpOLE(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateICmpSLE(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::binary_op_ge(llvm::Value *lhs_val, llvm::Value *rhs_val) {
  if (lhs_val->getType()->isDoubleTy() && rhs_val->getType()->isDoubleTy())
    return builder->CreateFCmpOGE(lhs_val, rhs_val);
  if (lhs_val->getType()->isIntegerTy() && rhs_val->getType()->isIntegerTy())
    return builder->CreateICmpSGE(lhs_val, rhs_val);
  throw std::logic_error("unknown type signature");
}

llvm::Value *IRGen::operator()(ast::LiteralExprNode<int> &node) {
  return llvm::ConstantInt::get(
      *context, llvm::APInt(32, static_cast<uint64_t>(node.value), true));
}

llvm::Value *IRGen::operator()(ast::LiteralExprNode<double> &node) {
  return llvm::ConstantFP::get(*context, llvm::APFloat(node.value));
}

llvm::Value *IRGen::operator()(ast::LiteralExprNode<bool> &node) {
  return llvm::ConstantInt::getBool(*context, node.value);
}

llvm::Value *IRGen::operator()(ast::VariableExprNode &node) {
  llvm::AllocaInst *alloc = current_scope_symbols.at(node.name);
  return builder->CreateLoad(alloc->getAllocatedType(), alloc);
}

llvm::AllocaInst *IRGen::create_entry_block_alloc(llvm::Function *func,
                                                  llvm::StringRef name,
                                                  llvm::Type *type) {
  llvm::IRBuilder<> tmp_block(&func->getEntryBlock(),
                              func->getEntryBlock().begin());
  return tmp_block.CreateAlloca(type, nullptr, name);
}

llvm::Type *IRGen::type_from_typename(const std::string &name) {
  if (name == "int")
    return builder->getInt32Ty();
  else if (name == "float")
    return builder->getDoubleTy();
  else if (name == "bool")
    return builder->getInt1Ty();
  else if (name == "void")
    return builder->getVoidTy();
  throw std::logic_error(fmt::format("unknown type: {}", name));
}

llvm::Value *IRGen::operator()(std::unique_ptr<ast::UnaryExprNode> &node) {
  auto rhs_val = std::visit(*this, node->rhs);
  if (rhs_val == nullptr)
    throw std::logic_error("failed to codegen for rhs");

  if (node->op == "+")
    return unary_op_pos(rhs_val);
  else if (node->op == "-")
    return unary_op_neg(rhs_val);
  else if (node->op == "!")
    return unary_op_not(rhs_val);
  throw std::logic_error(fmt::format("unknown unary operator: {}", node->op));
}

llvm::Value *IRGen::operator()(std::unique_ptr<ast::BinaryExprNode> &node) {
  auto lhs_val = std::visit(*this, node->lhs),
       rhs_val = std::visit(*this, node->rhs);
  if (lhs_val == nullptr)
    throw std::logic_error("failed to codegen for lhs");
  if (rhs_val == nullptr)
    throw std::logic_error("failed to codegen for rhs");

  if (node->op == "+")
    return binary_op_add(lhs_val, rhs_val);
  else if (node->op == "-")
    return binary_op_sub(lhs_val, rhs_val);
  else if (node->op == "*")
    return binary_op_mul(lhs_val, rhs_val);
  else if (node->op == "/")
    return binary_op_div(lhs_val, rhs_val);
  else if (node->op == "==")
    return binary_op_eq(lhs_val, rhs_val);
  else if (node->op == "!=")
    return binary_op_neq(lhs_val, rhs_val);
  else if (node->op == "<")
    return binary_op_lt(lhs_val, rhs_val);
  else if (node->op == ">")
    return binary_op_gt(lhs_val, rhs_val);
  else if (node->op == "<=")
    return binary_op_le(lhs_val, rhs_val);
  else if (node->op == ">=")
    return binary_op_ge(lhs_val, rhs_val);
  throw std::logic_error(fmt::format("unknown operator: {}", node->op));
}

llvm::Value *IRGen::operator()(std::unique_ptr<ast::CallExprNode> &node) {
  auto callee_func = module->getFunction(node->callee);
  if (callee_func == nullptr)
    throw std::logic_error(fmt::format("unknown function: {}", node->callee));
  if (callee_func->arg_size() != node->args.size())
    throw std::logic_error(
        fmt::format("arg count mismatch: expected {} args, got {} args",
                    callee_func->arg_size(), node->args.size()));
  std::vector<llvm::Value *> arg_vals;
  for (auto &arg : node->args) {
    auto arg_val = std::visit(*this, arg);
    arg_vals.push_back(arg_val);
  }
  return builder->CreateCall(callee_func, arg_vals);
}

void IRGen::operator()(ast::LetStmtNode &node) {
  llvm::Function *current_func = builder->GetInsertBlock()->getParent();
  llvm::Value *init_val;
  llvm::Type *var_type_ir;
  if (node.var_type == "int") {
    init_val = llvm::ConstantInt::get(
        *context, llvm::APInt(32, static_cast<uint64_t>(0), true));
    var_type_ir = builder->getInt32Ty();
  } else if (node.var_type == "float") {
    init_val = llvm::ConstantFP::get(*context, llvm::APFloat(0.0));
    var_type_ir = builder->getDoubleTy();
  } else
    throw std::logic_error(fmt::format("unknown type: {}", node.var_type));
  llvm::AllocaInst *alloc =
      create_entry_block_alloc(current_func, node.var_name, var_type_ir);
  builder->CreateStore(init_val, alloc);
  current_scope_symbols[node.var_name] = alloc;
}

void IRGen::operator()(ast::AssignmentStmtNode &node) {
  llvm::Value *rhs_val = std::visit(*this, node.assign_expr),
              *lhs_val = current_scope_symbols.at(node.var_name);
  builder->CreateStore(rhs_val, lhs_val);
}

void IRGen::operator()(std::unique_ptr<ast::IfStmtNode> &node) {
  llvm::Value *cond_expr = std::visit(*this, node->condition);
  llvm::Function *current_func = builder->GetInsertBlock()->getParent();

  llvm::BasicBlock *then_block =
                       llvm::BasicBlock::Create(*context, "then", current_func),
                   *else_block = llvm::BasicBlock::Create(*context, "else"),
                   *merge_block = llvm::BasicBlock::Create(*context, "merge");

  builder->CreateCondBr(cond_expr, then_block, else_block);
  builder->SetInsertPoint(then_block);
  std::visit(*this, node->then_stmt);
  if (builder->GetInsertBlock()->getTerminator() == nullptr)
    builder->CreateBr(merge_block);

  current_func->insert(current_func->end(), else_block);
  builder->SetInsertPoint(else_block);
  std::visit(*this, node->else_stmt);
  if (builder->GetInsertBlock()->getTerminator() == nullptr)
    builder->CreateBr(merge_block);

  current_func->insert(current_func->end(), merge_block);
  builder->SetInsertPoint(merge_block);
}

void IRGen::operator()(ast::ReturnStmtNode &node) {
  llvm::Value *return_expr = std::visit(*this, node.return_expr);
  builder->CreateRet(return_expr);
}

void IRGen::operator()(std::unique_ptr<ast::CompoundStmtNode> &node) {
  for (auto &stmt : node->stmts)
    std::visit(*this, stmt);
}

void IRGen::operator()(ast::FunctionDeclNode &node) {
  std::vector<llvm::Type *> arg_types;
  for (auto &arg : node.proto.args) {
    const std::string &type_name = arg.second;
    llvm::Type *type = type_from_typename(type_name);
    if (type->isVoidTy())
      throw std::logic_error("void not allowed here");
    arg_types.push_back(type);
  }
  llvm::Type *return_type = type_from_typename(node.proto.return_type);
  llvm::FunctionType *func_type =
      llvm::FunctionType::get(return_type, arg_types, false);
  llvm::Function *func =
      llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                             node.proto.name, module.get());
  auto arg_name_it = node.proto.args.begin();
  for (auto &arg : func->args()) {
    arg.setName(arg_name_it->first);
    arg_name_it++;
  }
  if (!node.func_body.has_value())
    return;

  llvm::BasicBlock *func_block =
      llvm::BasicBlock::Create(*context, "entry", func);
  builder->SetInsertPoint(func_block);
  current_scope_symbols.clear();
  for (auto &arg : func->args()) {
    llvm::AllocaInst *alloc =
        create_entry_block_alloc(func, arg.getName(), arg.getType());
    builder->CreateStore(&arg, alloc);
    current_scope_symbols[std::string(arg.getName())] = alloc;
  }
  std::visit(*this, node.func_body.value());
  if (builder->GetInsertBlock()->getTerminator() == nullptr)
    builder->CreateRet(llvm::UndefValue::get(return_type));
  llvm::verifyFunction(*func);
}
} // namespace stapl::ir
