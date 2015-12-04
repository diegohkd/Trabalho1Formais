#include "regulargrammar.h"

RegularGrammar::RegularGrammar()
{
}

QList<QStringList> RegularGrammar::getProductions(){
    return productions;
}

void RegularGrammar::setProductions(QList<QStringList > productions){
    this->productions = productions;
    createAndSetTerminals();
}

void RegularGrammar::createAndSetTerminals(){
    Vt.clear();
    for(int i = 0; i < productions.length(); i++){
        for(int j = 1; j < productions[i].length(); j++){
            if(productions[i][j].length() == 1){
                if(!Vt.contains(productions[i][j]) && productions[i][j].compare("&") != 0)
                    Vt.push_back(productions[i][j]);
            }else{
                if(!Vt.contains((QString)productions[i][j][0]))
                    Vt.push_back((QString)productions[i][j][0]);
            }
        }
    }
}

QStringList RegularGrammar::getVt(){
    return Vt;
}
