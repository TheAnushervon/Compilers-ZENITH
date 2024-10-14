#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <string>
#include <iostream>
#include "node.h"

class Identifier : public Node {
public:
    explicit Identifier(const std::string& name) : name(name) {}
    std::string GetName() const {
        return name;
    }

    std::string ToString() const override {
        return name;
    }
    void Print() const {
        std::cout << "Identifier: " << name << std::endl;
    }

private:
    std::string name; 
};

#endif // IDENTIFIER_H
