#pragma once

#include "Token.h"

#include <cctype>
#include <map>
#include <vector>

class Scanner {
public:
    Scanner( const std::string & source );
    std::vector< Token > scanTokens();

private:
    bool isAtEnd();
    void scanToken();
    char advance();

    void handleIdentifier();
    void handleNumber();
    void handleString();

    void addToken( TokenType type );
    void addToken( TokenType type, const std::string & literal );

    bool match( char expected );
    char peek();
    char peekNext();
    
    bool isAlpha( char c ) { return std::isalpha( c ) || c == '_'; }
    bool isAlphaNum( char c ) { return isAlpha( c ) || std::isdigit( c ); }

    std::string source_;
    std::vector< Token > tokens_;
    int start_ = 0;
    int current_ = 0;
    int line_ = 1;
    std::map< std::string, TokenType > keywords_;
};
