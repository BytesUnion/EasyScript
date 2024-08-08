#ifndef LEXER_H
#define LEXER_H

#include "TokenType.h"
#include <string>
#include <unordered_set>
#include <vector>

class Token {
public:
	Token(TokenType type, std::string value, int line, int column);
	std::string ToString() const;

	TokenType type;
	std::string value;
	int line;
	int column;
};


class Lexer {
private:
	std::string input;
	size_t position;
	int line;
	int column;

	static const std::unordered_set<std::string> Keywords;
	static const std::unordered_set<std::string> Operators;
	static const std::unordered_set<char> Delimeters;

public:
	Lexer(const std::string& input);

	std::vector<Token> TokenizeAll();

private:
	Token GetNextToken();
	Token LexComment();
	Token LexString();
	Token LexNumber();
	Token LexIdentifierOrKeyword();
	Token LexOperator();
	Token LexDelimiter();
	void SkipWhitespace();
	bool IsOperatorChar(char c) const;
};

#endif