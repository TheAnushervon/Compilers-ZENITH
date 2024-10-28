#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <vector>
#include "node.h"

class Expression : public Node {
public:
    // Relation { ( and | or | xor ) Relation }
    std::vector<std::shared_ptr<Node>> relations;
    Expression(const std::vector<std::shared_ptr<Node>>& relationsList) : relations(relationsList) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Expression:\n";

        for (const auto& relation : relations) {
            result += relation->ToString(count + 2) + " ";
        }

        // Удаление лишнего пробела в конце строки, если есть элементы
        if (!relations.empty()) {
            result.pop_back();
        }

        return result;
    }
};

#endif // EXPRESSION_H
