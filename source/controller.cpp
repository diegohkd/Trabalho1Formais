#include "controller.h"

Controller::Controller()
{
}

void Controller::createAndSetRegularGrammar(QString text){

    text.remove(" ");
    text.replace("\n\n", "\n");
    text.replace(QRegExp("\n$"), "");
    QStringList list = text.split("\n");
    QList<QStringList > all_productions;

    for(int i = 0; i < list.length(); i++){
        QStringList productions;
        productions.push_back((QString)list[i][0]);
        list[i].remove(0,3);
        productions = productions + list[i].split("|");
        all_productions.push_back(productions);
    }
    rg.setProductions(all_productions);
}

void Controller::createAndSetRegularExpression(QString expression){
    re.setExpression(expression);
}

void Controller::createAndSetPatternRegularExpression(QString expression){
    pattern_re.setExpression(expression);
}

void Controller::createAndSetFA(QTableWidget *table, int tab_index){
    switch(tab_index){
    case 0:
        fa1 = new FiniteAutomata(table);
        break;
    case 1:
        fa2 = new FiniteAutomata(table);
        break;
    case 2:
        fa3 = new FiniteAutomata(table);
        break;
    case 3:
        fa4 = new FiniteAutomata(table);
        break;
    case 4:
        fa5 = new FiniteAutomata(table);
        break;
    case 5:
        fa6 = new FiniteAutomata(table);
        break;
    case 6:
        fa7 = new FiniteAutomata(table);
        break;
    case 7:
        fa8 = new FiniteAutomata(table);
        break;
    case 8:
        fa9 = new FiniteAutomata(table);
        break;
    default:
        pattern_fa = new FiniteAutomata(table);
        break;
    }
}

RegularGrammar Controller::getRegularGrammar(){
    return rg;
}

void Controller::generateFAFromRE(int tab_index){
    switch(tab_index){
    case 0:
        fag.generateFA(fa1, re);
        break;
    case 1:
        fag.generateFA(fa2, re);
        break;
    case 2:
        fag.generateFA(fa3, re);
        break;
    case 3:
        fag.generateFA(fa4, re);
        break;
    case 4:
        fag.generateFA(fa5, re);
        break;
    case 5:
        fag.generateFA(fa6, re);
        break;
    case 6:
        fag.generateFA(fa7, re);
        break;
    case 7:
        fag.generateFA(fa8, re);
        break;
    case 8:
        fag.generateFA(fa9, re);
        break;
    default:
        fag.generateFA(pattern_fa, pattern_re);
        break;
    }
}

void Controller::generateFAFromRG(int tab_index){
    switch(tab_index){
    case 0:
        fag.generateFA(fa1, rg);
        break;
    case 1:
        fag.generateFA(fa2, rg);
        break;
    case 2:
        fag.generateFA(fa3, rg);
        break;
    case 3:
        fag.generateFA(fa4, rg);
        break;
    case 4:
        fag.generateFA(fa5, rg);
        break;
    case 5:
        fag.generateFA(fa6, rg);
        break;
    case 6:
        fag.generateFA(fa7, rg);
        break;
    case 7:
        fag.generateFA(fa8, rg);
        break;
    case 8:
        fag.generateFA(fa9, rg);
        break;
    default:
        break;
    }
}

