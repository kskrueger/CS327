//
// Created by Karter Krueger on 11/24/19.
//

#ifndef CS327_EXPR_H
#define CS327_EXPR_H

/*
 * (7 points) Implement an abstract expr class containing:
 *  An empty constructor.
 *  A destructor.
 *  An abstract method long compute() const.
 */

class expr {
public:
    expr();

    ~expr();

    virtual long compute() const {

    };
private:

};

#endif //CS327_EXPR_H
