//
// Created by Karter Krueger on 12/1/19.
//


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

expr::~expr() {

}

/*
 * (7 points) Implement a concrete class literal, derived from expr, for storing an integer constant (as a long).
 */

class literal : public expr {
    const long value;
public:
    explicit literal(long x) : value(x) { };
};

/*
 (7 points) Implement a concrete class variable, derived from expr, for storing a variable (as a reference to a long).
 */

class variable : public expr {
    long value;
public:
    variable(long x) {
        value = x;
    };
};

class binary_op : expr {
public:
    binary_op(expr *left1, char op1, expr *right1) {
        left = left1;
        op = op1;
        right = right1;
    }
    expr *left;
    expr *right;
    char op;
};