bool Controller::determinizeFA(int tab_index){
    FiniteAutomata *a = new FiniteAutomata(fa1->copyTransitionTable());
    FiniteAutomata *b = new FiniteAutomata(fa1->copyTransitionTable());
    int result = false;
    switch(tab_index){
    case 0:
        a->modifyTransitionTable(fa1->copyTransitionTable());
        fa1->determinizeFA();
        b->modifyTransitionTable(fa1->copyTransitionTable());
        break;
    case 1:
        a->modifyTransitionTable(fa2->copyTransitionTable());
        fa2->determinizeFA();
        b->modifyTransitionTable(fa2->copyTransitionTable());
        break;
    case 2:
        a->modifyTransitionTable(fa3->copyTransitionTable());
        fa3->determinizeFA();
        b->modifyTransitionTable(fa3->copyTransitionTable());
        break;
    case 3:
        a->modifyTransitionTable(fa4->copyTransitionTable());
        fa4->determinizeFA();
        b->modifyTransitionTable(fa4->copyTransitionTable());
        break;
    case 4:
        a->modifyTransitionTable(fa5->copyTransitionTable());
        fa5->determinizeFA();
        b->modifyTransitionTable(fa5->copyTransitionTable());
        break;
    case 5:
        a->modifyTransitionTable(fa6->copyTransitionTable());
        fa6->determinizeFA();
        b->modifyTransitionTable(fa6->copyTransitionTable());
        break;
    case 6:
        a->modifyTransitionTable(fa7->copyTransitionTable());
        fa7->determinizeFA();
        b->modifyTransitionTable(fa7->copyTransitionTable());
        break;
    case 7:
        a->modifyTransitionTable(fa8->copyTransitionTable());
        fa8->determinizeFA();
        b->modifyTransitionTable(fa8->copyTransitionTable());
        break;
    case 8:
        a->modifyTransitionTable(fa9->copyTransitionTable());
        fa9->determinizeFA();
        b->modifyTransitionTable(fa9->copyTransitionTable());
        break;
    default:
        a->modifyTransitionTable(pattern_fa->copyTransitionTable());
        pattern_fa->determinizeFA();
        b->modifyTransitionTable(pattern_fa->copyTransitionTable());
        break;
    }
    result = isSameValidator(a,b);
    return result;
}

bool Controller::isSameValidator(FiniteAutomata *a, FiniteAutomata *b){

    int cl1 = a->getTransitionTable()->rowCount();
    int cl2 = b->getTransitionTable()->rowCount();
    int cc1 = a->getTransitionTable()->columnCount();
    int cc2 = b->getTransitionTable()->columnCount();
    bool result = false;
    if(cl1 == cl2 && cc1 == cc2){
        result = true;
        for(int i = 0; i < cl1 && result == true; i++){
            if(a->getTransitionTable()->verticalHeaderItem(i)->text() != b->getTransitionTable()->verticalHeaderItem(i)->text() ){
                result = false;
            }
        }
        for(int j = 0; j < cc1 && result == true; j++){
            if(a->getTransitionTable()->horizontalHeaderItem(j)->text() != b->getTransitionTable()->horizontalHeaderItem(j)->text()){
                result = false;
            }
        }        
        for(int i = 0; i < cl1 && result == true; i++){
            for(int j = 0; j < cc1; j++){
                if(a->getTransitionTable()->item(i,j)->text() != b->getTransitionTable()->item(i,j)->text()){
                    result = false;
                }
            }
        }
    }

    return result;
}

bool Controller::minimizeFA(int tab_index){
    FiniteAutomata *a = new FiniteAutomata(fa1->copyTransitionTable());
    FiniteAutomata *b = new FiniteAutomata(fa1->copyTransitionTable());
    int result = false;
    switch(tab_index){
    case 0:
        a->modifyTransitionTable(fa1->copyTransitionTable());
        fa1->minimizeFA();
        b->modifyTransitionTable(fa1->copyTransitionTable());
        break;
    case 1:
        a->modifyTransitionTable(fa2->copyTransitionTable());
        fa2->minimizeFA();
        b->modifyTransitionTable(fa2->copyTransitionTable());
        break;
    case 2:
        a->modifyTransitionTable(fa3->copyTransitionTable());
        fa3->minimizeFA();
        b->modifyTransitionTable(fa3->copyTransitionTable());
        break;
    case 3:
        a->modifyTransitionTable(fa4->copyTransitionTable());
        fa4->minimizeFA();
        b->modifyTransitionTable(fa4->copyTransitionTable());
        break;
    case 4:
        a->modifyTransitionTable(fa5->copyTransitionTable());
        fa5->minimizeFA();
        b->modifyTransitionTable(fa5->copyTransitionTable());
        break;
    case 5:
        a->modifyTransitionTable(fa6->copyTransitionTable());
        fa6->minimizeFA();
        b->modifyTransitionTable(fa6->copyTransitionTable());
        break;
    case 6:
        a->modifyTransitionTable(fa7->copyTransitionTable());
        fa7->minimizeFA();
        b->modifyTransitionTable(fa7->copyTransitionTable());
        break;
    case 7:
        a->modifyTransitionTable(fa8->copyTransitionTable());
        fa8->minimizeFA();
        b->modifyTransitionTable(fa8->copyTransitionTable());
        break;
    case 8:
        a->modifyTransitionTable(fa9->copyTransitionTable());
        fa9->minimizeFA();
        b->modifyTransitionTable(fa9->copyTransitionTable());
        break;
    default:
        a->modifyTransitionTable(pattern_fa->copyTransitionTable());
        pattern_fa->minimizeFA();
        b->modifyTransitionTable(pattern_fa->copyTransitionTable());
        break;
    }
    result = isSameValidator(a,b);
    return result;
}

