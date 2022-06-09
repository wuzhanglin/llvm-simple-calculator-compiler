#pragma once

#include "AST.h"
#include "Lexer.h"

#include "llvm/Support/raw_ostream.h"

class Parser
{
public:

    Parser(Lexer& lexer) :
        lexer(lexer),
        hasError(false)
    {
        Advance();
    }

    AST* Parse();

    bool HasError()
    {
        return hasError;
    }

private:

    AST* ParseSource();
    Expr* ParseExpr();
    Expr* ParseTerm();
    Expr* ParseFactor();

    void AddError()
    {
        llvm::errs() << "Unexpected: " << token.GetText() << "\n";
        hasError = true;
    }

    void Advance()
    {
        lexer.GetNext(token);
    }

    bool CheckNextToken(Token::TokenType expectedTokenType)
    {
        if (!token.Is(expectedTokenType))
        {
            AddError();
            return true;
        }

        return false;
    }

    bool Consume(Token::TokenType tokenType)
    {
        if (CheckNextToken(tokenType))
        {
            return true;
        }

        Advance();
        return false;
    }

private:

    Lexer& lexer;
    Token token;
    bool hasError;
};
