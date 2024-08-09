#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Process/Value.h"

class Expression {
public:
    virtual ~Expression() = default;
    virtual Value Evaluate(std::unordered_map<std::string, Value>& variables) = 0;
};