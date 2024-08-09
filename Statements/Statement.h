#pragma once
#include <unordered_map>
#include "../Process/Value.h"

class Statement {
public:
    virtual ~Statement() = default;
    virtual void Execute(std::unordered_map<std::string, Value>& variables) = 0;
};