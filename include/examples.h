#pragma once

#include "port.h"

#include "token.h"
#include "ast.h"

struct Example {
  typedef std::shared_ptr<Example> Ptr;
  static std::size_t size();
  static Ptr example(int k);
  const std::string input;
  const std::vector < Token::Ptr > tokens;
  const AST::Ptr ast;
  const double ans;
  Example(const std::string &_input, const std::vector<Token::Ptr> &_tokens, const AST::Ptr &_ast, const double &_ans);
};



