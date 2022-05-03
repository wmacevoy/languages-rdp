#include "gtest/gtest.h"
#include "token.h"

TEST(Token,Identifier) {
  TokenType tokenType = TokenType::identifier;
  int line = 13;
  int col = 2;
  std::string id = "x";
  Token::Ptr token = Token::identifier(id,line,col);
  JSON obj = token->toJSON();

  ASSERT_EQ(token->getType(),tokenType);
  ASSERT_EQ(token->getLine(), line);
  ASSERT_EQ(token->getCol(),col);

  ASSERT_EQ(token->getId(),id);

  ASSERT_EQ(obj["type"],"token");
  ASSERT_EQ(obj["token-type"],"identifier");
  ASSERT_EQ(obj["id"],id);
  ASSERT_EQ(obj["line"],line);
  ASSERT_EQ(obj["col"],col);
  
  Token::Ptr dup(new Token(obj));

  ASSERT_EQ(token,dup);
}

TEST(Token,Keyword) {
  TokenType tokenType = TokenType::keyword;
  int line = 13;
  int col = 2;
  std::string word = "for";
  Token::Ptr token = Token::keyword(word,line,col);
  JSON obj = token->toJSON();

  ASSERT_EQ(token->getWord(),word);  

  ASSERT_EQ(token->getType(),tokenType);
  ASSERT_EQ(token->getLine(), line);
  ASSERT_EQ(token->getCol(),col);

  ASSERT_EQ(obj["type"],"token");
  ASSERT_EQ(obj["token-type"],"keyword");
  ASSERT_EQ(obj["word"],word);
  ASSERT_EQ(obj["line"],line);
  ASSERT_EQ(obj["col"],col);
	    
  Token::Ptr dup(new Token(obj));

  ASSERT_EQ(token,dup);
}

TEST(Token,Number) {
  TokenType tokenType = TokenType::number;
  int line = 13;
  int col = 2;
  double value = 3.14;
  Token::Ptr token = Token::number(value,line,col);
  JSON obj = token->toJSON();

  ASSERT_EQ(token->getType(),tokenType);
  ASSERT_EQ(token->getLine(), line);
  ASSERT_EQ(token->getCol(),col);

  ASSERT_EQ(token->getValue(),value);

  ASSERT_EQ(obj["type"],"token");
  ASSERT_EQ(obj["token-type"],"number");
  ASSERT_EQ(obj["value"],value);
  ASSERT_EQ(obj["line"],line);
  ASSERT_EQ(obj["col"],col);
	    
  Token::Ptr dup(new Token(obj));

  ASSERT_EQ(token,dup);
}

TEST(Token,Simple) {
  for (auto tokenType : { TokenType::add, TokenType::sub, TokenType::times,TokenType::divide,
			   TokenType::lparen, TokenType::rparen, TokenType::eof }) {
    int line = 13;
    int col = 2;
    Token::Ptr token;
    std::string strTokenType;
    
    switch(tokenType) {
    case TokenType::add:    token=Token::add(line,col);    strTokenType="add";    break;
    case TokenType::sub:    token=Token::sub(line,col);    strTokenType="sub";    break;
    case TokenType::times:  token=Token::times(line,col);  strTokenType="times";  break;
    case TokenType::divide: token=Token::divide(line,col); strTokenType="divide"; break;
    case TokenType::lparen: token=Token::lparen(line,col); strTokenType="lparen"; break;
    case TokenType::rparen: token=Token::rparen(line,col); strTokenType="rparen"; break;
    case TokenType::eof:    token=Token::eof(line,col);    strTokenType="eof";    break;
    default: throw std::range_error("invalid");
    }

    JSON obj = token->toJSON();

    ASSERT_EQ(token->getType(),tokenType);
    ASSERT_EQ(token->getLine(), line);
    ASSERT_EQ(token->getCol(),col);

    ASSERT_EQ(obj["type"],"token");
    ASSERT_EQ(obj["token-type"],strTokenType);
    ASSERT_EQ(obj["line"],line);
    ASSERT_EQ(obj["col"],col);
	    
    Token::Ptr dup(new Token(obj));

    ASSERT_EQ(token,dup);
  }
}

TEST(Token,Unrecognized) {
  TokenType tokenType = TokenType::unrecognized;
  int line = 13;
  int col = 2;
  std::string what = "?#4!!";
  Token::Ptr token = Token::unrecognized(what,line,col);
  JSON obj = token->toJSON();

  ASSERT_EQ(token->getType(),tokenType);
  ASSERT_EQ(token->getLine(), line);
  ASSERT_EQ(token->getCol(),col);

  ASSERT_EQ(token->getWhat(),what);

  ASSERT_EQ(obj["type"],"token");
  ASSERT_EQ(obj["token-type"],"unrecognized");
  ASSERT_EQ(obj["what"],what);
  ASSERT_EQ(obj["line"],line);
  ASSERT_EQ(obj["col"],col);
	    
  Token::Ptr dup(new Token(obj));

  ASSERT_EQ(token,dup);
}
