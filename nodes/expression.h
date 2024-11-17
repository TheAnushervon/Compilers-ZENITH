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
    std::vector<std::string> operators; // Добавили вектор операторов

    Expression(const std::vector<std::shared_ptr<Node>>& relationsList, const std::vector<std::string>& ops)
        : relations(relationsList), operators(ops) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Expression:\n";

        // Итерируемся по relations и operators одновременно
        for (size_t i = 0; i < relations.size(); ++i) {
            result += relations[i]->ToString(count + 2);

            // Добавляем оператор, если он есть (не забываем, что операторов на 1 меньше, чем relations)
            if (i < operators.size()) {
                result += "\n" + std::string((count + 2) * 2, ' ') + "Operator: " + operators[i] + "\n";
            }
        }

        return result;
    }
};

#endif // EXPRESSION_H
