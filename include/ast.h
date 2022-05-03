#pragma once

#include "port.h"

#include "token.h"

enum class ASTType { number,recall,store,add,sub,times,divide,unrecognized };

std::string ASTTypeToJSON(enum ASTType astType);
enum ASTType JSONToASTType(const std::string &jsonASTType);

class AST {
 public:
  typedef std::shared_ptr < AST > Ptr;
  friend int operator<=>(const Ptr &a, const Ptr &b);
  friend bool operator==(const Ptr &a, const Ptr &b);
  friend bool operator!=(const Ptr &a, const Ptr &b);
  friend bool operator<=(const Ptr &a, const Ptr &b);
  friend bool operator>=(const Ptr &a, const Ptr &b);
  friend bool operator<(const Ptr &a, const Ptr &b);
  friend bool operator>(const Ptr &a, const Ptr &b);
  
  static Ptr number(Token::Ptr token);
  static Ptr add(Token::Ptr token, Ptr arg0, Ptr arg1);
  static Ptr sub(Token::Ptr token, Ptr arg0, Ptr arg1);
  static Ptr times(Token::Ptr token, Ptr arg0, Ptr arg1);
  static Ptr divide(Token::Ptr token, Ptr arg0, Ptr arg1);
  static Ptr store(Token::Ptr token, Ptr arg0);
  static Ptr recall(Token::Ptr token);
  static Ptr unrecognized(Token::Ptr token);

  typedef std::function<JSON (const AST &ast)> Jsonify;
  AST(Token::Ptr _token, const Jsonify &_jsonify);
  AST(Token::Ptr _token, const Jsonify &_jsonify, Ptr arg0);
  AST(Token::Ptr _token, const Jsonify &_jsonify, Ptr arg0, Ptr arg1);

  ASTType getType() const;

  Jsonify jsonify;
  JSON toJSON() const;
  
  Token::Ptr token;
  std::vector<Ptr> args;

  int operator<=>(const AST&) const;  
};

bool operator==(const AST &a, const AST &b);
bool operator!=(const AST &a, const AST &b);
bool operator<=(const AST &a, const AST &b);
bool operator>=(const AST &a, const AST &b);
bool operator<(const AST &a, const AST &b);
bool operator>(const AST &a, const AST &b);

std::ostream& operator<<(std::ostream& out, const AST &ast);
