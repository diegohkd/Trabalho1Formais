#ifndef NODE_H
#define NODE_H

#include <QString>

class Node {
private:
    Node *parent;
    Node *left_child;
    Node *right_child;
    QString label;
    unsigned index;             // if not zero, then it is a leaf
public:
    Node();
    Node(Node*, Node*, Node*, QString, unsigned);
    ~Node();
    Node* getParent();
    Node* getLeftChild();
    Node* getRightChild();
    QString getLabel();
    unsigned getIndex();
    void setParent(Node*);
    void setLeftChild(Node*);
    void setRightChild(Node*);
    void setLabel(QString);
    void setIndex(unsigned);
};

#endif // NODE_H
