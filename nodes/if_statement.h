#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include "node.h"
#include <memory>

class IfStatement : public Node {
  public:
   IfStatement(std::shared_ptr<Node> expression, std::shared_ptr<Node> thenBodyNode, std::shared_ptr<Node> elseBodyNode = nullptr)
   : ifExpression(expression), thenBody(thenBodyNode), elseBody(elseBodyNode) {}

    std::string ToString(int counter) const override {
       std::string result = "IfStatement: " + thenBody->ToString(counter);
        if (elseBody != nullptr) {
            result += elseBody->ToString(counter);
        } 
        return result;
    }

  private:
//if Expression then Body [ else Body ] end 
    std::shared_ptr<Node> ifExpression;
    std::shared_ptr<Node> thenBody;
    std::shared_ptr<Node> elseBody;
};

#endif // IFSTATEMENT_H
