#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "node.h"
#include <string>

class Identifier : public Node {
public:
    std::string name;

    Identifier(const std::string &name) : name(name) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Identifier:\n";
        result += ots + "  " + name + "\n"; // Отображение имени с дополнительным отступом
        return result;
    }
};

#endif // IDENTIFIER_H
