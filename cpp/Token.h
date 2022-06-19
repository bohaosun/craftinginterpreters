#pragma once

#include "TokenType.h"

#include <iostream>
#include <string>

class Token {

  friend std::ostream &operator<<( std::ostream &os, const Token &token );

private:
  TokenType   type_;
  std::string lexeme_;
  std::string literal_; // use string for now, may need to change later
  int         line_;

public:
  Token( TokenType type, const std::string &lexeme, const std::string &literal, int line );
  std::string getLexeme() const { return lexeme_; }
};
