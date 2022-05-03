#include "calc.h"

Calc::Calc() {
}

bool Calc::toolchain() {
  if (!!parser && !!parser->getScanner()) {
    scanner = parser->getScanner();
  }
  if (!!scanner && !!scanner->getStream()) {
    stream = scanner->getStream();
  }
  if (!stream) {
    stream = Scanner::stdinStream();
  }
  if (!scanner) {
    scanner = Scanner::real();
  }
  if (!parser) {
    parser = Parser::real();
  }
  if (scanner->getStream() != stream) {
    scanner->setStream(stream);
  }
  if (parser->getScanner() != scanner) {
    parser->setScanner(scanner);
  }
  return !!parser;
}

bool Calc::parse() {
  if (!toolchain()) {
    throw std::logic_error("toolchain failed");
  }
  
  prog = parser->parse();
  
  if (!prog || prog->getType() == ASTType::unrecognized) return false;
  
  return true;
}

void Calc::run() {
  if (!toolchain() || !prog || prog->getType() == ASTType::unrecognized) {
    throw std::logic_error("parse failed");    
  }
  ans = vm.run(prog);
};
