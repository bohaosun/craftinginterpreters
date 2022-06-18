#include "Scanner.h"
#include "TokenType.h"

#include <cassert>
#include <cctype>

void error( int line, const std::string &message );

Scanner::Scanner( const std::string &source )
    : source_( source ),
      keywords_{
          { "and", TokenType::AND },
          { "class", TokenType::CLASS },
          { "else", TokenType::ELSE },
          { "false", TokenType::FALSE },
          { "for", TokenType::FOR },
          { "fun", TokenType::FUN },
          { "if", TokenType::IF },
          { "nil", TokenType::NIL },
          { "or", TokenType::OR },
          { "print", TokenType::PRINT },
          { "return", TokenType::RETURN },
          { "super", TokenType::SUPER },
          { "this", TokenType::THIS },
          { "true", TokenType::TRUE },
          { "var", TokenType::VAR },
          { "while", TokenType::WHILE } } {}

std::vector< Token > Scanner::scanTokens() {
  while ( !isAtEnd() ) {
    // We are at the beginning of the next lexeme.
    start_ = current_;
    scanToken();
  }

  assert( current_ == source_.size() );
  tokens_.emplace_back( TokenType::FILE_END, "", "null", line_ );
  return tokens_;
}

bool Scanner::isAtEnd() {
  return current_ >= source_.length();
}

void Scanner::scanToken() {
  char c = advance();
  switch ( c ) {
  case '(':
    addToken( TokenType::LEFT_PAREN );
    break;
  case ')':
    addToken( TokenType::RIGHT_PAREN );
    break;
  case '{':
    addToken( TokenType::LEFT_BRACE );
    break;
  case '}':
    addToken( TokenType::RIGHT_BRACE );
    break;
  case ',':
    addToken( TokenType::COMMA );
    break;
  case '.':
    addToken( TokenType::DOT );
    break;
  case '-':
    addToken( TokenType::MINUS );
    break;
  case '+':
    addToken( TokenType::PLUS );
    break;
  case ';':
    addToken( TokenType::SEMICOLON );
    break;
  case '*':
    addToken( TokenType::STAR );
    break;
  case '!':
    addToken( match( '=' ) ? TokenType::BANG_EQUAL : TokenType::BANG );
    break;
  case '=':
    addToken( match( '=' ) ? TokenType::EQUAL_EQUAL : TokenType::EQUAL );
    break;
  case '<':
    addToken( match( '=' ) ? TokenType::LESS_EQUAL : TokenType::LESS );
    break;
  case '>':
    addToken( match( '=' ) ? TokenType::GREATER_EQUAL : TokenType::GREATER );
    break;
  case '/':
    if ( match( '/' ) ) {
      // comment: ignote the whole line
      while ( !isAtEnd() && peek() != '\n' ) {
        advance();
      }
    } else {
      addToken( TokenType::SLASH );
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace.
    break;

  case '\n':
    // no token, only increase line number
    ++line_;
    break;
  case '"':
    handleString();
    break;
  default:
    if ( std::isdigit( c ) ) {
      handleNumber();
    } else if ( isAlpha( c ) ) { // alpha or '_'
      handleIdentifier();
    } else {
      error( line_, "Unexpected character." );
    }
  }
}

// helper functions

void Scanner::handleIdentifier() {
  while ( isAlphaNum( peek() ) )
    advance();

  std::string text = source_.substr( start_, current_ - start_ );
  if ( keywords_.find( text ) != keywords_.end() ) {
    addToken( keywords_[ text ] );
  } else {
    addToken( TokenType::IDENTIFIER );
  }
}

// the only number type in lox is float
void Scanner::handleNumber() {
  while ( std::isdigit( peek() ) )
    advance();

  bool hasDot = false;

  // if see the digital point, we need the next digit to be a number to consider it a valid number
  if ( peek() == '.' && std::isdigit( peekNext() ) ) {
    hasDot = true;
    advance(); // consume '.'

    while ( std::isdigit( peek() ) )
      advance();
  }

  std::string numberStr = source_.substr( start_, current_ - start_ );
  // to match the display format in test...
  if ( hasDot ) {
    addToken( TokenType::NUMBER, numberStr );
  } else {
    addToken( TokenType::NUMBER, numberStr + ".0" );
  }
}

void Scanner::handleString() {
  while ( !isAtEnd() && peek() != '"' ) {
    if ( peek() == '\n' )
      ++line_;
    advance();
  }

  if ( isAtEnd() ) {
    error( line_, "Unterminated string!" );
  }

  advance(); // take the right '"'

  addToken( TokenType::STRING, source_.substr( start_ + 1, current_ - start_ - 2 ) ); // the length needs to ignore double quotes
}

char Scanner::advance() {
  return source_.at( current_++ );
}

void Scanner::addToken( TokenType type ) {
  addToken( type, "null" );
}

void Scanner::addToken( TokenType type, const std::string &literal ) {
  std::string text = source_.substr( start_, current_ - start_ );
  tokens_.emplace_back( type, text, literal, line_ );
}

bool Scanner::match( char expected ) {
  if ( isAtEnd() )
    return false;
  if ( source_.at( current_ ) != expected )
    return false;

  ++current_;
  return true;
}

char Scanner::peek() {
  if ( isAtEnd() )
    return '\0';
  return source_.at( current_ );
}

char Scanner::peekNext() {
  if ( current_ + 1 >= source_.length() )
    return '\0';
  return source_.at( current_ + 1 );
}
