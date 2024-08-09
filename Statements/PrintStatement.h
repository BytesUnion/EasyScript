#pragma once
#include "Statement.h"
#include "../Expressions/Expression.h"
#include <memory>

class PrintStatement : public Statement {
private:
    std::shared_ptr<Expression> expr;

public:
    PrintStatement(std::shared_ptr<Expression> expr);
    void Execute(std::unordered_map<std::string, Value>& variables) override;
};