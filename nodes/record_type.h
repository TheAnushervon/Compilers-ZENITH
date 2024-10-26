#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <vector>
#include <memory>
#include "node.h"

class RecordType : public Node {
public:
     RecordType(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
        std::string result = "record: ";
        result += child->ToString(2);
        return result;
    }

private:
    //VariableDeclaration
    std::shared_ptr<Node> child; 
};

#endif // RECORDTYPE_H
