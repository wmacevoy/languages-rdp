#include "gtest/gtest.h"

#include "ast.h"
#include "scanner.h"
#include "parser.h"
#include "examples.h"

TEST(Parser,Mock) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);
    
    Scanner::Ptr scanner = Scanner::mock();
    Parser::Ptr parser = Parser::mock();

    scanner->setString(ex->input);
    parser->setScanner(scanner);
    
    AST::Ptr expect = ex->ast;
    AST::Ptr result = parser->parse();

    ASSERT_EQ(expect,result);
  }
}

TEST(Parser,Real) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);

    // not testing the scanner here, so mock scanner is ok
    Scanner::Ptr scanner = Scanner::mock();
    Parser::Ptr parser = Parser::real();

    scanner->setString(ex->input);
    parser->setScanner(scanner);
    
    AST::Ptr expect = ex->ast;
    AST::Ptr result = parser->parse();

    ASSERT_EQ(expect,result);
  }
}
