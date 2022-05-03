#include "port.h"
#include "ast.h"

#include "gtest/gtest.h"

AST::Ptr number(double value) {
  int line = 13;
  int col = 2;
  Token::Ptr token = Token::number(value,line,col);
  AST::Ptr ast = AST::number(token);
  return ast;
}

TEST(AST,Number) {
  int line = 13;
  int col = 2;
  double value = 3.14;
  Token::Ptr token = Token::number(value,line,col);
  AST::Ptr ast = AST::number(token);

  JSON expect=u8R"-=-({"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":3.14},"type":"ast","value":3.14})-=-"_JSON;
  ASSERT_EQ(ast->getType(),ASTType::number);
  ASSERT_EQ(ast->toJSON(),expect);
}

TEST(AST,Add) {
  int line = 13;
  int col = 32;
  Token::Ptr token = Token::add(line,col);
  AST::Ptr ast = AST::add(token,number(4),number(5));

  JSON expect=u8R"-=-({"args":[{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":4.0},"type":"ast","value":4.0},{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":5.0},"type":"ast","value":5.0}],"ast-type":"add","token":{"col":32,"line":13,"token-type":"add","type":"token"},"type":"ast"})-=-"_JSON;

  ASSERT_EQ(ast->getType(),ASTType::add);  
  ASSERT_EQ(ast->toJSON(),expect);
}

TEST(AST,Sub) {
  int line = 13;
  int col = 32;
  Token::Ptr token = Token::sub(line,col);
  AST::Ptr ast = AST::sub(token,number(4),number(5));

  JSON expect=u8R"-=-({"args":[{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":4.0},"type":"ast","value":4.0},{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":5.0},"type":"ast","value":5.0}],"ast-type":"sub","token":{"col":32,"line":13,"token-type":"sub","type":"token"},"type":"ast"})-=-"_JSON;

  ASSERT_EQ(ast->getType(),ASTType::sub);    
  ASSERT_EQ(ast->toJSON(),expect);
}

TEST(AST,Times) {
  int line = 13;
  int col = 32;
  Token::Ptr token = Token::times(line,col);
  AST::Ptr ast = AST::times(token,number(4),number(5));

  JSON expect=u8R"-=-({"args":[{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":4.0},"type":"ast","value":4.0},{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":5.0},"type":"ast","value":5.0}],"ast-type":"times","token":{"col":32,"line":13,"token-type":"times","type":"token"},"type":"ast"})-=-"_JSON;

  ASSERT_EQ(ast->getType(),ASTType::times);
  ASSERT_EQ(ast->toJSON(),expect);
}

TEST(AST,Divide) {
  int line = 13;
  int col = 32;
  Token::Ptr token = Token::divide(line,col);
  AST::Ptr ast = AST::divide(token,number(4),number(5));

  JSON expect=u8R"-=-({"args":[{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":4.0},"type":"ast","value":4.0},{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":5.0},"type":"ast","value":5.0}],"ast-type":"divide","token":{"col":32,"line":13,"token-type":"divide","type":"token"},"type":"ast"})-=-"_JSON;

  ASSERT_EQ(ast->getType(),ASTType::divide);
  ASSERT_EQ(ast->toJSON(),expect);
}

TEST(AST,Store) {
  int line = 13;
  int col = 32;
  Token::Ptr token = Token::keyword("S",line,col);
  AST::Ptr ast = AST::store(token,number(4));

  JSON expect=u8R"-=-({"args":[{"ast-type":"number","token":{"col":2,"line":13,"token-type":"number","type":"token","value":4.0},"type":"ast","value":4.0}],"ast-type":"keyword","token":{"col":32,"line":13,"token-type":"keyword","type":"token","word":"S"},"type":"ast"})-=-"_JSON;

  ASSERT_EQ(ast->getType(),ASTType::store);  
  ASSERT_EQ(ast->toJSON(),expect);
}

TEST(AST,Recall) {
  int line = 13;
  int col = 32;
  Token::Ptr token = Token::keyword("R",line,col);
  AST::Ptr ast = AST::recall(token);

  JSON expect=u8R"-=-({"ast-type":"keyword","token":{"col":32,"line":13,"token-type":"keyword","type":"token","word":"R"},"type":"ast"})-=-"_JSON;

  ASSERT_EQ(ast->getType(),ASTType::recall);
  ASSERT_EQ(ast->toJSON(),expect);
}



