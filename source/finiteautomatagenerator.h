#ifndef FINITEAUTOMATAGENERATOR_H
#define FINITEAUTOMATAGENERATOR_H

#include "node.h"
#include "threadedbinarytree.h"
#include "regularexpression.h"
#include "finiteautomata.h"
#include "regulargrammar.h"
#include <QString>
#include <QList>
#include <QStack>

using namespace std;

class FiniteAutomataGenerator{
private:
    unsigned current_leaf_id;
    ThreadedBinaryTree createThreadedBinaryTree(RegularExpression expression);
    void threadBinaryTree(Node*, QStack<Node*>*);
    QList<Node*> createComposition(QList<Node*>, QList<Node*>, Node*, QString);
    Node* createSubBynaryTree(QString, unsigned, unsigned, unsigned, unsigned);
    int findLowestPriorityOperator(QString, unsigned, unsigned);
    QList<QString> createListOfSubExpIDs(QList<QString>, QString);
    QString getToken(QString, unsigned);
    int compContainsComp(QList<QList<Node*> >, QList<Node*>);
    QList<Node*> getNodesFromComposition(QList<Node*>, QString);
    Node* findNextRightParent(Node*);
    void setFinalStates(QTableWidget*, QList<QList<Node*> >);
public:
    FiniteAutomataGenerator();
    ~FiniteAutomataGenerator();
    void generateFA(FiniteAutomata*, RegularExpression);
    void generateFA(FiniteAutomata*, RegularGrammar);
};

#endif // FINITEAUTOMATAGENERATOR_H
