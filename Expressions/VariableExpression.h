#pragma once
#include "Expression.h"
#include <string>

class VariableExpression : public Expression {
private:
    std::string name;

public:
    VariableExpression(const std::string& name);
    Value Evaluate(std::unordered_map<std::string, Value>& variables) override;
};