void Controller::complementsFA(int tab_index){
    switch(tab_index){
    case 0:
        fa1->complementsFA();
        break;
    case 1:
        fa2->complementsFA();
        break;
    case 2:
        fa3->complementsFA();
        break;
    case 3:
        fa4->complementsFA();
        break;
    case 4:
        fa5->complementsFA();
        break;
    case 5:
        fa6->complementsFA();
        break;
    case 6:
        fa7->complementsFA();
        break;
    case 7:
        fa8->complementsFA();
        break;
    case 8:
        fa9->complementsFA();
        break;
    default:
        pattern_fa->complementsFA();
        break;
    }
}

//if instancias de cima = debug visual nas tabelas da interface
bool Controller::equalsFAs(){
    FiniteAutomata *fuc1 = new FiniteAutomata(fa7->getTransitionTable());
    FiniteAutomata *fuc2 = new FiniteAutomata(fa8->getTransitionTable());


    FiniteAutomata *f1 = new FiniteAutomata(fa1->getTransitionTable());
    FiniteAutomata *f2 = new FiniteAutomata(fa2->getTransitionTable());

    // 1 - GR e AR para AFs
    fag.generateFA(f1, rg);
    fag.generateFA(f2, re);//já é determinizado

    FiniteAutomata *ftemp1 = new FiniteAutomata(fa1->copyTransitionTable());
    FiniteAutomata *ftemp2 = new FiniteAutomata(fa2->copyTransitionTable());

    // 2 - AF1 determinizados
    ftemp1->determinizeFA();

    //TODO isto é gambiarra
    // 3 - AF1 e AF2 completos
    ftemp1->complementsFA();
    ftemp1->complementsFA();
    ftemp2->complementsFA();
    ftemp2->complementsFA();

    FiniteAutomata *fc1 = new FiniteAutomata(ftemp1->copyTransitionTable());
    FiniteAutomata *fc2 = new FiniteAutomata(ftemp2->copyTransitionTable());
    
    //4 - AF1 e AF2 complementados AFC1 e AFC2
    fc1->complementsFA();
    fc2->complementsFA();
    fa3->modifyTransitionTable(fc1->copyTransitionTable());
    fa4->modifyTransitionTable(fc2->copyTransitionTable());



    QTableWidget *t1 = ftemp1->getTransitionTable();
    QTableWidget *t2 = ftemp2->getTransitionTable();
    QTableWidget *tc1 = fc1->getTransitionTable();
    QTableWidget *tc2 = fc2->getTransitionTable();

    QTableWidget *tu1 = fa5->getTransitionTable();
    QTableWidget *tu2 = fa6->getTransitionTable();

    // 5 - Uniao
    uniFAs(tc1, t2, tu1, fa5);
    uniFAs(tc2, t1, tu2, fa6);

    FiniteAutomata *temp1 = new FiniteAutomata(tu1);
    FiniteAutomata *temp2 = new FiniteAutomata(tu2);
    fa5->modifyTransitionTable(temp1->copyTransitionTable());
    fa6->modifyTransitionTable(temp2->copyTransitionTable());

    // 6 - Complemento das Unioes
    fuc1->modifyTransitionTable(temp1->copyTransitionTable());
    fuc2->modifyTransitionTable(temp2->copyTransitionTable());
    fuc1->complementsFA();
    fuc2->complementsFA();

    // atualiza m9 com f8 U f9
    QTableWidget *tfu1 = fa7->copyTransitionTable();
    QTableWidget *tfu2 = fa8->copyTransitionTable();
    QTableWidget *tf = fa9->getTransitionTable();
    uniFAs(tfu1, tfu2, tf, fa9);
    FiniteAutomata *temp3 = new FiniteAutomata(tf);
    fa9->modifyTransitionTable(temp3->copyTransitionTable());


    // 7 - Verifica se o m9 minimo é vazio ou não
    bool isEquals = false;
    FiniteAutomata *finalTemp = new FiniteAutomata(fa9->copyTransitionTable());
    finalTemp->minimizeFA();
    
    QTableWidget *tabFinal = finalTemp->getTransitionTable();

    if(!tabFinal->verticalHeaderItem(0)->text().contains("*") && tabFinal->rowCount() == 1) {
        isEquals = true;
    }

    return isEquals;
}

