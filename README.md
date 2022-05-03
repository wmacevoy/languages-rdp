# rdp

This is the infrastructure (thinking about how interepreters/compilers
are put together generally).  For a simple calculator with a store/recall
feature.

## structure

- token lexical tokens
  - tokens keep their state as json
  - toJSON() 
  - static builders Token::number() etc. for each TokenType.
  - use ref-counted shared pointers to simplify managment of tokens.
- ast abstract syntax tree
  - ast keep a reference token and args as child nodes.
  - toJSON()
  - jsonify internal function to compose JSON if required.  Functional approach avoids lots of sub-classes.
  - static builders AST::number() etc. for each ASTType
- vm virtual machine
  - heap is a "memory map" of addreses (string) to JSON documents (memory).
  - We only use heap["store"] for this to store a single (double) number.
  - run(prog) creates an empty stack (stack of json objects) as a run-time stack, then runs exec(prog,stack).  The only remaining value on the stack is the result of run.
  - exec(prog,stack).  Recursive method to execute (simulate) program on machine with given stack.
- scanner
  - mock scanner.  Produces test tokens for test inputs.
  - real scanner. Proper scanner for files, strings, and stdin.
- parser
  - mock parser
  - real parser
- examples.  Provides example cases for test/mock.
  
- calc (in main_calc).  Lazy builder to apply these to a given calculation.

## make / test / run

Calc by default uses stdin.

```sh
make all
./test_all
bin/calc [--mockScanner] [--mockParser] [--file <file>] [--string <string>]
```

`ex1.in` and `ex2.in` have the sample input strings.

```sh
make all && bin/calc --file ex1.in
```
should produce 81.0 as a result.  You can run these in a docker with 

```sh
./run bin/calc ...
```
## references

JSON data is stored this way
https://github.com/nlohmann/json

Tests are written with Google test

https://github.com/google/googletest

Textbook reference

PROGRAMMING LANGUAGES:
AN ACTIVE LEARNING APPROACH

By Kent D. Lee
Luther College

ISBN: 978-0-387-79421-1