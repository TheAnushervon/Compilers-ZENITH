#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "node.h"
#include <string>

class Integer : public Node {
  public:
  Integer(const std::string &name) : name(name) {}

    std::string ToString(int counter) const override {
      std::string result = "integer";
      return result; 
    }

  private:
    std::string name;
};

#endif // IDENTIFIER_H
