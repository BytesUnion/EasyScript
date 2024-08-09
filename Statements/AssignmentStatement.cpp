#include "AssignmentStatement.h"

AssignmentStatement::AssignmentStatement(const std::string& variableName, std::shared_ptr<Expression> value)
    : variableName(variableName), value(value) {}

void AssignmentStatement::Execute(std::unordered_map<std::string, Value>& variables) {
    variables[variableName] = value->Evaluate(variables);
}