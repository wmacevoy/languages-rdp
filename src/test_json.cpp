// https://github.com/nlohmann/json
#include "port.h"

#include "gtest/gtest.h"

// using json = JSON;

TEST(json,string) {
  // https://pspdfkit.com/blog/2021/string-literals-character-   -and-multiplatform-cpp
  std::string s = u8R"-=-({ "happy": "😀", "pi": 3.141 })-=-"_string;

  JSON j = JSON::parse(s);

  ASSERT_EQ(j["happy"],u8R"-=-(😀)-=-");
  ASSERT_EQ(j["pi"],3.141);
}

TEST(json,object) {
  JSON j1;
  j1["answer"]["everything"] = 42;
  j1["fibonacci"]={1,1,2,3,5,8};

  JSON j2=u8R"-=-({ "answer": { "everything": 42 }, "fibonacci" : [1,1,2,3,5,8] })-=-"_JSON;

  ASSERT_EQ(j1,j2);

  for (auto& [key, value] : j1.items()) {
    std::ostringstream oss;
    oss << "j1[" << key << "]=" << value;
    std::string expect;
    if (key == "answer") expect=u8R"-=-(j1[answer]={"everything":42})-=-"_string;
    if (key == "fibonacci") expect=u8R"-=-(j1[fibonacci]=[1,1,2,3,5,8])-=-"_string;
    ASSERT_EQ(expect,oss.str());
  }
}
