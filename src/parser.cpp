#include "ast.h"
#include "parser.h"
#include "examples.h"

void Parser::setScanner(Scanner::Ptr _scanner) {
  scanner=_scanner;
}

Scanner::Ptr Parser::getScanner() const {
  return scanner;
}

AST::Ptr Parser::parse() { return AST::unrecognized(Token::eof(0,0)); }

class MockParser : public Parser {
 public:
  virtual AST::Ptr parse();
};

AST::Ptr MockParser::parse() {
  Scanner::Ptr scanner = getScanner();
  std::vector<Token::Ptr> tokens;
  for (int i=0; i<1000; ++i) {
    Token::Ptr token = scanner->next();
    tokens.push_back(token);
    if (token->getType() == TokenType::eof) break;
  }
  int n = Example::size();
  for (int k=0; k<n; ++k) {
    Example::Ptr ex = Example::example(k);
    if (tokens == ex->tokens) {
      return ex->ast;
    }
  }

  for (auto token : tokens) {
    std::cout << token->toJSON() << std::endl;
  }
  
  return AST::unrecognized(Token::eof(0,0));
}

struct RealParser : Parser {
  bool debug;
  typedef Scanner::Mark Mark;

  Token::Ptr next() {
    return getScanner()->next();
  }
  
  void putBack(Token::Ptr tkn) {
    getScanner()->putBack(tkn);
  }

  Mark mark() {
    return getScanner()->mark();
  }
  
  void accept(Mark mark) {
    return getScanner()->accept(mark);
  }

  void reject(Mark mark) {
    return getScanner()->reject(mark);
  }
  
  Token::Ptr next(std::function<bool(Token::Ptr)> filter) {
    Mark m = mark();
    Token::Ptr tkn = next();
    if (filter(tkn)) {
      accept(m);
      if (debug) {
	std::cerr << "accepted token " << tkn->toJSON() << std::endl;
      }
      return tkn;
    }
    reject(m);
    return Token::Ptr(NULL);
  }
  
  Token::Ptr next(TokenType tokenType) {
    return next([&](auto tkn) {
		  return tkn->getType() == tokenType;
		});
  }

  // TERMINALS in grammer.
  
  Token::Ptr NUMBER() { return next(TokenType::number); }
  Token::Ptr LPAREN() { return next(TokenType::lparen); }
  Token::Ptr RPAREN() { return next(TokenType::rparen); }
  Token::Ptr ADD()    { return next(TokenType::add); }
  Token::Ptr SUB()    { return next(TokenType::sub); }
  Token::Ptr TIMES()    { return next(TokenType::times); }
  Token::Ptr DIVIDE()    { return next(TokenType::divide); }
  Token::Ptr END_OF_FILE()    { return next(TokenType::eof); }  

  Token::Ptr KEYWORD(const std::string &word) {
    return next([&](auto tkn) {
		  return tkn->getType() == TokenType::keyword
		    && tkn->getWord() == word;
		});
  }

  Token::Ptr STORE() { return KEYWORD("S"); }
  Token::Ptr RECALL() { return KEYWORD("R"); }

  // all ::= expression END_OF_FILE.
  // expression :: = term ADD expression | term.
  // term ::= factor_opt_store TIMES term | factor_opt_store.
  // factor_opt_store ::= factor STORE | factor.
  // factor ::= number | recall | LPAREN expression RPAREN.

  // Recursive Decent Parsers work really badly on left-recursive grammars:
  //   if your gammar could try to match a rule without 
  //
  //  expression ::= expression ADD term | term

  AST::Ptr factorN() {
    auto m = mark();
    Token::Ptr N;
    if ((N=NUMBER())) {
      accept(m);
      return AST::number(N);
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // factor = RECALL
  AST::Ptr factorR() {
    auto m = mark();
    Token::Ptr R;
    if ((R=RECALL())) {
      accept(m);
      return AST::recall(R);
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // factor = ( expresssion )
  AST::Ptr factorPeP() {
    auto m = mark();
    Token::Ptr LP; AST::Ptr e; Token::Ptr RP;
    if ((LP=LPAREN()) && (e=expression()) && (RP=RPAREN())) {
      accept(m);
      return e;
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // factor = number | recall | ( expression ) [optional STORE]
  AST::Ptr factor() {
    auto m = mark();
    AST::Ptr f;
    if ((f=factorN()) || (f=factorR()) || (f=factorPeP())) {
      Token::Ptr S = STORE();
      if (S) {
	f=AST::store(S,f);
      }
      if (debug) {
	std::cerr << "accepted factor " << f->toJSON() << std::endl;
      }
      accept(m);
      return f;
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // term = factor TIMES term
  AST::Ptr termfTt() {
    auto m = mark();
    AST::Ptr f;
    Token::Ptr T;
    AST::Ptr t;
    if ((f=factor()) && (T=TIMES()) && (t=term())) {
      accept(m);
      return AST::times(T,f,t);
    }
    reject(m);
    return AST::Ptr(NULL);  
  }

  // term = factor TIMES term | factor
  AST::Ptr term() {
    auto m = mark();
    AST::Ptr t;
    if ((t = termfTt())||(t=factor())) {
      accept(m);
      if (debug) {
	std::cerr << "accepted term " << t->toJSON() << std::endl;
      }
      return t;
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // expression = term ADD expressioin
  AST::Ptr expressiontAe() {
    auto m = mark();
    AST::Ptr t;
    Token::Ptr A;
    AST::Ptr e;
    if ((t=term()) && (A=ADD()) && (e=expression())) {
      accept(m);
      return AST::add(A,t,e);
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // expression = term ADD expression | term 
  AST::Ptr expression() {
    auto m = mark();
    AST::Ptr e;
    if ((e = expressiontAe())||(e=term())) {
      accept(m);
      if (debug) {
	std::cerr << "accepted expression " << e->toJSON() << std::endl;
      }
      return e;
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  // all = expression EOF
  AST::Ptr all() {
    auto m = mark();
    AST::Ptr e;
    Token::Ptr E;
    if ((e = expression()) && (E=END_OF_FILE())) {
      accept(m);
      if (debug) {
	std::cerr << "accepted all " << e->toJSON() << std::endl;
      }
      return e;
    }
    reject(m);
    return AST::Ptr(NULL);
  }

  AST::Ptr parse() {
    return all();
  }

  RealParser() {
    debug = false;
  }
};

Parser::Ptr Parser::mock() { return Ptr(new MockParser()); }
Parser::Ptr Parser::real() { return Ptr(new RealParser()); }
