#ifndef BODY_H
#define BODY_H

#include "node.h"
#include <memory>
#include <vector>

class Body : public Node {
  public:
    Body(std::shared_ptr<Node> childNode) : child(childNode) {}
    std::string ToString(int counter) const override {
        std::string result = "Body: " + child->ToString(counter);

        return result;
    }

  private:
// { SimpleDeclaration | Statement } 
   std::shared_ptr<Node> child;
};

#endif // BODY_H
