#include "Token.h"
#include "TokenType.h"
#include <string>

Token::Token( TokenType type, const std::string & lexeme, const std::string & literal, int line )
    : type( type ), lexeme( lexeme ), literal( literal ), line( line ) {}

std::string Token::toString() {
    return getTokenTypeName( type ) + " " + lexeme + " " + literal;
}
