#include "Interpreter.h"
#include "../Expressions/Expression.h"
#include "../Expressions/ConstantExpression.h"
#include "../Expressions/VariableExpression.h"
#include "../Statements/PrintStatement.h"
#include "../Statements/AssignmentStatement.h"
#include <stdexcept>
#include <iostream>

Interpreter::Interpreter(const std::string& filename) : currentFile(filename), currentTokenIndex(0) {}

void Interpreter::Execute(const std::string& script) {
    try {
        Lexer lexer(script);
        tokens = lexer.TokenizeAll();
        currentTokenIndex = 0;

        std::vector<std::shared_ptr<Statement>> statements = Parse();

        for (const auto& statement : statements) {
            statement->Execute(variables);
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

std::vector<std::shared_ptr<Statement>> Interpreter::Parse() {
    std::vector<std::shared_ptr<Statement>> statements;

    while (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].type != TokenType::EOF_) {
        auto statement = ParseStatement();
        if (statement == nullptr) {
            break;
        }
        statements.push_back(statement);
    }

    return statements;
}

std::shared_ptr<Statement> Interpreter::ParseStatement() {
    while (currentTokenIndex < tokens.size() &&
        (tokens[currentTokenIndex].type == TokenType::Comment || tokens[currentTokenIndex].value.empty())) {
        currentTokenIndex++;
    }

    if (currentTokenIndex >= tokens.size()) {
        return nullptr;
    }

    if (Match(TokenType::Keyword, "print")) {
        return ParsePrintStatement();
    }
    else if (Match(TokenType::Identifier)) {
        return ParseAssignmentStatement();
    }

    throw std::runtime_error("Unexpected token: " + tokens[currentTokenIndex].value);
}

std::shared_ptr<Statement> Interpreter::ParsePrintStatement() {
    Expect(TokenType::Delimiter, "(");
    auto expr = ParseExpression();
    Expect(TokenType::Delimiter, ")");
    return std::make_shared<PrintStatement>(expr);
}

std::shared_ptr<Statement> Interpreter::ParseAssignmentStatement() {
    std::string variableName = tokens[currentTokenIndex - 1].value;
    Expect(TokenType::Operator, "=");
    auto value = ParseExpression();
    return std::make_shared<AssignmentStatement>(variableName, value);
}

std::shared_ptr<Expression> Interpreter::ParseExpression() {
    if (Match(TokenType::Number)) {
        return std::make_shared<ConstantExpression>(Value(std::stod(tokens[currentTokenIndex - 1].value)));
    }
    else if (Match(TokenType::String)) {
        return std::make_shared<ConstantExpression>(Value(tokens[currentTokenIndex - 1].value));
    }
    else if (Match(TokenType::Identifier)) {
        return std::make_shared<VariableExpression>(tokens[currentTokenIndex - 1].value);
    }
    throw std::runtime_error("Unexpected token in expression: " + tokens[currentTokenIndex].value);
}

bool Interpreter::Match(TokenType type) {
    if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].type == type) {
        currentTokenIndex++;
        return true;
    }
    return false;
}

bool Interpreter::Match(TokenType type, const std::string& value) {
    if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].type == type && tokens[currentTokenIndex].value == value) {
        currentTokenIndex++;
        return true;
    }
    return false;
}

void Interpreter::Expect(TokenType type, const std::string& value) {
    if (!Match(type, value)) {
        throw std::runtime_error("Expected " + value + ", but found " + tokens[currentTokenIndex].value);
    }
}