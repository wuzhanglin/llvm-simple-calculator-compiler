#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

class Token
{
public:

    enum TokenType
    {
        kEOI,
        kUnknown,
        kIdent,
        kNumber,
        kComma,
        kColon,
        kPlus,
        kMinus,
        kStar,
        kSlash,
        kLeftParen,
        kRightParen,
        kKeywordWith
    };

public:

    TokenType GetType() const
    {
        return type;
    }

    void SetType(TokenType inType)
    {
        type = inType;
    }

    llvm::StringRef GetText() const
    {
        return text;
    }

    void SetText(llvm::StringRef inText)
    {
        text = inText;
    }

    bool Is(TokenType inType) const
    {
        return type == inType;
    }

    bool IsOneOf(TokenType type1, TokenType type2) const
    {
        return Is(type1) || Is(type2);
    }

    template<typename... Ts> bool IsOneOf(TokenType type1, TokenType type2, Ts... types) const
    {
        return Is(type1) || IsOneOf(type2, types...);
    }

private:

    TokenType type;
    llvm::StringRef text;
};

class Lexer
{
public:

    Lexer(const llvm::StringRef& Buffer)
    {
        bufferStart = Buffer.begin();
        bufferPtr = bufferStart;
    }

    void GetNext(Token& token);

private:

    void InitializeToken(Token& token, const char* tokenEnd, Token::TokenType type);

private:

    const char* bufferStart;
    const char* bufferPtr;
};
