#ifndef FINITEAUTOMATA_H
#define FINITEAUTOMATA_H

#include <QTableWidget>
#include <QStringList>
#include <QList>

class FiniteAutomata
{
private:
    QTableWidget *transition_table;
    QStringList getTransitionsOfState(QStringList, QTableWidget*);
    QString createState(QStringList);
    void removeDeadStates();
    bool checkEquivalenceOfTwoStates(QStringList, QString, QString);
public:
    FiniteAutomata();
    FiniteAutomata(QTableWidget*);
    ~FiniteAutomata();
    char getNewStateLabel();
    bool isEmpty();
    QTableWidget* copyTransitionTable();
    QTableWidget* getTransitionTable();
    void setTransitionTable(QTableWidget*);
    void determinizeFA();
    void minimizeFA();
    void complementsFA();
    void modifyTransitionTable(QTableWidget*);
};

#endif // FINITEAUTOMATA_H
