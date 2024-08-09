#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Lexer.h"
#include "Value.h"
#include "../Expressions/Expression.h"

class Statement;

class Interpreter {
private:
    std::string currentFile;
    std::vector<Token> tokens;
    size_t currentTokenIndex;
    std::unordered_map<std::string, Value> variables;

    std::vector<std::shared_ptr<Statement>> Parse();
    std::shared_ptr<Statement> ParseStatement();
    std::shared_ptr<Statement> ParsePrintStatement();
    std::shared_ptr<Statement> ParseAssignmentStatement();
    std::shared_ptr<Expression> ParseExpression();

    bool Match(TokenType type);
    bool Match(TokenType type, const std::string& value);
    void Expect(TokenType type, const std::string& value);

public:
    Interpreter(const std::string& filename);
    void Execute(const std::string& script);
};