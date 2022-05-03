#include <stdexcept>
#include "token.h"

std::string TokenTypeToJSON(TokenType tokenType) {
  switch(tokenType) {
  case TokenType::identifier: return "identifier";
  case TokenType::keyword: return "keyword";
  case TokenType::number: return "number";
  case TokenType::add: return "add";

  case TokenType::sub: return "sub";
  case TokenType::divide: return "divide";
  case TokenType::times: return "times";
  case TokenType::lparen: return "lparen";
  case TokenType::rparen: return "rparen";
  case TokenType::eof: return "eof";
  case TokenType::unrecognized: return "unrecognized";
  default: throw std::range_error("invalid token type");
  }
}

TokenType JSONToTokenType(const std::string &jsonTokenType) {
  if (jsonTokenType == "identifier") return TokenType::identifier;
  if (jsonTokenType == "keyword") return TokenType::keyword;
  if (jsonTokenType == "number") return TokenType::number;
  if (jsonTokenType == "add") return TokenType::add;
  if (jsonTokenType == "sub") return TokenType::sub;
  if (jsonTokenType == "divide") return TokenType::divide;
  if (jsonTokenType == "times") return TokenType::times;
  if (jsonTokenType == "lparen") return TokenType::lparen;
  if (jsonTokenType == "rparen") return TokenType::rparen;
  if (jsonTokenType == "eof") return TokenType::eof;
  if (jsonTokenType == "unrecognized") return TokenType::unrecognized;
  throw std::range_error("invalid token string");
}

Token::Token() {}

Token::Token(const JSON &_obj) : obj(_obj) {}

const JSON& Token::toJSON() const {
  return obj;
}

TokenType Token::getType() const { return JSONToTokenType(obj["token-type"]); }
int Token::getLine() const { return int(obj["line"]); }
int Token::getCol() const { return int(obj["col"]); }
int Token::operator<=>(const Token &to) const { return obj < to.obj ? -1 : (obj== to.obj ? 0 : 1); }

Token::Ptr Token::base(TokenType type, int line, int col) {
  JSON obj;
  obj["type"]="token";
  obj["token-type"]=TokenTypeToJSON(type);
  obj["line"]=line;
  obj["col"]=col;
  return Ptr(new Token(obj));
}

Token::Ptr Token::identifier(const std::string &id, int line, int col) {
  Ptr ans = base(TokenType::identifier,line,col);
  ans->obj["id"]=id;
  return ans;
}

Token::Ptr Token::keyword(const std::string &word, int line, int col) {
  Ptr ans = base(TokenType::keyword,line,col);
  ans->obj["word"]=word;
  return ans;
}

Token::Ptr Token::number(double value, int line, int col) {
  Ptr ans = base(TokenType::number,line,col);
  ans->obj["value"]=value;
  return ans;
}

Token::Ptr Token::add(int line, int col) {
  return base(TokenType::add,line,col);
}

Token::Ptr Token::sub(int line, int col) {
  return base(TokenType::sub,line,col);
}

Token::Ptr Token::times(int line, int col) {
  return base(TokenType::times,line,col);
}

Token::Ptr Token::divide(int line, int col) {
  return base(TokenType::divide,line,col);
}

Token::Ptr Token::lparen(int line, int col) {
  return base(TokenType::lparen,line,col);
}

Token::Ptr Token::rparen(int line, int col) {
  return base(TokenType::rparen,line,col);
}

Token::Ptr Token::eof(int line, int col) {
  return base(TokenType::eof,line,col);
}

Token::Ptr Token::unrecognized(const std::string &what, int line, int col) {
  Ptr ans = base(TokenType::unrecognized,line,col);
  ans->obj["what"]=what;
  return ans;
}

double Token::getValue() const {
  if (getType() != TokenType::number) {
    throw std::range_error("not a number");
  }
  return double(obj["value"]);
}

std::string Token::getId() const {
  if (getType() != TokenType::identifier) {
    throw std::range_error("not an identifier");
  }
  return std::string(obj["id"]);
}

std::string Token::getWhat() const {
  if (getType() != TokenType::unrecognized) {
    throw std::range_error("known token");
  }
  return std::string(obj["what"]);
}

std::string Token::getWord() const {
  if (getType() != TokenType::keyword) {
    throw std::range_error("not a keyword");
  }
  return std::string(obj["word"]);
}


int operator<=>(const Token::Ptr &a, const Token::Ptr &b) {
  if ((&a == &b) || (!a && !b)) return 0;
  if (!a) return -1;
  if (!b) return  1;
  return (*a) <=> (*b);
}

bool operator==(const Token::Ptr &a, const Token::Ptr &b) { return (a <=> b) == 0; }
bool operator!=(const Token::Ptr &a, const Token::Ptr &b) { return (a <=> b) != 0; }
bool operator<=(const Token::Ptr &a, const Token::Ptr &b) { return (a <=> b) <= 0; }
bool operator>=(const Token::Ptr &a, const Token::Ptr &b) { return (a <=> b) >= 0; }
bool operator<(const Token::Ptr &a, const Token::Ptr &b)  { return (a <=> b) < 0; }
bool operator>(const Token::Ptr &a, const Token::Ptr &b)  { return (a <=> b) > 0; } 

bool operator==(const Token &a, const Token &b) { return (a <=> b) == 0; }
bool operator!=(const Token &a, const Token &b)	{ return (a <=> b) != 0; }
bool operator<=(const Token &a, const Token &b)	{ return (a <=> b) <= 0; }
bool operator>=(const Token &a, const Token &b)	{ return (a <=> b) >= 0; }
bool operator<(const Token &a, const Token &b)	{ return (a <=> b) < 0; } 
bool operator>(const Token &a, const Token &b)	{ return (a <=> b) > 0; }

std::ostream& operator<<(std::ostream& out, const Token &token) {
  return out << token.toJSON();
}
