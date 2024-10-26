#ifndef TYPE_H
#define TYPE_H

#include "node.h"
#include <memory>
#include <string>

class Type : public Node {
  public:

Type(std::shared_ptr<Node> childNode) : child(childNode) {}
    std::string ToString(int counter) const override {
      std::string result = "Type: " + child->ToString(2);
      return result; 
    }
private:
//PrimitiveType | UserType | Identifier 
 std::shared_ptr<Node> child;
};

#endif // TYPE_H
