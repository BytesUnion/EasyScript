#include "ConstantExpression.h"
#include <iostream>

ConstantExpression::ConstantExpression(const Value& val) : value(val) {}

Value ConstantExpression::Evaluate(std::unordered_map<std::string, Value>& variables) {
    return value;
}
