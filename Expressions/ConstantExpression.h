#pragma once
#include "Expression.h"
#include "../Process/Value.h"

class ConstantExpression : public Expression {
private:
    Value value;

public:
    ConstantExpression(const Value& val);

    Value Evaluate(std::unordered_map<std::string, Value>& variables) override;
};
