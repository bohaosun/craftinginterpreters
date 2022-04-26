#include "Scanner.h"
#include "Token.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

bool hadError = false;

void report( int line, std::string where, const std::string &message ) {
  std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
  hadError = true;
}

void error( int line, const std::string &message ) {
  report( line, "", message );
}

void run( const std::string source ) {
  Scanner scanner( source );
  std::vector<Token> tokens = scanner.scanTokens();
  for ( Token token : tokens ) {
    std::cout << token.toString() << std::endl;
  }
}

void runFile( const std::string &path ) {
  // run the whole file
  std::ifstream file( path );
  std::stringstream buffer;
  buffer << file.rdbuf();
  run( buffer.str() );

  if ( hadError ) {
    exit( 65 );
  }
}

void runPrompt() {
  // run line by line
  for ( ;; ) {
    std::cout << "> ";
    std::string line;
    std::getline( std::cin, line );
    run( line );
    hadError = false;
  }
}

int main( int argc, char *argv[] ) {
  if ( argc > 2 ) {
    std::cerr << "Useage: cpplox [script]" << std::endl;
    return 64;
  } else if ( argc == 2 ) {
    runFile( argv[ 1 ] );
  } else {
    runPrompt();
  }

  return 0;
}
