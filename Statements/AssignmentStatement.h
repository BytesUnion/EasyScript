#pragma once
#include "Statement.h"
#include "../Expressions/Expression.h"
#include <string>
#include <memory>

class AssignmentStatement : public Statement {
private:
    std::string variableName;
    std::shared_ptr<Expression> value;

public:
    AssignmentStatement(const std::string& variableName, std::shared_ptr<Expression> value);
    void Execute(std::unordered_map<std::string, Value>& variables) override;
};