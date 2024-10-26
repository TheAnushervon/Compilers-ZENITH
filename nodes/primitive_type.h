#ifndef PRIMITIVETYPE_H
#define PRIMITIVETYPE_H

#include <memory>
#include "node.h"

class PrimitiveType : public Node {
  public:
PrimitiveType(const std::string& typeName) : name(typeName) {}

std::string ToString(int counter) const override {
  std::string result = "PrimitiveType: " + name;
  return result; 
}

private:
// int, real, bool
std::string name;
};


#endif // PRIMITIVETYPE_H
