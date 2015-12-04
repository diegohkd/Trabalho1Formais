#include "regularexpression.h"

unsigned RegularExpression::id_sub_expression = 0;

RegularExpression::RegularExpression(){
}

RegularExpression::RegularExpression(QString expression){
    this->expression = expression;
    findAndSetTerminals();
}

QString RegularExpression::getExpression(){
    return expression;
}

void RegularExpression::setExpression(QString expression){
    this->expression = expression;
    findAndSetTerminals();
}

/* Check if:
 * - expression contains invalid characters
 * - expression contains an invalid number of characters (e.g. a**)
*/
bool RegularExpression::validateExpression(){
    // add dot when there is a concatenation between two entities
    QString newExpression = "";
    for(int i = 0; i < expression.length()-1; i++){
        newExpression += expression[i];
        if((!isOperator(expression[i]) && expression[i] != '(') || expression[i] == ')' ||
                expression[i] == '?' || expression[i] == '*'){
            if((!isOperator(expression[i+1]) && expression[i+1] != ')') || expression[i+1] == '('){
                newExpression += ".";
            }
        }
    }
    newExpression += expression[expression.length()-1];
    expression = newExpression;

    return true;
}

QList<QString> RegularExpression::findSubExpressions(){
    unsigned begin, end;                                    // begin and end of sub-expression
    QList<QString> result;                                  // list with new expression and sub-expressions
    QString new_expression = "";                            // new expression

    // search for sub-expressions "(...)"
    for(int i = 0; i < expression.length(); i++){
        unsigned count_parenthesis = 0;                     // equals to zero when matching ')' was found
        if(expression[i] == '('){
            count_parenthesis++;
            begin = i;

            // find matching ')'
            i++;
            while(count_parenthesis != 0 && i < expression.length()){
                if(expression[i] == '(')
                    count_parenthesis++;
                if(expression[i] == ')')
                    count_parenthesis--;
                i++;
            }
            end = i-1;

            // add sub-expression into list of sub-expressions "result"
            result.push_back(expression.section("", begin+2, end));

            // append sub-expression ID to new_expression
            QString left = "<", right = ">";
            new_expression += left + QString::number(id_sub_expression++) + right;
            i--;
        }
        else
            new_expression += expression[i];
    }
    result.push_front(new_expression);

    return result;
}

QStringList RegularExpression::getTerminals(){
    return terminals;
}

/*------------------------------------------------
 *              PRIVATE FUNCTIONS
 *------------------------------------------------
*/

void RegularExpression::findAndSetTerminals(){

    terminals.clear();
    for(int i = 0; i < expression.length(); i++){
        if(!isOperator(expression[i]) && expression[i] != '(' && expression[i] != ')' && !terminals.contains((QString)expression[i]))
            terminals.push_back((QString)expression[i]);
    }
}

bool RegularExpression::isOperator(QChar c){
    if(c == '*' || c == '?' || c == '.' || c == '|')
        return true;
    else
        return false;
}
