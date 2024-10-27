#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "node.h"
#include <string>

class Identifier : public Node {
  public:
    std::string name;

    Identifier(const std::string &name) : name(name) {}

    std::string ToString(int counter) const override {
      std::string result = "Identifiert: " + name;
      return result; 
    }

};

#endif // IDENTIFIER_H
