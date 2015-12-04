#include "finiteautomata.h"

FiniteAutomata::FiniteAutomata(){

}

FiniteAutomata::FiniteAutomata(QTableWidget *transition_table){
    this->transition_table = transition_table;
}

FiniteAutomata::~FiniteAutomata(){
    delete transition_table;
}

void FiniteAutomata::setTransitionTable(QTableWidget *table){
    this->transition_table = table;
}

QTableWidget* FiniteAutomata::getTransitionTable(){
    return transition_table;
}
/*
 * get new state label that has not been used yet
*/
char FiniteAutomata::getNewStateLabel(){
    char current_Nt = 'A';
    bool isNew = false;
    while(!isNew){
        isNew = true;
        for(int i = 0; i < transition_table->rowCount() && isNew == true; i++){
            if(transition_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"),"").compare((QString)current_Nt) == 0)
                isNew = false;
        }
        if(!isNew)
            current_Nt = static_cast<char>(current_Nt + 1);
        else
            return current_Nt;
    }
    return current_Nt;
}

bool FiniteAutomata::isEmpty(){
   FiniteAutomata *temp = new FiniteAutomata(copyTransitionTable());
   temp->minimizeFA();
   if(temp->getTransitionTable()->rowCount() == 1 && temp->getTransitionTable()->verticalHeaderItem(0)->text().contains("*") == false)
       return true;
   else
       return false;
}

/*
 * determinize this finite automata
*/
void FiniteAutomata::determinizeFA(){
    QTableWidget *original_table;               // transition table before determinization
    QStringList all_new_states;                 // list of new created states
    QStringList finals;                         // list of final states

    if(transition_table->horizontalHeaderItem(0)->text().compare("-") != 0){
        // create copy of current transition table in order to modify
        // current table and use the old one
        original_table = copyTransitionTable();

        // add final states to "finals" variable
        for(int i = 0; i < transition_table->rowCount(); i++){
            if(transition_table->verticalHeaderItem(i)->text().contains('*'))
                finals.push_back((QString)transition_table->verticalHeaderItem(i)->text()[transition_table->verticalHeaderItem(i)->text().length()-1]);
        }
        // remove '*' and '->' from original table
        for(int i = 0; i < original_table->rowCount(); i++){
            original_table->setVerticalHeaderItem(i, new QTableWidgetItem((QString)original_table->verticalHeaderItem(i)->text()[original_table->verticalHeaderItem(i)->text().length()-1]));
        }
        // remove rows
        for(int i = transition_table->rowCount(); i > 0; i--)
            transition_table->removeRow(i);

        // set first state label
        transition_table->setVerticalHeaderItem(0, new QTableWidgetItem(original_table->verticalHeaderItem(0)->text()));

        // add first state to all_new_states
        all_new_states.push_back((QString)transition_table->verticalHeaderItem(0)->text()[transition_table->verticalHeaderItem(0)->text().length()-1]);

        // loop over each row trying to create its transitions and possibly creating new states (new rows)
        int current_row = 0;
        while(current_row < transition_table->rowCount()){

            // get current state (e.g. 'A,B,C'), split it up and store each nonterminal into new_state_nonterminals
            QString current_state = transition_table->verticalHeaderItem(current_row)->text();
            QStringList new_state_nonterminals = current_state.split(",");

            // get transitions for all states of current_state (e.g. state is 'A,B', transitions[0]
            // has all transitions of state A and also state B for column 0)
            QStringList transitions = getTransitionsOfState(new_state_nonterminals, original_table);

            for(int i = 0; i < transition_table->columnCount(); i++){

                // check if transition's state does not exist, if so then add to new_states and create new state on the table
                if(transitions[i].compare("-") != 0){
                    if(!all_new_states.contains(transitions[i])){
                        all_new_states.push_back(transitions[i]);
                        transition_table->insertRow(transition_table->rowCount());
                        transition_table->setVerticalHeaderItem(transition_table->rowCount()-1, new QTableWidgetItem(transitions[i]));
                    }
                }
                transition_table->setItem(current_row, i, new QTableWidgetItem(transitions[i]));
            }
            current_row++;
        }

        // set final states
        QList<int> finals_index;
        for(int i = 0; i < transition_table->rowCount(); i++){
            QString state = transition_table->verticalHeaderItem(i)->text();
            for(int j = 0; j < state.length(); j++){
                if(finals.contains((QString)state[j])){
                    finals_index.push_back(i);
                    break;
                }
            }
        }

        // change states with more than one nonterminal to only one terminal

        // create new labels for all states
        char current_nonterminal = 'A';
        QStringList new_states_one_nonterminal_label;
        for(int i = 0; i < all_new_states.length(); i++){
            new_states_one_nonterminal_label.push_back((QString)current_nonterminal);
            current_nonterminal = static_cast<char>(current_nonterminal + 1);
        }

        // change old label for new label of each item
        for(int i = 0; i < transition_table->rowCount(); i++){

            // set header
            transition_table->setVerticalHeaderItem(i, new QTableWidgetItem(new_states_one_nonterminal_label[i]));
            for(int j = 0; j < transition_table->columnCount(); j++){
                if(transition_table->item(i,j)->text().compare("-") != 0){
                    int index = all_new_states.indexOf(transition_table->item(i,j)->text());
                    transition_table->setItem(i, j, new QTableWidgetItem(new_states_one_nonterminal_label[index]));
                }
            }
        }

        // set initial state
        transition_table->setVerticalHeaderItem(0, new QTableWidgetItem("->" + transition_table->verticalHeaderItem(0)->text()));

        // set final states
        for(int i = 0; i < finals_index.length(); i++){
            QString state = transition_table->verticalHeaderItem(finals_index[i])->text();
            transition_table->setVerticalHeaderItem(finals_index[i], new QTableWidgetItem("*" + transition_table->verticalHeaderItem(finals_index[i])->text()));
        }
    }
}

void FiniteAutomata::minimizeFA(){

   if(transition_table->horizontalHeaderItem(0)->text().compare("-") != 0){
       determinizeFA();

       // remove dead states
       removeDeadStates();

       if(transition_table->rowCount() > 0){
           // determinize again only to organize transition table - remove later maybe
           determinizeFA();

           // create error state if necessary and set '-' transitions to error transition
           char error = '#';
           for(int i = 0; i < transition_table->rowCount(); i++){
               for(int j = 0; j < transition_table->columnCount(); j++){
                   if(transition_table->item(i,j)->text().compare("-") == 0){
                       // create error state if not created yet
                       if(error == '#'){
                           error = getNewStateLabel();
                           transition_table->insertRow(transition_table->rowCount());
                           transition_table->setVerticalHeaderItem(transition_table->rowCount()-1, new QTableWidgetItem((QString)error));
                           for(int k = 0; k < transition_table->columnCount(); k++){
                               transition_table->setItem(transition_table->rowCount()-1, k, new QTableWidgetItem((QString)error));
                           }
                       }
                       transition_table->setItem(i, j, new QTableWidgetItem((QString)error));
                   }
               }
           }

           // find equivalent sets
           QStringList equivalent_sets;

           // add final and not final sets to equivalent_sets
           QString finals = "", not_finals = "";
           for(int i = 0; i < transition_table->rowCount(); i++){
               if(transition_table->verticalHeaderItem(i)->text().contains("*")){
                   finals += transition_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"), "");
               }else{
                   not_finals += transition_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"), "");
               }
           }
           if(!finals.isEmpty() && !not_finals.isEmpty()){
               if(finals.contains(transition_table->verticalHeaderItem(0)->text().replace(QRegExp("[*->]"), ""))){
                   equivalent_sets.push_back(finals);
                   equivalent_sets.push_back(not_finals);
               }else{
                   equivalent_sets.push_back(not_finals);
                   equivalent_sets.push_back(finals);
               }
           }else{
               if(!finals.isEmpty())
                   equivalent_sets.push_back(finals);
               if(!not_finals.isEmpty())
                   equivalent_sets.push_back(not_finals);
           }

           // begin loop
           int length_before = equivalent_sets.length();
           int length_after = -1;
           while(length_before != length_after){
               length_before = equivalent_sets.length();
               QStringList new_equivalent_sets;
               for(int i = 0; i < equivalent_sets.length(); i++){
                   QString current_equivalent_set;
                   QString new_equivalent_set = "";

                   current_equivalent_set = equivalent_sets[i];

                   QString first_state = (QString)equivalent_sets[i][0];
                   for(int j = 1; j < equivalent_sets[i].length() ; j++){
                       // check if first_state and current_equivalent_set[j] are not equivalents
                       // if so then add current_equivalent_set to new_equivalent_set
                       if(!checkEquivalenceOfTwoStates(equivalent_sets, first_state, (QString)equivalent_sets[i][j])){
                           new_equivalent_set += equivalent_sets[i][j];
                           equivalent_sets[i].replace(equivalent_sets[i][j], "");
                           j--;
                       }
                   }
                   if(new_equivalent_set.length() != 0)
                       new_equivalent_sets.push_back(new_equivalent_set);
               }
               equivalent_sets += new_equivalent_sets;
               length_after = equivalent_sets.length();
           }

           // if found equivalent sets with more than one state then create new table
           if(equivalent_sets.length() < transition_table->rowCount()){
               QTableWidget *old_table = copyTransitionTable();
               QString new_states_labels;

               // delete extra columns and rows of table
               for(int i = transition_table->rowCount(); i >= 0; i--)
                   transition_table->removeRow(i);

               // add correct numbe of rows
               for(int i = 0; i < equivalent_sets.length(); i++)
                   transition_table->insertRow(0);

               // create new labels of states
               char current_new_state = 'A';
               for(int i = 0; i < equivalent_sets.length(); i++){
                   new_states_labels.push_back((QString)current_new_state);
                   transition_table->setVerticalHeaderItem(i, new QTableWidgetItem((QString)current_new_state));
                   current_new_state = static_cast<char>(current_new_state + 1);
               }


               for(int i = 0; i < equivalent_sets.length(); i++){
                   QString state = (QString)equivalent_sets[i][0];

                   // find row of one state of current equivalent state
                   for(int j = 0; j < old_table->rowCount(); j++){
                       if(old_table->verticalHeaderItem(j)->text().replace(QRegExp("[*->]"), "").compare(state) == 0){
                           // modify each item of current row
                           for(int k = 0; k < old_table->columnCount(); k++){
                               // find equivalent set of current transition
                               if(old_table->item(j,k)->text().compare("-") != 0){
                                   for(int l = 0; l < equivalent_sets.length(); l++){
                                       if(equivalent_sets[l].contains(old_table->item(j, k)->text()))
                                           transition_table->setItem(i,k, new QTableWidgetItem((QString)new_states_labels[l]));
                                   }
                               }
                           }
                       }
                   }
               }

               // set initial and final states
               transition_table->setVerticalHeaderItem(0, new QTableWidgetItem("->" + transition_table->verticalHeaderItem(0)->text()));

               // get final states from old_table
               QStringList finals;
               for(int i = 0; i < old_table->rowCount(); i++){
                   if(old_table->verticalHeaderItem(i)->text().contains("*"))
                       finals.push_back(old_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"), ""));
               }

               for(int i = 0; i < equivalent_sets.length(); i++){
                   for(int j = 0; j < equivalent_sets[i].length(); j++){
                       if(finals.contains((QString)equivalent_sets[i][j])){
                           transition_table->setVerticalHeaderItem(i, new QTableWidgetItem("*" + transition_table->verticalHeaderItem(i)->text()));
                           break;
                       }
                   }
               }
           }
           removeDeadStates();
       }else{
           transition_table->insertRow(0);
           transition_table->setVerticalHeaderItem(0, new QTableWidgetItem("->A"));
           for(int i = 0; i < transition_table->columnCount(); i++){
               transition_table->setItem(0,i,new QTableWidgetItem("-"));
           }
       }
   }
}

void FiniteAutomata::complementsFA(){
    determinizeFA();
    // create error state if necessary and set '-' transitions to error transition
    char error = '#';
    for(int i = 0; i < transition_table->rowCount(); i++){
        for(int j = 0; j < transition_table->columnCount(); j++){
            if(transition_table->item(i,j)->text().compare("-") == 0){
                // create error state if not created yet
                if(error == '#'){
                    error = getNewStateLabel();
                    transition_table->insertRow(transition_table->rowCount());
                    transition_table->setVerticalHeaderItem(transition_table->rowCount()-1, new QTableWidgetItem((QString)error));
                    for(int k = 0; k < transition_table->columnCount(); k++){
                        transition_table->setItem(transition_table->rowCount()-1, k, new QTableWidgetItem((QString)error));
                    }
                }
                transition_table->setItem(i, j, new QTableWidgetItem((QString)error));
            }
        }
    }
    for(int i = 0; i < transition_table->rowCount(); i++){
        if(transition_table->verticalHeaderItem(i)->text().contains("*")){
          transition_table->setVerticalHeaderItem(i, new QTableWidgetItem(transition_table->verticalHeaderItem(i)->text().remove("*")));
        } else {
          transition_table->setVerticalHeaderItem(i, new QTableWidgetItem(transition_table->verticalHeaderItem(i)->text().insert(0, "*")));  
        }
    }
}
/*------------------------------------------------
 *              PRIVATE FUNCTIONS
 *------------------------------------------------
*/

QTableWidget* FiniteAutomata::copyTransitionTable(){
    QTableWidget *new_table = new QTableWidget(transition_table->rowCount(), transition_table->columnCount());

    // copy row headers label
    for(int i = 0; i < new_table->rowCount(); i++)
        new_table->setVerticalHeaderItem(i, new QTableWidgetItem(transition_table->verticalHeaderItem(i)->text()));
    // copy column headers label
    for(int i = 0; i < new_table->columnCount(); i++)
        new_table->setHorizontalHeaderItem(i, new QTableWidgetItem(transition_table->horizontalHeaderItem(i)->text()));

    // copy all items
    for(int i = 0; i < new_table->rowCount(); i++){
        for(int j = 0; j < new_table->columnCount(); j++){
            new_table->setItem(i,j, new QTableWidgetItem(transition_table->item(i,j)->text()));
        }
    }
    return new_table;
}

/*
 * return transitions for each column
 *
 * argument state might has more than one nonterminal (e.g. 'S,A'), in this case return
 * the union of their transitions
*/
QStringList FiniteAutomata::getTransitionsOfState(QStringList state, QTableWidget *table){
    QList<QStringList> list;

    for(int i = 0; i < table->columnCount(); i++){
        QStringList states;
        list.push_back(states);
    }

    for(int i = 0; i < state.length(); i++){
        int index;

        // find row index of current state
        for(int j = 0; j < table->rowCount(); j++){
            QChar c = table->verticalHeaderItem(j)->text()[table->verticalHeaderItem(j)->text().length()-1];
            if(((QString)c).compare(state[i]) == 0)
                index = j;
        }

        // add each transition to list
        QStringList transitions;
        for(int j = 0; j < table->columnCount(); j++){
            QStringList temp = table->item(index,j)->text().split(",");
            if(temp.length() > 0 && temp[0].compare("-") != 0){
                for(int k = 0; k < temp.length(); k++){
                    if(!list[j].contains(temp[k]))
                        list[j].push_back(temp[k]);
                }
            }
        }
    }

    for(int i = 0; i < list.length(); i++)
        list[i].sort();

    QStringList return_list;
    QString transition = "";
    for(int i = 0; i < list.length(); i++){
        if(list[i].length() > 0){
            transition = list[i][0];
            for(int j = 1; j < list[i].length(); j++){
                transition += "," + list[i][j];
            }
        }else{
            transition = "-";
        }
        return_list.push_back(transition);
    }

    return return_list;
}

void FiniteAutomata::removeDeadStates(){
    QStringList alive_states;
    QList<int> pending_rows_index;

    //
    for(int i = 0; i < transition_table->rowCount(); i++){
        QString temp = transition_table->verticalHeaderItem(i)->text();
        if(transition_table->verticalHeaderItem(i)->text().contains("*"))
            alive_states.push_back(transition_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"), ""));
        else
            pending_rows_index.push_back(i);
    }

    int length_before = alive_states.length();
    int length_after = -1;
    while(length_before != length_after){
        length_before = alive_states.length();

        for(int i = 0; i < pending_rows_index.length(); i++){
            for(int j = 0; j < transition_table->columnCount(); j++){
                if(alive_states.contains(transition_table->item(pending_rows_index[i], j)->text())){
                    alive_states.push_back(transition_table->verticalHeaderItem(pending_rows_index[i])->text().replace(QRegExp("[*->]"), ""));
                    pending_rows_index.removeAt(i);
                    break;
                }
            }
        }
        length_after = alive_states.length();
    }

    // remove dead states
    QStringList dead_states;
    int total_removed = 0;
    for(int i = 0; i < pending_rows_index.length(); i++){
        dead_states.push_back(transition_table->verticalHeaderItem(pending_rows_index[i] - total_removed)->text().replace(QRegExp("[*->]"), ""));
        transition_table->removeRow(pending_rows_index[i] - total_removed);
        total_removed++;
    }

    // remove transitions to dead states
    for(int i = 0; i < transition_table->rowCount(); i++){
        for(int j = 0; j < transition_table->columnCount(); j++){
            if(dead_states.contains(transition_table->item(i,j)->text()))
                transition_table->setItem(i,j, new QTableWidgetItem("-"));
        }
    }
}

bool FiniteAutomata::checkEquivalenceOfTwoStates(QStringList equivalent_sets, QString state_1, QString state_2){
    int state_1_row_i = -1, state_2_row_i = -1;

    // find index (row) of states 1 and 2
    for(int i = 0; i < transition_table->rowCount(); i++){
        if(transition_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"), "").compare(state_1) == 0)
            state_1_row_i = i;
        if(transition_table->verticalHeaderItem(i)->text().replace(QRegExp("[*->]"), "").compare(state_2) == 0)
            state_2_row_i = i;
    }

    bool is_same_equiv_set = true;
    for(int i = 0; i < transition_table->columnCount() && is_same_equiv_set == true; i++){
        for(int j = 0; j < equivalent_sets.length(); j++){
            QString st1 = transition_table->item(state_1_row_i, i)->text();
            QString st2 = transition_table->item(state_2_row_i, i)->text();
            if(equivalent_sets[j].contains(transition_table->item(state_1_row_i, i)->text())){
                if(!equivalent_sets[j].contains(transition_table->item(state_2_row_i, i)->text()))
                    is_same_equiv_set = false;
            }
            if(equivalent_sets[j].contains(transition_table->item(state_2_row_i, i)->text())){
                if(!equivalent_sets[j].contains(transition_table->item(state_1_row_i, i)->text()))
                    is_same_equiv_set = false;
            }
        }
    }
    return is_same_equiv_set;
}

void FiniteAutomata::modifyTransitionTable(QTableWidget *table){
    for(int i = transition_table->rowCount(); i > 0; i--)
        transition_table->removeRow(i);
    for(int i = transition_table->columnCount(); i > 0; i--)
        transition_table->removeColumn(i);

    transition_table->setVerticalHeaderItem(0, new QTableWidgetItem(table->verticalHeaderItem(0)->text()));
    transition_table->setHorizontalHeaderItem(0, new QTableWidgetItem(table->horizontalHeaderItem(0)->text()));
    for(int i = 1; i < table->rowCount(); i++){
        transition_table->insertRow(i);
        transition_table->setVerticalHeaderItem(i, new QTableWidgetItem(table->verticalHeaderItem(i)->text()));
    }
    for(int i = 1; i < table->columnCount(); i++){
        transition_table->insertColumn(i);
        transition_table->setHorizontalHeaderItem(i, new QTableWidgetItem(table->horizontalHeaderItem(i)->text()));
    }
    for(int i = 0; i < transition_table->rowCount(); i++){
        for(int j = 0; j < transition_table->columnCount(); j++){
            transition_table->setItem(i,j, new QTableWidgetItem(table->item(i,j)->text()));
        }
    }
}
