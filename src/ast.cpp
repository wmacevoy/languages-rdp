
#include <stdexcept>
#include "ast.h"

std::string ASTTypeToJSON(enum ASTType astType) {
  switch(astType) {
  case ASTType::number: return "number";
  case ASTType::add: return "add";
  case ASTType::sub: return "sub";
  case ASTType::divide: return "divide";
  case ASTType::times: return "times";
  case ASTType::store: return "store";
  case ASTType::recall: return "recall";
  case ASTType::unrecognized: return "unrecognized";    
  default: throw std::range_error("invalid ast type");
  }
}

enum ASTType JSONToASTType(const std::string &jsonASTType) {
  if (jsonASTType == "number") return ASTType::number;
  if (jsonASTType == "add") return ASTType::add;
  if (jsonASTType == "sub") return ASTType::sub;
  if (jsonASTType == "divide") return ASTType::divide;
  if (jsonASTType == "times") return ASTType::times;
  if (jsonASTType == "store") return ASTType::store;
  if (jsonASTType == "recall") return ASTType::recall;
  if (jsonASTType == "unrecognized") return ASTType::unrecognized;
  throw std::range_error("invalid ast string");
}

static JSON baseJsonify(const AST &ast) {
  JSON ans;
  ans["type"]="ast";
  if (ast.token) {
    ans["ast-type"]=TokenTypeToJSON(ast.token->getType());
    ans["token"]=ast.token->toJSON();
  } else {
    ans["ast-type"]="unrecognized";
    ans["token"]; // null
  }
  if (ast.args.size() > 0) {
    for (auto arg : ast.args) {
      ans["args"].push_back(arg->toJSON());
    }
  }
  return ans;
}

static JSON numberJsonify(const AST &ast) {
  JSON ans=baseJsonify(ast);
  ans["value"]=ast.token->getValue();
  return ans;
}

AST::Ptr AST::number(Token::Ptr token) {
  if (token->getType() == TokenType::number) {
    return Ptr(new AST(token,numberJsonify));
  } else {
     throw std::range_error("not number");
  }
}

AST::Ptr AST::add(Token::Ptr token, AST::Ptr arg0, AST::Ptr arg1) {
  if (token->getType() == TokenType::add) {
    return Ptr(new AST(token,baseJsonify,arg0,arg1));
  } else {
    throw std::range_error("not add");
  }
}

AST::Ptr AST::unrecognized(Token::Ptr token) {

  Token::Ptr unrec = Token::unrecognized(token->toJSON().dump(),token->getLine(),token->getCol());
  return Ptr(new AST(unrec,baseJsonify));
}

AST::Ptr AST::sub(Token::Ptr token, AST::Ptr arg0, AST::Ptr arg1) {
  if (token->getType() == TokenType::sub) {
    return Ptr(new AST(token,baseJsonify,arg0,arg1));
  } else {
    throw std::range_error("not sub");
  }
}

AST::Ptr AST::times(Token::Ptr token, AST::Ptr arg0, AST::Ptr arg1) {
  if (token->getType() == TokenType::times) {
    return Ptr(new AST(token,baseJsonify,arg0,arg1));
  } else {
    throw std::range_error("not times");
  }
}


AST::Ptr AST::divide(Token::Ptr token, AST::Ptr arg0, AST::Ptr arg1) {
  if (token->getType() == TokenType::divide) {
    return Ptr(new AST(token,baseJsonify,arg0,arg1));
  } else {
    throw std::range_error("not divide");
  }
}


AST::Ptr AST::store(Token::Ptr token, AST::Ptr arg0) {
  if (token->getType() == TokenType::keyword && token->getWord() == "S") {
    return Ptr(new AST(token,baseJsonify,arg0));
  } else {
    throw std::range_error("not store");
  }
}

AST::Ptr AST::recall(Token::Ptr token) {
  if (token->getType() == TokenType::keyword && token->getWord() == "R") {
    return Ptr(new AST(token,baseJsonify));
  } else {
    throw std::range_error("not recall");
  }
}


ASTType AST::getType() const {
  if (!token) return ASTType::unrecognized;
  switch(token->getType()) {
  case TokenType::number: return ASTType::number;
  case TokenType::add: return ASTType::add;
  case TokenType::sub: return ASTType::sub;
  case TokenType::times: return ASTType::times;
  case TokenType::divide: return ASTType::divide;
  case TokenType::keyword:
    if (token->getWord() == "S") return ASTType::store;
    if (token->getWord() == "R") return ASTType::recall;
  default: return ASTType::unrecognized;
  }
}


AST::AST(Token::Ptr _token, const AST::Jsonify &_jsonify) : token(_token), jsonify(_jsonify) {}

AST::AST(Token::Ptr _token, const AST::Jsonify &_jsonify, AST::Ptr arg0) : token(_token), jsonify(_jsonify), args(1) {
  args.at(0)=arg0;
}

AST::AST(Token::Ptr _token, const AST::Jsonify &_jsonify, AST::Ptr arg0, AST::Ptr arg1) : token(_token), jsonify(_jsonify), args(2) {
  args.at(0)=arg0;
  args.at(1)=arg1;  
}

JSON AST::toJSON() const {
  return jsonify(*this);
}

int AST::operator<=>(const AST &to) const {
  int cmp = token <=> to.token;
  if (cmp != 0) return cmp;
  std::size_t i;
  for (i=0; i<args.size() && i<to.args.size(); ++i) {
    cmp = args[i] <=> to.args[i];
    if (cmp != 0) return cmp;
  }
  if (i < args.size()) return 1;
  if (i < to.args.size()) return -1;
  return 0;

}

int operator<=>(const AST::Ptr &a, const AST::Ptr &b) {
  if ((&a == &b) || (!a && !b)) return 0;
  if (!a) return -1;
  if (!b) return  1;
  return (*a) <=> (*b);
}

bool operator==(const AST::Ptr &a, const AST::Ptr &b) { return (a <=> b) == 0; }
bool operator!=(const AST::Ptr &a, const AST::Ptr &b) { return (a <=> b) != 0; }
bool operator<=(const AST::Ptr &a, const AST::Ptr &b) { return (a <=> b) <= 0; }
bool operator>=(const AST::Ptr &a, const AST::Ptr &b) { return (a <=> b) >= 0; }
bool operator<(const AST::Ptr &a, const AST::Ptr &b) { return (a <=> b) < 0; }
bool operator>(const AST::Ptr &a, const AST::Ptr &b) { return (a <=> b) > 0; }

bool operator==(const AST &a, const AST &b)     { return (a <=> b) == 0; }
bool operator!=(const AST &a, const AST &b)	{ return (a <=> b) != 0; }
bool operator<=(const AST &a, const AST &b)	{ return (a <=> b) <= 0; }
bool operator>=(const AST &a, const AST &b)	{ return (a <=> b) >= 0; }
bool operator<(const AST &a, const AST &b)	{ return (a <=> b) < 0; } 
bool operator>(const AST &a, const AST &b)	{ return (a <=> b) > 0; }

std::ostream& operator<<(std::ostream& out, const AST &ast) {
  return out << ast.toJSON();
}
