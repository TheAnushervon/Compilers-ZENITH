#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "node.h"
#include <iostream>
#include <string>

class Identifier : public Node {
  public:
    explicit Identifier(const std::string &name) : name(name) {}
    std::string GetName() const { return name; }

    std::string ToString(int counter) const override { return name; }
    void Print() const { std::cout << "Identifier: " << name << std::endl; }

  private:
    std::string name;
};

#endif // IDENTIFIER_H
