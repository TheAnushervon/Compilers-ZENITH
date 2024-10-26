#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "node.h"
#include <string>

class Identifier : public Node {
  public:
     Identifier(const std::string &name) : name(name) {}

    std::string ToString(int counter) const override {
      std::string result = "Identifiert: " + name;
      return result; 
    }

  private:
    std::string name;
};

#endif // IDENTIFIER_H
