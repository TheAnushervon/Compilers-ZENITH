#ifndef FACTOR_H
#define FACTOR_H

#include <vector>
#include <memory>
#include <string> // Добавлено для std::string
#include "node.h"

class Factor : public Node {
public:
    // Summand { ( + | - ) Summand }
    std::vector<std::shared_ptr<Node>> summands;
    std::vector<std::string> operators; // Добавляем вектор операторов

    // Обновленный конструктор
    Factor(const std::vector<std::shared_ptr<Node>>& summandsList, const std::vector<std::string>& ops)
        : summands(summandsList), operators(ops) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Factor:\n";

        for (size_t i = 0; i < summands.size(); ++i) {
            result += summands[i]->ToString(counter + 2);

            // Добавляем оператор, если он есть
            if (i < operators.size()) {
                result += "\n" + std::string((counter + 2) * 2, ' ') + "Operator: " + operators[i] + "\n";
            }
        }

        return result;
    }
};

#endif // FACTOR_H
