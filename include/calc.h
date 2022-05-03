#pragma once

#include "port.h"

#include "scanner.h"
#include "parser.h"
#include "vm.h"

class Calc {
 public:
  Scanner::Stream stream;
  Scanner::Ptr scanner;
  Parser::Ptr parser;

  Calc();
  VM vm;
  AST::Ptr prog;  
  JSON ans;

  bool toolchain();
  bool parse();
  void run();
};
