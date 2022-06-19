#pragma once

#include "Token.h"
#include <memory>
#include <string>

class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;

class AstPrinter;

/*
 * TODO: potential improvements:
 * 1. Now for each T, we need to add a method accept in all subclasses of Expr
 * 2. Now for each new visitor, we need to add it as friend in all subclasses of Expr
 */

template < typename T >
class Visitor {
public:
  virtual T visitBinaryExpr( const Binary &expr ) const     = 0;
  virtual T visitGroupingExpr( const Grouping &expr ) const = 0;
  virtual T visitLiteralExpr( const Literal &expr ) const   = 0;
  virtual T visitUnaryExpr( const Unary &expr ) const       = 0;
};

class Expr {
public:
  virtual std::string accept( const Visitor< std::string > &visitor ) const = 0;
};

class Binary : public Expr {
  friend class AstPrinter;

public:
  Binary( Expr *left, Token token, Expr *right )
      : left_( left ),
        operator_( token ),
        right_( right ) {}

  std::string accept( const Visitor< std::string > &visitor ) const override {
    return visitor.visitBinaryExpr( *this );
  }

private:
  std::unique_ptr< Expr > left_;
  Token                   operator_;
  std::unique_ptr< Expr > right_;
};

class Grouping : public Expr {
  friend class AstPrinter;

public:
  Grouping( Expr *expr ) : expr_( expr ) {}

  std::string accept( const Visitor< std::string > &visitor ) const override {
    return visitor.visitGroupingExpr( *this );
  }

private:
  std::unique_ptr< Expr > expr_;
};

class Literal : public Expr {
  friend class AstPrinter;

public:
  Literal( const std::string &literal ) : literal_( literal ) {}

  std::string accept( const Visitor< std::string > &visitor ) const override {
    return visitor.visitLiteralExpr( *this );
  }

private:
  std::string literal_; // TODO: maybe another type, like token?
};

class Unary : public Expr {
  friend class AstPrinter;

public:
  Unary( const Token &token, Expr *right ) : operator_( token ), right_( right ) {}

  std::string accept( const Visitor< std::string > &visitor ) const override {
    return visitor.visitUnaryExpr( *this );
  }

private:
  Token                   operator_;
  std::unique_ptr< Expr > right_;
};
