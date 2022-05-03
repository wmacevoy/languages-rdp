#pragma once

#include "port.h"

#include "token.h"

class Scanner {
 public:
  typedef std::shared_ptr<std::istream> Stream;
  typedef std::shared_ptr<Scanner> Ptr;
  typedef uint64_t Mark;
  
protected:
  Stream stream;
  std::vector<Token::Ptr> putBacks;
  Mark at;
  bool debug;
  std::multiset<Mark> marks;
  std::deque<Token::Ptr> marked;
  void unmark(Mark mark);
  void reset(Mark mark);  
public:
  static Ptr mock();
  static Ptr real();

  static Stream stringStream(const std::string &str);
  static Stream fileStream(const std::string &file);
  static Stream stdinStream();

  Scanner();

  
  virtual void setString(const std::string &str);
  virtual void setFile(const std::string &file);  
  virtual void setStream(Stream stream);
  virtual void setStdin();
  virtual Stream getStream() const;
  virtual void putBack(Token::Ptr token);
  virtual Token::Ptr next();

  Mark mark();

  void accept(Mark mark);
  void reject(Mark mark);
protected:
  virtual Token::Ptr nextFromStream();  
};
