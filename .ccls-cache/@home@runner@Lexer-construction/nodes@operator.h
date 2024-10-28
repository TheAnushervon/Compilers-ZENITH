
#ifndef OPERATOR_H
#define OPERATOR_H

#include "node.h"
#include <iostream>
#include <string>

class Operator : public Node {
  public:
    explicit Operator(const std::string &name) : name(name) {}
    std::string GetName() const { return name; }

    std::string ToString(int counter) const override { return name; }
    void Print() const { std::cout << "Operator: " << name << std::endl; }

  private:
    std::string name;
};

#endif // IDENTIFIER_H
