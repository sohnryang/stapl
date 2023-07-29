#include "ast_comparator.h"

#include <memory>

namespace stapl::ast {
bool expr_equals(const ExprNode &expr1, const ExprNode &expr2) {
  ASTComparator cmp;
  return std::visit(cmp, expr1, expr2);
}

bool stmt_equals(const StmtNode &stmt1, const StmtNode &stmt2) {
  ASTComparator cmp;
  return std::visit(cmp, stmt1, stmt2);
}

bool decl_equals(const DeclNode &decl1, const DeclNode &decl2) {
  ASTComparator cmp;
  return std::visit(cmp, decl1, decl2);
}
} // namespace stapl::ast
