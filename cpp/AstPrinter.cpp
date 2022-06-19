#include "Expr.h"
#include "Token.h"
#include "TokenType.h"

#include <initializer_list>
#include <iostream>

class AstPrinter : public Visitor< std::string > {
public:
  void print( const Expr &expr ) const {
    std::cout << expr.accept( *this ) << std::endl;
  }

  std::string visitBinaryExpr( const Binary &expr ) const override {
    return parenthesize( expr.operator_.getLexeme(), { expr.left_.get(), expr.right_.get() } );
  }

  std::string visitGroupingExpr( const Grouping &expr ) const override {
    return parenthesize( "group", { expr.expr_.get() } );
  }

  std::string visitLiteralExpr( const Literal &expr ) const override {
    return expr.literal_;
  }

  std::string visitUnaryExpr( const Unary &expr ) const override {
    return parenthesize( expr.operator_.getLexeme(), { expr.right_.get() } );
  }

private:
  std::string parenthesize( const std::string &name, std::initializer_list< const Expr * > exprs ) const {
    std::string ans = "(" + name;
    for ( const Expr *expr : exprs ) {
      ans.push_back( ' ' );
      ans += expr->accept( *this );
    }
    ans.push_back( ')' );

    return ans;
  }
};

int main() {
  AstPrinter printer;

  Binary expr{ new Unary( Token( TokenType::MINUS, "-", "", 1 ), new Literal( "123" ) ),
               Token( TokenType::STAR, "*", "", 1 ),
               new Grouping( new Literal( "45.67" ) ) };

  printer.print( expr );

  return 0;
}
