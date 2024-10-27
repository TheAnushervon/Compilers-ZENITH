#ifndef PRIMITIVETYPE_H
#define PRIMITIVETYPE_H

#include <memory>
#include "node.h"

class PrimitiveType : public Node {
  public:
  // int, real, bool
  std::string name;
  PrimitiveType(const std::string& typeName) : name(typeName) {}

std::string ToString(int counter) const override {
  std::string result = "PrimitiveType: " + name;
  return result; 
}

};


#endif // PRIMITIVETYPE_H
