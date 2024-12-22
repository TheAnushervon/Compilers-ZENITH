#ifndef SIMPLE_H
#define SIMPLE_H

#include <vector>
#include <memory>
#include <string> // Добавлено для std::string
#include "node.h"

class Simple : public Node {
public:
    // Factor { ( + | - ) Factor }
    std::vector<std::shared_ptr<Node>> factors;
    std::vector<std::string> operators; // Добавили вектор операторов

    // Обновленный конструктор, принимающий вектор операторов
    Simple(const std::vector<std::shared_ptr<Node>>& factorsList, const std::vector<std::string>& ops)
        : factors(factorsList), operators(ops) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Simple:\n";

        // Итерируемся по факторам и операторам одновременно
        for (size_t i = 0; i < factors.size(); ++i) {
            result += factors[i]->ToString(counter + 2);

            // Добавляем оператор, если он есть
            if (i < operators.size()) {
                result += "\n" + std::string((counter + 2) * 2, ' ') + "Operator: " + operators[i] + "\n";
            }
        }

        return result;
    }
};

#endif // SIMPLE_H
