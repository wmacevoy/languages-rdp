#include "gtest/gtest.h"

#include "examples.h"
#include "vm.h"
#include "parser.h"


TEST(VM,Examples) {
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);
    
    VM vm;
    JSON expect = ex->ans;
    auto result = vm.run(ex->ast);
    ASSERT_EQ(result,expect);
  }
}
