#include "gtest/gtest.h"
#include "scanner.h"
#include "examples.h"


TEST(Scanner,Mock) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);
    
    Scanner::Ptr scanner=Scanner::mock();
    scanner->setString(ex->input);
    for (int i=0; i<ex->tokens.size(); ++i) {
      ASSERT_EQ(*scanner->next(),*ex->tokens[i]);
    }
  }
}

TEST(Scanner,PutBack) {
  int k = 0;
  Example::Ptr ex = Example::example(k);

  for (int i=0; i<ex->tokens.size(); ++i) {
    for (int k=i+1; k<=ex->tokens.size(); ++k) {
      Scanner::Ptr scanner=Scanner::mock();
      scanner->setString(ex->input);
      for (int j=0; j<k; ++j) { scanner->next(); }
      for (int j=k-1; j>=i; --j) { scanner->putBack(ex->tokens[j]); }
      for (int j=i; j<ex->tokens.size(); ++j) {
	ASSERT_EQ(*scanner->next(),*ex->tokens[j]);
      }
    }
  }
}

TEST(Scanner,Mark) {
  int k = 1;
  Example::Ptr ex = Example::example(k);

  Scanner::Ptr scanner=Scanner::mock();
  scanner->setString(ex->input);

  scanner->next();
  auto m1 = scanner->mark();
  auto m2 = scanner->mark();
  scanner->next();
  auto m3 = scanner->mark();
  scanner->next();
  scanner->accept(m3);
  ASSERT_EQ(scanner->next(),ex->tokens[m3+1]);
  scanner->accept(m2);
  ASSERT_EQ(scanner->next(),ex->tokens[m3+2]);
  scanner->reject(m1);
  ASSERT_EQ(scanner->next(),ex->tokens[m1]);
}


TEST(Scanner,Real) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);
    
    Scanner::Ptr scanner=Scanner::real();
    scanner->setString(ex->input);
    for (int i=0; i<ex->tokens.size(); ++i) {
      ASSERT_EQ(*scanner->next(),*ex->tokens[i]);
    }
  }
}


