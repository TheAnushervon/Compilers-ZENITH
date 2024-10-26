#ifndef SIMPLEDECLARATION_H
#define SIMPLEDECLARATION_H

#include <string>
#include <memory>
#include "node.h"

class SimpleDeclaration : public Node {
public:

SimpleDeclaration(std::shared_ptr<Node> childNode) : child(childNode) {}

   std::string ToString(int counter) const override {
       std::string y = "SimpleDeclaration: " + child->ToString(2);
       return y;
   }

private:
std::shared_ptr<Node> child;

};

#endif // SIMPLEDECLARATION_H
