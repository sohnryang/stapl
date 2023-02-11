#include <gtest/gtest.h>

#include <lexer.h>

#include <sstream>
#include <string>
#include <utility>

using namespace stapl::parsing;

TEST(LexerTest, Def) {
  Lexer lexer("def");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kDef, "def"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kEof, ""));
}

TEST(LexerTest, Number) {
  Lexer lexer("42 3.141592");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "42"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kFloat, "3.141592"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kEof, ""));
}

TEST(LexerTest, FunctionDef) {
  Lexer lexer("def func()\n  0");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kDef, "def"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIdentifier, "func"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kMisc, "("));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kMisc, ")"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kEof, ""));
}

TEST(LexerTest, Proto) {
  Lexer lexer("func(a: int): void");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIdentifier, "func"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kMisc, "("));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIdentifier, "a"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kMisc, ":"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIdentifier, "int"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kMisc, ")"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kMisc, ":"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIdentifier, "void"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kEof, ""));
}

TEST(LexerTest, Operator) {
  Lexer lexer("1 + 2 * 3 >= 0");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "1"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kOp, "+"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "2"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kOp, "*"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "3"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kOp, ">="));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kEof, ""));
}

TEST(LexerTest, If) {
  Lexer lexer("if x < 0");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIf, "if"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kIdentifier, "x"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kOp, "<"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kInt, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::kEof, ""));
}
