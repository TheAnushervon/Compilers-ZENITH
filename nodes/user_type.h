#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "node.h"
#include <memory>
#include <string>

class UserType : public Node {
  public:
UserType(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
      std::string result = "UserType: " + child->ToString(2);
      return result; 
    }
private:
//ArrayType | RecordType
 std::shared_ptr<Node> child;
};

#endif // USER_TYPE_H
