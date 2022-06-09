#include "Lexer.h"

#include <cctype>

void Lexer::GetNext(Token& token)
{
    while (*bufferPtr && std::isspace(*bufferPtr))
    {
        ++bufferPtr;
    }

    if (!*bufferPtr)
    {
        token.SetType(Token::kEOI);
        return;
    }

    if (std::isalpha(*bufferPtr))
    {
        const char* end = bufferPtr + 1;
        while (std::isalpha(*end))
        {
            ++end;
        }

        llvm::StringRef name(bufferPtr, end - bufferPtr);
        Token::TokenType type = (name == "with" ? Token::kKeywordWith : Token::kIdent);
        InitializeToken(token, end, type);
        return;
    }
    else if (std::isdigit(*bufferPtr))
    {
        const char* end = bufferPtr + 1;
        while (std::isdigit(*end))
        {
            ++end;
        }

        InitializeToken(token, end, Token::kNumber);
        return;
    }
    else
    {
        switch (*bufferPtr)
        {
        case '+':
            InitializeToken(token, bufferPtr + 1, Token::kPlus);
            break;

        case '-':
            InitializeToken(token, bufferPtr + 1, Token::kMinus);
            break;

        case '*':
            InitializeToken(token, bufferPtr + 1, Token::kStar);
            break;

        case '/':
            InitializeToken(token, bufferPtr + 1, Token::kSlash);
            break;

        case '(':
            InitializeToken(token, bufferPtr + 1, Token::kLeftParen);
            break;

        case ')':
            InitializeToken(token, bufferPtr + 1, Token::kRightParen);
            break;

        case ':':
            InitializeToken(token, bufferPtr + 1, Token::kColon);
            break;

        case ',':
            InitializeToken(token, bufferPtr + 1, Token::kComma);
            break;

        default:
            InitializeToken(token, bufferPtr + 1, Token::kUnknown);
        }

        return;
    }
}

void Lexer::InitializeToken(Token& token, const char* tokenEnd, Token::TokenType type)
{
    token.SetType(type);
    token.SetText(llvm::StringRef(bufferPtr, tokenEnd - bufferPtr));

    bufferPtr = tokenEnd;
}
