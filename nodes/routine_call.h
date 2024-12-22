#ifndef ROUTINECALL_H
#define ROUTINECALL_H

#include <vector>
#include <memory>
#include "statement.h"
#include "expression.h"
#include "identifier.h"

class RoutineCall : public Statement {
public:
    RoutineCall(std::shared_ptr<Identifier> routineName, const std::vector<std::shared_ptr<Expression>>& arguments)
        : routineName(routineName), arguments(arguments) {}

    std::string ToString() const override {
        std::string result = routineName->ToString() + "(";
        for (size_t i = 0; i < arguments.size(); ++i) {
            result += arguments[i]->ToString();
            if (i < arguments.size() - 1) result += ", ";
        }
        result += ")";
        return result;
    }

private:
    std::shared_ptr<Identifier> routineName;              
    std::vector<std::shared_ptr<Expression>> arguments;   
};

#endif // ROUTINECALL_H
