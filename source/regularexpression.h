#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H

#include <QString>
#include <QList>
#include <QStringList>

using namespace std;

class RegularExpression{
private:
    QStringList terminals;
    QString expression;
    static unsigned id_sub_expression;

    void findAndSetTerminals();
    bool isOperator(QChar);
public:
    RegularExpression();
    RegularExpression(QString expression);
    QString getExpression();
    void setExpression(QString);
    bool validateExpression();
    QList<QString> findSubExpressions();
    QStringList getTerminals();
};


#endif // REGULAREXPRESSION_H
