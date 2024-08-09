#include "PrintStatement.h"
#include <iostream>

PrintStatement::PrintStatement(std::shared_ptr<Expression> expr) : expr(expr) {}

void PrintStatement::Execute(std::unordered_map<std::string, Value>& variables) {
    Value result = expr->Evaluate(variables);
    std::cout << result.ToString() << std::endl;
}