#pragma once

#include "port.h"

#include "ast.h"

class VM {
 public:
  typedef std::map<std::string,JSON> Heap;
  typedef std::stack<JSON> Stack;

  Heap heap;
  VM();
  JSON run(AST::Ptr prog);
 private:
  void exec(AST::Ptr prog, Stack &stack);
};
