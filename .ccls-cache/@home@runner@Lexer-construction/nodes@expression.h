#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <iostream>
#include "node.h"

class Expression : public Node{
public:
    virtual ~Expression() = default;

    virtual std::string ToString() const override = 0;

    virtual void Print() const {
        std::cout << "Expression: " << ToString() << std::endl;
    }
};

#endif // EXPRESSION_H
