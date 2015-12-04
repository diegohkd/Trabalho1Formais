#ifndef REGULARGRAMMAR_H
#define REGULARGRAMMAR_H

#include <QList>
#include <QString>
#include <QStringList>

class RegularGrammar
{
private:
    QList<QStringList> productions;
    QStringList Vt;

    void createAndSetTerminals();
public:
    RegularGrammar();
    QList<QStringList> getProductions();
    void setProductions(QList<QStringList>);
    QStringList getVt();
};

#endif // REGULARGRAMMAR_H
