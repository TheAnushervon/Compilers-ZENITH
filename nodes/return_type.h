#ifndef RETURNTYPE_H
#define RETURNTYPE_H

#include <memory>
#include <string>
#include "node.h"

class ReturnType : public Node {
public:
    std::shared_ptr<Node> type;

    ReturnType(std::shared_ptr<Node> type) : type(type) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        return ots + "ReturnType:\n" + type->ToString(counter + 2);
    }
};

#endif // RETURNTYPE_H
