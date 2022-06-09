#include "Parser.h"

AST* Parser::Parse()
{
    AST* tree = ParseSource();
    CheckNextToken(Token::kEOI);

    return tree;
}

AST* Parser::ParseSource()
{
    Expr* expr;
    llvm::SmallVector<llvm::StringRef, 8> variables;
    if (token.Is(Token::kKeywordWith))
    {
        Advance();
        if (CheckNextToken(Token::kIdent))
        {
            goto errorTag;
        }

        variables.push_back(token.GetText());
        Advance();
        while (token.Is(Token::kComma))
        {
            Advance();
            if (CheckNextToken(Token::kIdent))
            {
                goto errorTag;
            }

            variables.push_back(token.GetText());
            Advance();
        }

        if (Consume(Token::kColon))
        {
            goto errorTag;
        }
    }

    expr = ParseExpr();
    if (CheckNextToken(Token::kEOI))
    {
        goto errorTag;
    }

    if (variables.empty())
    {
        return expr;
    }
    else
    {
        return new WithDeclaration(variables, expr);
    }

errorTag:
    while (token.GetType() != Token::kEOI)
    {
        Advance();
    }

    return nullptr;
}

Expr* Parser::ParseExpr()
{
    Expr* left = ParseTerm();
    while (token.IsOneOf(Token::kPlus, Token::kMinus))
    {
        BinaryOp::Operator op = token.Is(Token::kPlus) ? BinaryOp::kPlus : BinaryOp::kMinus;

        Advance();
        Expr* right = ParseTerm();
        left = new BinaryOp(op, left, right);
    }

    return left;
}

Expr* Parser::ParseTerm()
{
    Expr* left = ParseFactor();
    while (token.IsOneOf(Token::kStar, Token::kSlash))
    {
        BinaryOp::Operator op = token.Is(Token::kStar) ? BinaryOp::kMultiple : BinaryOp::kDivide;

        Advance();
        Expr* right = ParseFactor();
        left = new BinaryOp(op, left, right);
    }

    return left;
}

Expr* Parser::ParseFactor()
{
    Expr* expr = nullptr;
    switch (token.GetType())
    {
    case Token::kNumber:
        expr = new Factor(Factor::kNumber, token.GetText());
        Advance();
        break;

    case Token::kIdent:
        expr = new Factor(Factor::kIdent, token.GetText());
        Advance();
        break;

    case Token::kLeftParen:
        Advance();
        expr = ParseExpr();
        if (!Consume(Token::kRightParen))
        {
            break;
        }

    default:
        if (!expr)
        {
            AddError();
        }

        while (
            !token.IsOneOf(Token::kRightParen, Token::kStar, Token::kPlus, Token::kMinus, Token::kSlash, Token::kEOI))
        {
            Advance();
        }

        break;
    }

    return expr;
}
