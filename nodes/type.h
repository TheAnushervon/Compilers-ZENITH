#ifndef TYPE_H
#define TYPE_H

#include "node.h"
#include <memory>
#include <string>

class Type : public Node {
  public:
  //PrimitiveType | UserType | Identifier 
   std::shared_ptr<Node> child;

  Type(std::shared_ptr<Node> childNode) : child(childNode) {}
    std::string ToString(int counter) const override {
      std::string result = "Type: " + child->ToString(2);
      return result; 
    }

};

#endif // TYPE_H