void Controller::uniFAs(QTableWidget *ci1, QTableWidget *ci2, QTableWidget *ci3, FiniteAutomata *fa){
    //deleta o que existia anteriormente
    for(int i = ci3->rowCount()-1; i > 0; i--)
        ci3->removeRow(i);
    for(int i = ci3->columnCount(); i >= 0; i--)
        ci3->removeColumn(i);
    ci3->setItem(0,0, new QTableWidgetItem(""));

    //cria as colunas de af1 e os seus respectivos labels
    QList<QString> labels;
    for(int i = 0; i < ci1->columnCount(); i++){
        ci3->insertColumn(i);
        ci3->setHorizontalHeaderItem(i, new QTableWidgetItem(ci1->horizontalHeaderItem(i)->text()));
        labels.insert(i, ci1->horizontalHeaderItem(i)->text());
    }

    //cria as colunas e coloca os labels para labels ainda não criados para af2
    QList<QString> labels2;
    for(int i=0; i < ci2->columnCount(); i++){
        labels2.insert(i, ci2->horizontalHeaderItem(i)->text());
        if(!labels.contains(ci2->horizontalHeaderItem(i)->text())){
            ci3->insertColumn(ci3->columnCount());
            ci3->setHorizontalHeaderItem(ci3->columnCount()-1, new QTableWidgetItem(ci2->horizontalHeaderItem(i)->text()));
        }
    }

    //considerando que o estado inicial é sempre o primeiro
    //Criacao do novo estado inicial
    if(ci1->verticalHeaderItem(0)->text().contains("*") || ci2->verticalHeaderItem(0)->text().contains("*")){
        ci3->setVerticalHeaderItem(0, new QTableWidgetItem("*->S"));
    } else {
        ci3->setVerticalHeaderItem(0, new QTableWidgetItem("->S"));
    }


    //cria linhas para os estados de af1 e insere os estados
    int j;
    for(int i=0; i < ci1->rowCount(); i++){
        j = ci3->rowCount();
        ci3->insertRow(j);
        if(ci1->verticalHeaderItem(i)->text().contains("->")){
            ci3->setVerticalHeaderItem(j, new QTableWidgetItem(ci1->verticalHeaderItem(i)->text().remove("->")));
        } else {
            ci3->setVerticalHeaderItem(j, new QTableWidgetItem(ci1->verticalHeaderItem(i)->text()));
        }
    }


    //hash necessario para mapear o nome do estado antigo para o novo
    //cria linhas para os estados de af2 e insere os estados
    QHash<QString, QString> hash2;
    for(int i=0; i < ci2->rowCount(); i++){
        QString newState = (QString)fa->getNewStateLabel();
        j = ci3->rowCount();
        ci3->insertRow(j);
        hash2[ci2->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"),"")] = newState;
        if(ci2->verticalHeaderItem(i)->text().contains("*")){
            ci3->setVerticalHeaderItem(j, new QTableWidgetItem("*"+newState));
        } else {
            ci3->setVerticalHeaderItem(j, new QTableWidgetItem(newState));
        }
    }

    //monta as transições para o estado inicial.
    for(int j = 0; j < ci3->columnCount(); j++){
        QString trans = "-";
        for(int i = 0; i < ci1->columnCount(); i++){
            if(ci1->horizontalHeaderItem(i)->text().contains(ci3->horizontalHeaderItem(j)->text())){
                trans = ci1->item(0,i)->text();
            }
        }

        for(int i = 0; i < ci2->columnCount(); i++){
            if(ci2->horizontalHeaderItem(i)->text().contains(ci3->horizontalHeaderItem(j)->text())){
                if(trans.contains("-")){
                    trans = hash2[ci2->item(0,i)->text()];
                } else {
                    trans = trans+","+hash2[ci2->item(0,i)->text()];    
                }
            }
        }
        ci3->setItem(0,j, new QTableWidgetItem(trans));
    }
    
    //coloca em af3 as transicoes de af1
    for(int j = 0; j < ci3->columnCount(); j++){
        int index = labels.indexOf(ci3->horizontalHeaderItem(j)->text());
        if(index != -1){
            for(int i = 0; i < ci1->rowCount(); i++){
                ci3->setItem(i+1,j, new QTableWidgetItem(ci1->item(i,index)->text()));
            }
        } else {
            for(int i = 0; i < ci1->rowCount(); i++){
                ci3->setItem(i+1,j, new QTableWidgetItem("-"));
            }
        }
    }

    //coloca em af3 as transicoes de af2
    for(int j = 0; j < ci3->columnCount(); j++){
        int index = labels2.indexOf(ci3->horizontalHeaderItem(j)->text());
        if(index != -1){
            for(int i = 0; i < ci2->rowCount(); i++){
                ci3->setItem(i+1+ci1->rowCount(),j, new QTableWidgetItem(hash2[ci2->item(i,index)->text()]));
            }
        } else {
            for(int i = 0; i < ci2->rowCount(); i++){
                ci3->setItem(i+1+ci1->rowCount(),j, new QTableWidgetItem("-"));
            }
        }
    }
}

//¬(¬M1U¬M2)
void Controller::intersecFAs(){

    //M1 e M2
    FiniteAutomata *f1 = new FiniteAutomata(fa1->getTransitionTable());
    FiniteAutomata *f2 = new FiniteAutomata(fa2->getTransitionTable());

    //¬M1 e ¬M2
    FiniteAutomata *fc1 = new FiniteAutomata(fa3->getTransitionTable());
    FiniteAutomata *fc2 = new FiniteAutomata(fa4->getTransitionTable());
    fc1->modifyTransitionTable(f1->getTransitionTable());
    fc2->modifyTransitionTable(f2->getTransitionTable());

    fc1->complementsFA();
    fc2->complementsFA();

    QTableWidget *ci1 = fc1->getTransitionTable();
    QTableWidget *ci2 = fc2->getTransitionTable();

    //fa5 = ¬M1 U ¬M2
    QTableWidget *ci3 = fa5->getTransitionTable();
    uniFAs(ci1, ci2, ci3, fa5);

    //aplica o complementsAF no fa5
    FiniteAutomata *f6 = new FiniteAutomata(fa6->getTransitionTable());
    f6->modifyTransitionTable(fa5->getTransitionTable());
    f6->complementsFA();
}

QList<QList<int> > Controller::findTextPattern(QString text_pattern, QString text){
    createAndSetPatternRegularExpression(text_pattern);
    //generateFAFromRE(-1);

    QTableWidget *table = new QTableWidget(1,1);
    createAndSetFA(table, -1);

    generateFAFromRE(-1);

    table = pattern_fa->getTransitionTable();
    int i_current_state = 0;
    QList<QList<int> > found_patterns_index;
    QList<int> new_pattern;

    for(int i = 0; i < text.length(); i++){
        i_current_state = 0;
        // check if initial state contains transition with current character
        QString transition = "-";
        for(int j = 0; j < table->columnCount(); j++){
            if(table->horizontalHeaderItem(j)->text().compare((QString)text[i]) == 0 && table->item(0,j)->text().compare("-") != 0){
                transition = table->item(0,j)->text();
            }
        }
        int begin_pattern = i;
        if(transition.compare("-") != 0){
            // found transition, then go to the next state

            // save begin of sentence index at text
            new_pattern.clear();
            new_pattern.push_back(i);
            found_patterns_index.push_back(new_pattern);

            // find next state row
            for(int j = 0; j < table->rowCount(); j++){
                if(table->verticalHeaderItem(j)->text().remove(QRegExp("[*->]")).compare(transition) == 0)
                    i_current_state = j;
            }

            bool is_pattern = true;
            while(is_pattern){
                // if current state is final, then pattern was found and set end of pattern index at text
                if(table->verticalHeaderItem(i_current_state)->text().contains("*")){
                    found_patterns_index[found_patterns_index.length()-1].push_back(i);
                }

                // check if there is transition for next character
                i++;
                if(i < text.length()){
                    transition = "-";
                    for(int j = 0; j < table->columnCount(); j++){
                        if(table->horizontalHeaderItem(j)->text().compare((QString)text[i]) == 0 && table->item(i_current_state, j)->text().compare("-") != 0){
                            transition = table->item(i_current_state, j)->text();
                        }
                    }

                    // has transition
                    if(transition.compare("-") != 0){
                        // find next state row
                        for(int j = 0; j < table->rowCount(); j++){
                            if(table->verticalHeaderItem(j)->text().remove(QRegExp("[*->]")).compare(transition) == 0)
                                i_current_state = j;
                        }

                    }else
                        is_pattern = false;
                }else
                    is_pattern = false;
            }
        }
        i = begin_pattern;
    }
    return found_patterns_index;
}

QList<QList<int> > Controller::formatFontPatterns(QList<QList<int> > list){
    QList<QList<int> > new_list;
    QList<int> pattern;
    if(list[0].length() > 1){
        pattern.push_back(list[0][0]);
        pattern.push_back(list[0][list[0].length()-1]);
        new_list.push_back(pattern);
    }
    for(int i = 1; i < list.length(); i++){
        bool was_added = false;
        for(int j = 0; j < new_list.length(); j++){
            pattern.clear();
            pattern.push_back(list[i][0]);
            pattern.push_back(list[i][list[i].length()-1]);
            if(pattern.length() > 1){
                if(pattern[0] >= new_list[j][0] && pattern[0] <= new_list[j][1]){
                    QList<int> deletar = new_list[j];
                    if(pattern[1] > new_list[j][1]){
                        new_list[j][new_list[j].length()-1] = pattern[1];
                    }
                    was_added = true;
                }
            }
        }
        if(was_added == false){
            if(list[i].length() > 1){
                pattern.clear();
                pattern.push_back(list[i][0]);
                pattern.push_back(list[i][list[i].length()-1]);
                new_list.push_back(pattern);
            }
        }
    }
    return new_list;
}

void Controller::modifyFA(int index, int index2){
    QTableWidget *table;

    switch (index2) {
    case 0:
        table = fa1->getTransitionTable();
        break;
    case 1:
        table = fa2->getTransitionTable();
        break;
    case 2:
        table = fa3->getTransitionTable();
        break;
    case 3:
        table = fa4->getTransitionTable();
        break;
    case 4:
        table = fa5->getTransitionTable();
        break;
    case 5:
        table = fa6->getTransitionTable();
        break;
    case 6:
        table = fa7->getTransitionTable();
        break;
    case 7:
        table = fa8->getTransitionTable();
        break;
    case 8:
        table = fa9->getTransitionTable();
        break;
    default:
        break;
    }

    switch (index) {
    case 0:
        fa1->modifyTransitionTable(table);
        break;
    case 1:
        fa2->modifyTransitionTable(table);
        break;
    case 2:
        fa3->modifyTransitionTable(table);
        break;
    case 3:
        fa4->modifyTransitionTable(table);
        break;
    case 4:
        fa5->modifyTransitionTable(table);
        break;
    case 5:
        fa6->modifyTransitionTable(table);
        break;
    case 6:
        fa7->modifyTransitionTable(table);
        break;
    case 7:
        fa8->modifyTransitionTable(table);
        break;
    case 8:
        fa9->modifyTransitionTable(table);
        break;
    default:
        break;
    }
}
