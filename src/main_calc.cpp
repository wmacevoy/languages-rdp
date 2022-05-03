#include "calc.h"

int main(int argc, const char *argv[]) {
  Calc calc;

  for (int argi=1; argi<argc; ++argi) {
    std::string arg=argv[argi];
    if (arg=="--mockScanner") {
      calc.scanner = Scanner::mock();
      continue;
    }
    
    if (arg=="--mockParser") {
      calc.parser = Parser::mock();
      continue;
    }

    if (arg=="--file") {
      ++argi;
      calc.stream = Scanner::fileStream(argv[argi]);
      continue;
    }

    if (arg=="--string") {
      ++argi;
      calc.stream = Scanner::stringStream(argv[argi]);
      continue;
    }

    std::cerr << "uknown arg: " << arg << std::endl;
    exit(1);
  }

  if (!calc.toolchain()) {
    std::cerr << "toolchain configuration error." << std::endl;
    return 1;
  }

  if (!calc.parse()) {
    std::cerr << "syntax error: " << calc.prog->toJSON() << std::endl;
    return 1;
  }

  calc.run();
  
  std::cout << "ans=" << calc.ans << std::endl;
  return 0;
}
