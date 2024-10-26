#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "node.h"
#include <memory>

class Assignment : public Node {
  public:

Assignment(std::shared_ptr<Node> modifiablePrim, std::shared_ptr<Node> expr)
    : modifiablePrimary(modifiablePrim), expression(expr) {}

    std::string ToString(int counter) const override {
        std::string result;
        result = "Assignment: " + modifiablePrimary->ToString(counter) + expression->ToString(counter);

        return result;
    }

  private:
//: ModifiablePrimary := Expression 
    std::shared_ptr<Node> modifiablePrimary;
    std::shared_ptr<Node> expression;
};

#endif // ASSIGNMENT_H
