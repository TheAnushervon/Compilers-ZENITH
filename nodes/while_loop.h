#ifndef WHILELOOP_H
#define WHILELOOP_H

#include "node.h"
#include <memory>

class WhileLoop : public Node {
  public:
 //while Expression loop Body end
  std::shared_ptr<Node> expression;
  std::shared_ptr<Node> body;

  WhileLoop(std::shared_ptr<Node> expr, std::shared_ptr<Node> bodyNode)
  : expression(expr), body(bodyNode) {}

    std::string ToString(int counter) const override {
        std::string result = "WhileLoop: " +expression->ToString(counter) + " " + body->ToString(counter);
        return result;
    }
};

#endif // WHILELOOP_H
