#ifndef TYPE_H
#define TYPE_H

#include "node.h"
#include <iostream>
#include <string>

class Type : public Node {
  public:
    virtual ~Type() = default;

    // virtual void Print() const {
    //     std::cout << "Type: " << ToString() << std::endl;
    // }
};

#endif // TYPE_H
