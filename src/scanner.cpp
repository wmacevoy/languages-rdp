#include "scanner.h"

#include "examples.h"

#include <fstream>
#include <sstream>

Scanner::Scanner() : at(0), debug(false)  {}

Scanner::Stream Scanner::stringStream(const std::string &str) {
  return Stream(new std::istringstream(str));
}

Scanner::Stream Scanner::fileStream(const std::string &file) {
  return Stream(new std::ifstream(file));  
}

Scanner::Stream Scanner::stdinStream() {
  // don't delete cin
  Stream shim(&std::cin, [](auto p) {});
  return shim;
}

void Scanner::setString(const std::string &str) {
  setStream(stringStream(str));
}

void Scanner::setFile(const std::string &file) {
  setStream(fileStream(file));
}

void Scanner::setStdin() {
  setStream(stdinStream());
}

void Scanner::setStream(Stream _stream) {
  at=0;
  putBacks.clear();
  marks.clear();
  marked.clear();
  stream = _stream;
}

Scanner::Stream Scanner::getStream() const {
  return stream;
}

void Scanner::putBack(Token::Ptr token) {
  --at;
  putBacks.push_back(token);
}

Token::Ptr Scanner::next() {
  Token::Ptr ans;
  if (!putBacks.empty()) {
    ans = *putBacks.rbegin();
    putBacks.pop_back();
  } else {
    ans = nextFromStream();
  }

  if (!marks.empty()) {
    marked.push_back(ans);
  }
  if (debug) {
    std::cerr << "token at=" << at << "=" << ans->toJSON() << std::endl;
  }
  ++at;
  return ans;
}

Scanner::Mark Scanner::mark() {
  marks.insert(at);
  return at;
}

void Scanner::unmark(Mark mark) {
  Mark lo1 = *marks.begin();
  auto i = marks.find(mark);
  if (i != marks.end()) {
    marks.erase(i);
  }
  Mark lo2 = marks.empty() ? at : *marks.begin();
  while (lo1 < lo2) {
    marked.pop_front();
    ++lo1;
  }
}

void Scanner::reset(Mark mark) {
  Mark lo = *marks.begin();
  while (lo + marked.size() > mark) {
    putBack(*marked.rbegin());
    marked.pop_back();
  }
}

void Scanner::accept(Mark mark) {
  unmark(mark);
}

void Scanner::reject(Mark mark) {
  reset(mark);
  unmark(mark);
}

Token::Ptr Scanner::nextFromStream() {
  return Token::eof(0,0);
}

class MockScanner : public Scanner {
 public:
  std::string str() {
    Stream stream = getStream();
    if (!stream) return "";
  
    std::string all;
    for (;;) {
      int ch = stream->get();
      if (ch < 0) break;
      all.push_back(ch);
    }
    return all;
  }

  void mockStream() {
    tokens.clear();
    std::string input=str();
    int n = Example::size();
    for (int k=0; k<n; ++k) {
      Example::Ptr ex = Example::example(k);
      if (ex->input == input) {
	tokens=ex->tokens;
	return;
      }
    }
    tokens.push_back(Token::unrecognized(input,0,0));
    tokens.push_back(Token::eof(0,0));
  }
  
  std::vector < Token::Ptr > tokens;
  virtual void setStream(Stream stream) override {
    Scanner::setStream(stream);
    mockStream();
  }
  
  virtual Token::Ptr nextFromStream() override {
    return tokens.at(at);
  }
};

class RealScanner : public Scanner {
public:
  int line;
  int col;
  bool eof;

  RealScanner() {
    line=0;
    col=0;
    eof=false;
  }

  virtual void setStream(Stream stream) override {
    Scanner::setStream(stream);
    line=0;
    col=0;
    eof=false;
    ungets.clear();
  }
  
  struct got {
    int ch;
    int line;
    int col;
    got(int _ch, int _line, int _col) : ch(_ch), line(_line), col(_col) {}
  };
  std::vector<got> ungets;

  void unget(const got &ungot) {
    ungets.push_back(ungot);
  }

  got get() {
    if (!ungets.empty()) {
      got ans=*ungets.rbegin();
      ungets.pop_back();
      return ans;
    }
    int ch = (eof || !stream) ? -1 : stream->get();
    if (ch < 0) { ch = -1; eof = true; }
    if (ch == '\r') {
      int lf = stream->get(); // windows cr/lf pair - maybe
      if (lf < 0) { lf = -1; eof = true; }
      else if (lf != '\n') {
	unget(got(lf,line+1,0));
      }
      ch = '\n';
    }
    
    got ans(ch,line,col);
    
    ++col;
    if (ch == '\n') {
      ++line;
      col=0;
    }
    return ans;
  }


  virtual Token::Ptr nextFromStream() override {
    got g = get();
    if (g.ch >= 'a' && g.ch <= 'z') return Token::identifier(std::string(1,char(g.ch)),g.line,g.col);
    if (g.ch == 'S' || g.ch == 'R') return Token::keyword(std::string(1,char(g.ch)),g.line,g.col);
    if (g.ch == '+') return Token::add(g.line,g.col);
    if (g.ch == '-') return Token::sub(g.line,g.col);
    if (g.ch == '*') return Token::times(g.line,g.col);
    if (g.ch == '/') return Token::divide(g.line,g.col);
    if (g.ch == '(') return Token::lparen(g.line,g.col);
    if (g.ch == ')') return Token::rparen(g.line,g.col);
    if (g.ch >= '0' && g.ch <= '9') {
      std::string strnum;
      bool decimal = false;
      got h=g;
      while ((h.ch >= '0' && h.ch <= '9') || (!decimal && h.ch == '.')) {
	strnum.push_back(h.ch);
	if (h.ch == '.') { decimal = true; }
	h = get();
      }
      unget(h);
      double value;
      std::istringstream iss(strnum);
      iss >> value;
      return Token::number(value,g.line,g.col);
    }
    if (g.ch == -1) return Token::eof(g.line,g.col);
    return Token::unrecognized(std::string(1,char(g.ch)),g.line,g.col);
  }
};

Scanner::Ptr Scanner::mock() { return Ptr(new MockScanner()); }

Scanner::Ptr Scanner::real() { return Ptr(new RealScanner()); }
