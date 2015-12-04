#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "regulargrammar.h"
#include "regularexpression.h"
#include "finiteautomatagenerator.h"
#include "finiteautomata.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <QTableWidget>

class Controller
{
private:
    RegularGrammar rg;
    RegularExpression re, pattern_re;
    FiniteAutomata *fa1, *fa2, *fa3, *fa4, *fa5, *fa6, *fa7, *fa8, *fa9, *pattern_fa;

    FiniteAutomataGenerator fag;
public:
    Controller();
    void createAndSetRegularGrammar(QString);
    void createAndSetRegularExpression(QString);
    void createAndSetPatternRegularExpression(QString);
    RegularGrammar getRegularGrammar();
    void createAndSetFA(QTableWidget*, int);
    void generateFAFromRG(int);
    void generateFAFromRE(int);
    bool determinizeFA(int);
    bool minimizeFA(int);
    void complementsFA(int);
    void intersecFAs();
    bool equalsFAs();
    bool isSameValidator(FiniteAutomata*, FiniteAutomata*);
    void uniFAs(QTableWidget*, QTableWidget*, QTableWidget*, FiniteAutomata*);
    QList<QList<int> > findTextPattern(QString, QString);
    QList<QList<int> > formatFontPatterns(QList<QList<int> >);
    void modifyFA(int, int);
};

#endif // CONTROLLER_H
