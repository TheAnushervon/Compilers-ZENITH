#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <iostream>
#include "node.h"

class Statement : public Node {
public:
    virtual ~Statement() = default;

    virtual std::string ToString(int counter) const override = 0;

    virtual void Print() const {
        std::cout << "Statement: " << ToString(2) << std::endl;
    }
};

#endif // STATEMENT_H
