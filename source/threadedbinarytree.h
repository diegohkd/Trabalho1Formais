#ifndef THREADEDBINARYTREE_H
#define THREADEDBINARYTREE_H

#include "node.h"

#include <stdio.h>
#include <QString>

class ThreadedBinaryTree {
private:
    Node *root;
public:
    ThreadedBinaryTree();
    ThreadedBinaryTree(Node*);
    ~ThreadedBinaryTree();
    Node* getRoot();
    void setRoot(Node*);
    Node* search(Node*, QString);
};

#endif // TRHEADEDBINYARYTREE_H
