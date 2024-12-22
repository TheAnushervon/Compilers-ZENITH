#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include "node.h"
#include <memory>
#include <iostream>

class IfStatement : public Node {
public:
    // if Expression then Body [ else Body ] end 
    std::shared_ptr<Node> ifExpression;
    std::shared_ptr<Node> thenBody;
    std::shared_ptr<Node> elseBody;

    IfStatement(std::shared_ptr<Node> expression, std::shared_ptr<Node> thenBodyNode, std::shared_ptr<Node> elseBodyNode = nullptr)
        : ifExpression(expression), thenBody(thenBodyNode), elseBody(elseBodyNode) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result =  "IfStatement:\n";
        result += ifExpression->ToString(counter+2);
        

        result += thenBody->ToString(counter + 2);
        if (elseBody != nullptr) {
            result += elseBody->ToString(counter + 2);
        }

        return ots + result;
    }
};

#endif // IFSTATEMENT_H
