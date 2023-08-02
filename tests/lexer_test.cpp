#include <gtest/gtest.h>

#include "lexer.h"

#include <sstream>
#include <string>
#include <utility>

using namespace stapl::parsing;

TEST(LexerTest, Def) {
  Lexer lexer("def");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Def, "def"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, ArraySubscript) {
  Lexer lexer("arr[0][1][2]");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "arr"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "["));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "]"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "["));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "1"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "]"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "["));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "2"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "]"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, Literals) {
  Lexer lexer("42 3.141592 true false");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "42"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Float, "3.141592"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Bool, "true"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Bool, "false"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, FunctionDef) {
  Lexer lexer("def func()\n  0");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Def, "def"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "func"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "("));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, ")"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, Proto) {
  Lexer lexer("func(a: int): void");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "func"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "("));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "a"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, ":"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "int"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, ")"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, ":"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "void"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, Operator) {
  Lexer lexer("(1 + 2 * 3 >= 0) != false");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, "("));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "1"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Op, "+"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "2"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Op, "*"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "3"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Op, ">="));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, ")"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Op, "!="));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Bool, "false"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, If) {
  Lexer lexer("if x < 0");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::If, "if"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "x"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Op, "<"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "0"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, Let) {
  Lexer lexer("let x: int");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Let, "let"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "x"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Misc, ":"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Identifier, "int"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Eof, ""));
}

TEST(LexerTest, Return) {
  Lexer lexer("return 42");
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Return, "return"));
  EXPECT_EQ(lexer.get_token(), Token(TokenKind::Int, "42"));
}
