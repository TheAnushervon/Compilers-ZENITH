#ifndef FORLOOP_H
#define FORLOOP_H

#include "node.h"
#include <memory>

class ForLoop : public Node {
  public:
  ForLoop(std::shared_ptr<Node> id, std::shared_ptr<Node> rng, std::shared_ptr<Node> bdy)
  : identifier(id), range(rng), body(bdy) {}

    std::string ToString(int counter) const override {
        std::string result = "ForLoop: "+identifier->ToString( counter)
        +range->ToString(counter)
        +body->ToString(counter);
        
        return result;
    }

  private:
 //for Identifier Range loop Body end 
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> range;
    std::shared_ptr<Node> body;
};

#endif // FORLOOP_H
