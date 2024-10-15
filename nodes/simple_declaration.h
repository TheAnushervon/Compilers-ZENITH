#ifndef SIMPLEDECLARATION_H
#define SIMPLEDECLARATION_H

#include <string>
#include <iostream>
#include "statement.h"

class SimpleDeclaration : public Statement {
public:
    virtual ~SimpleDeclaration() = default;
    virtual std::string ToString(int counter) const override = 0;

    // virtual void Print() const override {
    //     std::cout << "SimpleDeclaration: " << ToString(counter) << std::endl;
    // }

};

#endif // SIMPLEDECLARATION_H
