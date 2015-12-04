#include "node.h"

Node::Node(){
    left_child = NULL;
    right_child = NULL;
    label = "";
    index = 0;
}

Node::Node(Node *parent, Node *left_child, Node *right_child, QString label, unsigned index){
    this->parent = parent;
    this->left_child = left_child;
    this->right_child = right_child;
    this->label = label;
    this->index = index;
}

Node::~Node(){
}

Node* Node::getParent(){
    return parent;
}

Node* Node::getLeftChild(){
    return left_child;
}

Node* Node::getRightChild(){
    return right_child;
}

QString Node::getLabel(){
    return label;
}

unsigned Node::getIndex(){
    return index;
}

void Node::setParent(Node *parent){
    this->parent = parent;
}

void Node::setLeftChild(Node *left_child){
    this->left_child = left_child;
}

void Node::setRightChild(Node *right_child){
    this->right_child = right_child;
}

void Node::setLabel(QString label){
    this->label = label;
}

void Node::setIndex(unsigned index){
    this->index = index;
}
