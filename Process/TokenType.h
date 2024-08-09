#ifndef TOKENTYPE_H
#define TOKENTYPE_H

enum class TokenType
{
    Keyword,
    Identifier,
    Number,
    String,
    Operator,
    Delimiter,
    Comment,
    EOF_
};

#endif