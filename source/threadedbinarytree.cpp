#include "threadedbinarytree.h"

ThreadedBinaryTree::ThreadedBinaryTree(){
    root = NULL;
}

ThreadedBinaryTree::ThreadedBinaryTree(Node *root){
    this->root = root;
}

ThreadedBinaryTree::~ThreadedBinaryTree(){
}

Node* ThreadedBinaryTree::getRoot(){
    return root;
}

void ThreadedBinaryTree::setRoot(Node *root){
    this->root = root;
}

Node* ThreadedBinaryTree::search(Node *node, QString key){
    Node *left = NULL, *right = NULL;
    if(node->getLabel() == key)
        return node;
    else{
        if(node->getLeftChild() == NULL && node->getRightChild() == NULL)
            return NULL;
    }
    if(node->getLeftChild() != NULL)
        left = search(node->getLeftChild(), key);
    if(node->getRightChild() != NULL)
        right = search(node->getRightChild(), key);
    if(left != NULL)
        return left;
    if(right != NULL)
        return right;
    return NULL;
}
