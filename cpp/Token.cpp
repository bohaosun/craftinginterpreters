#include "Token.h"
#include "TokenType.h"
#include <string>

Token::Token( TokenType type, const std::string &lexeme, const std::string &literal, int line )
    : type_( type ), lexeme_( lexeme ), literal_( literal ), line_( line ) {}

std::ostream &operator<<( std::ostream &os, const Token &token ) {
  os << getTokenTypeName( token.type_ ) << ' ' << token.lexeme_ << ' ' << token.literal_;
  return os;
}
