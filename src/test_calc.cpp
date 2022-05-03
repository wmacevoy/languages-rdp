#include "gtest/gtest.h"

#include "calc.h"
#include "examples.h"

TEST(Calc,Mock) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);

    Calc calc;
    calc.stream = Scanner::stringStream(ex->input);
    calc.scanner = Scanner::mock();
    calc.parser = Parser::mock();

    ASSERT_TRUE(calc.toolchain());
    ASSERT_TRUE(calc.parse());
    ASSERT_EQ(*calc.prog,*ex->ast);
    calc.run();
    ASSERT_EQ(calc.ans,ex->ans);
  }
}

TEST(Calc,Real) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);

    Calc calc;
    calc.stream = Scanner::stringStream(ex->input);
    calc.scanner = Scanner::real();
    calc.parser = Parser::real();

    ASSERT_TRUE(calc.toolchain());
    ASSERT_TRUE(calc.parse());
    ASSERT_EQ(*calc.prog,*ex->ast);
    calc.run();
    ASSERT_EQ(calc.ans,ex->ans);
  }
}

