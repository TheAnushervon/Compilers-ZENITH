#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <iostream>
#include "node.h"

class Type : public Node{
public:
    virtual ~Type() = default;


    virtual void Print() const {
        std::cout << "Type: " << ToString() << std::endl;
    }
};

#endif // TYPE_H
