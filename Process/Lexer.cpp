#include "Lexer.h"
#include "TokenType.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <stdexcept>
#include <cctype>
#include <sstream>

using namespace std;

Token::Token(TokenType type, string value, int line, int column)
    : type(type), value(move(value)), line(line), column(column) {}

std::string Token::ToString() const {
    std::ostringstream oss;
    oss << "\033[1;34mToken\033[0m("
        << "\033[1;32m" << static_cast<int>(type) << "\033[0m, '"
        << "\033[1;35m" << value << "\033[0m', line: \033[1;36m" << line
        << "\033[0m, col: \033[1;36m" << column << "\033[0m)";
    return oss.str();
}

const unordered_set<string> Lexer::Keywords = {
    "print", "if", "elif", "else", "endif", "endf", "True", "False", "f", "return",
    "for", "to", "do", "endfor", "while", "endwhile", "in", "break",
    "class", "endclass", "init", "endinit", "extends", "super", "from", "use", "share", "load", "as",
    "null", "new"
};

const unordered_set<string> Lexer::Operators = {
    "+", "-", "*", "/", "=", "==", "!=", ">", "<", ">=", "<=", "&&", "||",
    "**", "//", "%"
};

const unordered_set<char> Lexer::Delimeters = {
    '(', ')', '[', ']', '{', '}', ',', ':', '.'
};

Lexer::Lexer(const string& input)
    : input(input), position(0), line(1), column(1) {}

std::vector<Token> Lexer::TokenizeAll()
{
    std::vector<Token> tokens;
    Token token = GetNextToken();
    while (token.type != TokenType::EOF_)
    {
        tokens.push_back(token);
        token = GetNextToken();
    }
    tokens.push_back(token);
    return tokens;
}

Token Lexer::GetNextToken() {
    if (position >= input.length()) return Token(TokenType::EOF_, "", line, column);
    char currentChar = input[position];

    if (isspace(currentChar)) {
        SkipWhitespace();
        return GetNextToken();
    }

    if (currentChar == '>' && position + 1 < input.length() && input[position + 1] == '>') {
        return LexComment();
    }

    if (currentChar == '"' || currentChar == '\'') {
        return LexString();
    }

    if (isdigit(currentChar)) {
        return LexNumber();
    }

    if (isalpha(currentChar) || currentChar == '_') {
        return LexIdentifierOrKeyword();
    }

    if (IsOperatorChar(currentChar)) {
        return LexOperator();
    }

    if (Delimeters.find(currentChar) != Delimeters.end()) {
        return LexDelimiter();
    }

    throw std::runtime_error("шо за фігня тут: " + std::string(1, currentChar) + " рядок " + std::to_string(line) + ", стовпець " + std::to_string(column));
}

Token Lexer::LexComment()
{
    int startColumn = column;
    std::ostringstream oss;
    while (position < input.length() && input[position] != '\n')
    {
        oss << input[position];
        ++position;
        ++column;
    }
    return Token(TokenType::Comment, oss.str(), line, startColumn);
}

Token Lexer::LexString()
{
    int startColumn = column;
    char quote = input[position];
    std::ostringstream oss;
    ++position;
    ++column;

    while (position < input.length() && input[position] != quote)
    {
        oss << input[position];
        ++position;
        ++column;
    }

    if (position >= input.length())
    {
        throw std::runtime_error("шо за фігня тут " + std::to_string(line) + ", стовпець " + std::to_string(startColumn));
    }

    ++position;
    ++column;

    return Token(TokenType::String, oss.str(), line, startColumn);
}

Token Lexer::LexNumber()
{
    int startColumn = column;
    std::ostringstream oss;
    bool hasDecimalPoint = false;

    while (position < input.length() &&
        (std::isdigit(input[position]) || (input[position] == '.' && !hasDecimalPoint)))
    {
        if (input[position] == '.')
        {
            hasDecimalPoint = true;
        }
        oss << input[position];
        ++position;
        ++column;
    }

    return Token(TokenType::Number, oss.str(), line, startColumn);
}

Token Lexer::LexIdentifierOrKeyword()
{
    int startColumn = column;
    std::ostringstream oss;

    while (position < input.length() &&
        (std::isalnum(input[position]) || input[position] == '_'))
    {
        oss << input[position];
        ++position;
        ++column;
    }

    std::string value = oss.str();
    TokenType type = Keywords.find(value) != Keywords.end() ? TokenType::Keyword : TokenType::Identifier;

    return Token(type, value, line, startColumn);
}

Token Lexer::LexOperator()
{
    int startColumn = column;
    std::ostringstream oss;

    while (position < input.length() && IsOperatorChar(input[position]))
    {
        oss << input[position];
        ++position;
        ++column;

        std::string value = oss.str();
        if (Operators.find(value) != Operators.end())
        {
            if (position < input.length() && Operators.find(value + input[position]) != Operators.end())
            {
                continue;
            }
            break;
        }
    }

    std::string value = oss.str();
    if (Operators.find(value) == Operators.end())
    {
        throw std::runtime_error("шо за фігня " + value + " рядок " + std::to_string(line) + ", стовпець " + std::to_string(startColumn));
    }

    return Token(TokenType::Operator, value, line, startColumn);
}

Token Lexer::LexDelimiter()
{
    int startColumn = column;
    char delimiter = input[position];
    ++position;
    ++column;
    return Token(TokenType::Delimiter, std::string(1, delimiter), line, startColumn);
}

void Lexer::SkipWhitespace()
{
    while (position < input.length() && std::isspace(input[position]))
    {
        if (input[position] == '\n')
        {
            ++line;
            column = 1;
        }
        else
        {
            ++column;
        }
        ++position;
    }
}

bool Lexer::IsOperatorChar(char c) const
{
    return std::string("+-*/=!><&|%").find(c) != std::string::npos;
}