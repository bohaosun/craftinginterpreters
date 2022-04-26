#pragma once

#include "TokenType.h"

#include <string>

class Token {
private:
  TokenType type;
  std::string lexeme;
  std::string literal; // use string for now, may need to change later
  int line;

public:
  Token( TokenType type, const std::string &lexeme, const std::string &literal, int line );

  std::string toString();
  // TODO: add ostream support, and remove toString
};
