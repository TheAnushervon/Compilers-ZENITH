#ifndef SIMPLEDECLARATION_H
#define SIMPLEDECLARATION_H

#include <string>
#include <iostream>
#include "statement.h"

class SimpleDeclaration : public Statement {
public:
    virtual ~SimpleDeclaration() = default;
    virtual std::string ToString() const override = 0;

    virtual void Print() const override {
        std::cout << "SimpleDeclaration: " << ToString() << std::endl;
    }
};

#endif // SIMPLEDECLARATION_H
