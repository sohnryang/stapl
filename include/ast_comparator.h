#pragma once

#include <ast.h>
#include <memory>

namespace stapl::ast {
/**
  A visitor for comparing AST nodes.
 */
class ASTComparator {
public:
  template <typename T>
  bool operator()(const std::unique_ptr<T> &t1,
                  const std::unique_ptr<T> &t2) const {
    return *t1 == *t2;
  }

  template <typename T> bool operator()(const T &t1, const T &t2) const {
    return t1 == t2;
  }

  template <typename T1, typename T2>
  bool operator()(const T1 &, const T2 &) const {
    return false;
  }
};

/**
  Helper function for comparing ``ExprNode`` variants.
 */
bool expr_equals(const ExprNode &expr1, const ExprNode &expr2);

/**
  Helper function for comparing ``StmtNode`` variants.
 */
bool stmt_equals(const StmtNode &stmt1, const StmtNode &stmt2);

/**
  Helper function for comparing ``DeclNode`` variants.
 */
bool decl_equals(const DeclNode &decl1, const DeclNode &decl2);
} // namespace stapl::ast
