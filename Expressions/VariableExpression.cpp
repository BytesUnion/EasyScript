#include "VariableExpression.h"
#include <stdexcept>

VariableExpression::VariableExpression(const std::string& name) : name(name) {}

Value VariableExpression::Evaluate(std::unordered_map<std::string, Value>& variables) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        throw std::runtime_error("Variable not found: " + name);
    }
    return it->second;
}