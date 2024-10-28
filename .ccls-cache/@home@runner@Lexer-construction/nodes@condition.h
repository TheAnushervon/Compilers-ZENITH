

#ifndef CONDITION_H
#define CONDITION_H

#include "node.h"
#include <iostream>
#include <string>

class Condition : public Node {
  public:
    explicit Condition(const std::string &name) : name(name) {}
    std::string GetName() const { return name; }

    std::string ToString(int counter) const override { return name; }
    void Print() const { std::cout << "Operator: " << name << std::endl; }

  private:
    std::string name;
};

#endif // IDENTIFIER_H
