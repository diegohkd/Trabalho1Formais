#include "finiteautomatagenerator.h"

FiniteAutomataGenerator::FiniteAutomataGenerator(){
}

FiniteAutomataGenerator::~FiniteAutomataGenerator(){
}

void FiniteAutomataGenerator::generateFA(FiniteAutomata *fa, RegularExpression expression){
    QTableWidget *table = fa->getTransitionTable();
    QList<QList<Node*> > all_compositions;
    unsigned current_row;
    char current_nonterminal;
    int count_terminals;

    current_row = 0;
    current_leaf_id = 1;

    // delete extra columns and rows of table
    for(int i = table->rowCount(); i > 0; i--)
        table->removeRow(i);
    for(int i = table->columnCount(); i > 0; i--)
        table->removeColumn(i);
    table->setItem(0,0, new QTableWidgetItem(""));

    // check if RE is an empty sentence
    if(expression.getExpression().compare("&") == 0){
        table->setVerticalHeaderItem(0, new QTableWidgetItem("*->A"));
        table->setItem(0, 0, new QTableWidgetItem("-"));
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("a"));
    }else{

        // create threaded binary tree
        ThreadedBinaryTree tbt = createThreadedBinaryTree(expression);
        threadBinaryTree(tbt.getRoot(), new QStack<Node*>);

        // add correct number of columns and set horizontal headers labels
        QStringList all_terminals = expression.getTerminals();
        all_terminals.sort();

        count_terminals = all_terminals.length();
        table->setHorizontalHeaderItem(0, new QTableWidgetItem(all_terminals[0]));
        for(int i = 1; i < count_terminals; i++)
            table->insertColumn(0);
        for(int i = 0; i < count_terminals; i++)
            table->setHorizontalHeaderItem(i, new QTableWidgetItem(all_terminals[i]));

        // set first vertical header label
        current_nonterminal = 'A';
        table->setVerticalHeaderItem(0, new QTableWidgetItem((QString)current_nonterminal));

        // get composition of initial state
        QList<Node*> composition, visited_nodes;
        composition = createComposition(composition, visited_nodes, tbt.getRoot(), "descending");
        all_compositions.push_back(composition);

        // create new states and their transitions
        current_nonterminal = static_cast<char>(current_nonterminal + 1);
        while((int)current_row < table->rowCount()){
            for(int i = 0; i < count_terminals; i++){
                QList<Node*> nodes = getNodesFromComposition(all_compositions[current_row], table->horizontalHeaderItem(i)->text());

                // check if transition for current nonterminal with current terminal exists (is not a '-'),
                if(!nodes.isEmpty()){

                    // set table's item (but it may change later, in case the nonterminal used for this
                    // transition already exists)
                    table->setItem(current_row, i, new QTableWidgetItem((QString)current_nonterminal));

                    // try to create new line for current transition and its composition

                    // create composition of current state
                    composition.clear();
                    for(int j = 0; j < nodes.length(); j++){
                        visited_nodes.clear();
                        composition = createComposition(composition, visited_nodes, nodes[j]->getRightChild(), "ascending");
                    }
                    // check if state already exists
                    int i_existing_state = compContainsComp(all_compositions, composition);
                    if(i_existing_state != -1){
                        table->setItem(current_row, i, new QTableWidgetItem(table->verticalHeaderItem(i_existing_state)->text()));
                    }else{
                        table->insertRow(table->rowCount());
                        table->setVerticalHeaderItem(table->rowCount()-1, new QTableWidgetItem((QString)current_nonterminal));
                        all_compositions.push_back(composition);
                        current_nonterminal = static_cast<char>(current_nonterminal + 1);
                    }
                }else{
                    table->setItem(current_row, i, new QTableWidgetItem("-"));
                    }
            }
            current_row++;
        }

        // set initial and final states
        table->setVerticalHeaderItem(0, new QTableWidgetItem("->" + table->verticalHeaderItem(0)->text()));
        setFinalStates(table, all_compositions);

        // set current_leaf_id to 1 againt
        current_leaf_id = 0;
    }
}

void FiniteAutomataGenerator::generateFA(FiniteAutomata *fa, RegularGrammar rg){
    QList<QStringList> all_productions = rg.getProductions();
    QTableWidget *table = fa->getTransitionTable();
    QStringList terminals;
    QList<int> final_states_index;

    terminals = rg.getVt();
    terminals.sort();

    // delete extra columns and rows of table
    for(int i = table->columnCount(); i > 0; i--)
        table->removeColumn(i);
    for(int i = table->rowCount(); i > 0; i--)
        table->removeRow(i);
    table->setItem(0,0, new QTableWidgetItem(""));

    table->setVerticalHeaderItem(0, new QTableWidgetItem("->" + all_productions[0][0]));

    // create the correct number of rows and columns
    for(int i = 1; i < all_productions.length(); i++){
        table->insertRow(i);
        table->setVerticalHeaderItem(i, new QTableWidgetItem(all_productions[i][0]));
    }
    for(int i = 1; i < terminals.length(); i++){
        table->insertColumn(i);
        table->setHorizontalHeaderItem(i, new QTableWidgetItem(terminals[i]));
    }

    // check if regular grammar generates only the empty sentence
    if(all_productions.length() == 1 && all_productions[0].length() == 2 && all_productions[0][1].compare("&") == 0){
        table->setItem(0,0, new QTableWidgetItem("-"));
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("a"));
        final_states_index.push_back(0);
    }else{

        bool is_new_final_state_createad = false;
        char letter;

        table->setHorizontalHeaderItem(0, new QTableWidgetItem(terminals[0]));

        // create new states and their transitions for each production
        for(int i = 0; i < all_productions.length(); i++){
            for(int j = 1; j < all_productions[i].length(); j++){
                if(all_productions[i][j][0] == '&'){
                    // set state as final
                    if(!final_states_index.contains(i))
                        final_states_index.push_back(i);
                }else{

                    // production has the format A -> a
                    if(all_productions[i][j].length() == 1 && all_productions[i][j].compare("&") != 0){

                        // when there is production like A -> a, create a final state if not created yet
                        if(is_new_final_state_createad == false){
                            letter = fa->getNewStateLabel();
                            table->insertRow(table->rowCount());
                            table->setVerticalHeaderItem(table->rowCount()-1, new QTableWidgetItem("*" + (QString)letter));
                            is_new_final_state_createad = true;
                        }
                        // add transition to the correct column
                        for(int k = 0; k < table->columnCount(); k++){
                            if(table->horizontalHeaderItem(k)->text().compare((QString)all_productions[i][j][0]) == 0){
                                if(table->item(i,k) == NULL)
                                    table->setItem(i,k, new QTableWidgetItem((QString)letter));
                                else{
                                    if(table->item(i,k)->text().compare("") != 0)
                                        table->setItem(i,k, new QTableWidgetItem(table->item(i,k)->text() + "," + (QString)letter));
                                    else
                                        table->setItem(i,k, new QTableWidgetItem((QString)letter));
                                }

                            }
                        }
                    }else{

                        // production has the format A -> aB

                        // add transition to the correct column
                        for(int k = 0; k < table->columnCount(); k++){
                            if(table->horizontalHeaderItem(k)->text().compare((QString)all_productions[i][j][0]) == 0){
                                if(table->item(i,k) == NULL)
                                    table->setItem(i,k, new QTableWidgetItem((QString)all_productions[i][j][1]));
                                else{
                                    if(table->item(i,k)->text().compare("") != 0)
                                        table->setItem(i,k, new QTableWidgetItem(table->item(i,k)->text() + "," + (QString)all_productions[i][j][1]));
                                    else
                                        table->setItem(i,k, new QTableWidgetItem((QString)all_productions[i][j][1]));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // add '*' to final states
    for(int i = 0; i < final_states_index.length(); i++){
        int index = final_states_index[i];
        table->setVerticalHeaderItem(index, new QTableWidgetItem("*" + table->verticalHeaderItem(index)->text()));
    }

    // set empty transitions to '-'
    for(int i = 0; i < table->rowCount(); i++){
        for(int j = 0; j < table->columnCount(); j++){
            if(table->item(i,j) == NULL)
                table->setItem(i, j, new QTableWidgetItem("-"));
        }
    }
}

/*------------------------------------------------
 *              PRIVATE FUNCTIONS
 *------------------------------------------------
*/

/*
 * Create a ThreaderBinaryTree using a RegularExpression
*/
ThreadedBinaryTree FiniteAutomataGenerator::createThreadedBinaryTree(RegularExpression expression){

    // if expression is valid, then create TBT
    if(expression.validateExpression()){
        ThreadedBinaryTree tbt;
        Node *current_root;
        QList<QString> all_expressions;             // store all parenthesis expressions
        QList<QString> all_subexpression_IDs;       // for each parenthesis expression, there will be a token (ID)
        QList<QString> temp_list;
        RegularExpression current_reg_expression;   // current RegularExpression (in most cases is a parenthesis expression)
        QString current_str_expression;             // current expression stored in a QString
        int current_expression_index = -1;          // index of current parenthesis expression (used to access all_expressions and all_subexpression_IDs)
        int current_operator_index;                 // root index (i.e. an index of an operator which is the current root) from current expression
        QString token;

        current_leaf_id = 1;
        current_reg_expression = expression;

        // begin of a loop through all subexpressions
        do{

            // get parenthesis expressions from current regular expression
            temp_list = current_reg_expression.findSubExpressions();
            current_reg_expression.setExpression(temp_list[0]);
            temp_list.pop_front();

            // update list of parenthesis expressions, their IDs, and current string expression
            all_expressions = all_expressions + temp_list;
            current_str_expression = current_reg_expression.getExpression();
            all_subexpression_IDs = createListOfSubExpIDs(all_subexpression_IDs, current_str_expression);

            /*
             * get current root node index
             *
             * if index is:
             * -1:      expression is a single terminal (e.g. 'a') -> do not create subBinaryTree
             * -2:      expression has only one operator and one terminal (e.g. 'a*') -> do not create sub Binary Tree
             * -3:      expression has an parenthesis expression ID only (e.g. '<12>')
             * -4:      expression has an parenthesis expression ID and one operator (e.g. '<132>*')
             */
            int operator_index;
            current_operator_index = findLowestPriorityOperator(current_str_expression, 0, current_str_expression.length()-1);
            switch(current_operator_index){
            case -1:
                current_root = new Node(NULL, NULL, NULL, current_str_expression, 0); // here
                break;
            case -2:
                current_root = createSubBynaryTree(current_str_expression, 0, 0, 1, 1);
                break;
            case -3:
                current_root = new Node(NULL, NULL, NULL, current_str_expression, 0);
                break;
            case -4:
                token = getToken(current_str_expression, 0);
                operator_index = token.length()-1;
                current_root = createSubBynaryTree(current_str_expression, 0, token.length()-1, token.length(), token.length());
                break;
            default:
                current_root = createSubBynaryTree(current_str_expression,0, current_operator_index-1,
                                                   current_operator_index+1, current_str_expression.length()-1);
                break;
            }

            // set root of the original tree
            if(current_expression_index == -1)
                tbt.setRoot(current_root);

            // update node with id (e.g. <123> becomes '*')
            if(current_expression_index > -1){
                QString key = all_subexpression_IDs[current_expression_index];
                Node *node = tbt.search(tbt.getRoot(), key);
                if(node->getParent()->getLeftChild() == node){
                    node->getParent()->setLeftChild(current_root);
                    current_root->setParent(node->getParent());
                }
                if(node->getParent()->getRightChild() == node){
                    node->getParent()->setRightChild(current_root);
                    current_root->setParent(node->getParent());
                }
            }

            current_expression_index++;

            // update current_expression
            if(current_expression_index < all_expressions.length())
                current_reg_expression.setExpression(all_expressions[current_expression_index]);

        }while(current_expression_index < all_expressions.length());
        return tbt;
    }
    else{
    }
    return NULL;
}

/*
 * thread a binary tree represented by its root node. Needs an empty stack to perform recursive calls
 * Also set index of all leafs
*/
void FiniteAutomataGenerator::threadBinaryTree(Node *current_root, QStack<Node*> *pending_nodes){
    if(current_root->getLabel().compare("|") == 0 || current_root->getLabel().compare(".") == 0){
        pending_nodes->push(current_root);
        threadBinaryTree(current_root->getLeftChild(), pending_nodes);
        threadBinaryTree(current_root->getRightChild(), pending_nodes);
    }else{

        // current_root is either '*' or '?', then thread its right child to the top of the stack
        if(current_root->getLabel().compare("*") == 0 || current_root->getLabel().compare("?") == 0){
            if(pending_nodes->length() > 0)
                current_root->setRightChild(pending_nodes->pop());
            pending_nodes->push(current_root);
            threadBinaryTree(current_root->getLeftChild(), pending_nodes);
        }else{
            //
            current_root->setIndex(current_leaf_id++);
            // current_root is a leaf, then thread its right child to the top of the stack
            if(pending_nodes->length() > 0){
                current_root->setRightChild(pending_nodes->pop());
            }
        }
    }
}

/*
 * create composition for one state
 *
 * Arguments:
 * composition: itens that composes the composition to be created
 * visited_nodes: nodes already visites. Used in order to avoid infinite loops when there is a '*' (star) node
 * current_node: current visited node
 * direction: node is being visited in ascending or descending direction
*/
QList<Node*> FiniteAutomataGenerator::createComposition(QList<Node*> composition, QList<Node*> visited_nodes, Node* current_node, QString direction){
    if(current_node != NULL){
        if(current_node->getLabel().compare(".") == 0){
            if(direction.compare("descending") == 0){
                composition = createComposition(composition, visited_nodes, current_node->getLeftChild(), "descending");
            }
            if(direction.compare("ascending") == 0){
                composition = createComposition(composition, visited_nodes, current_node->getRightChild(), "descending");
            }
        }else{
            if(current_node->getLabel().compare("|") == 0){
                if(direction.compare("descending") == 0){
                    composition = createComposition(composition, visited_nodes, current_node->getLeftChild(), "descending");
                    composition = createComposition(composition, visited_nodes, current_node->getRightChild(), "descending");
                }
                if(direction.compare("ascending") == 0){
                    Node *parent = findNextRightParent(current_node);
                    Node *right_child = current_node->getRightChild();
                    while(right_child->getRightChild() != NULL && right_child->getRightChild() != parent){
                        right_child = right_child->getRightChild();
                    }
                    if(right_child->getRightChild() == NULL){
                        if(!composition.contains(right_child->getRightChild()))
                            composition.push_back(right_child->getRightChild());
                    }else{
                        composition = createComposition(composition, visited_nodes, right_child->getRightChild(), "ascending");
                    }
                }
            }else{
                if(current_node->getLabel().compare("?") == 0){
                    if(direction.compare("descending") == 0){
                        if(!visited_nodes.contains(current_node)){
                            // add to visiteds
                            visited_nodes.push_back(current_node);

                            composition = createComposition(composition, visited_nodes, current_node->getLeftChild(), "descending");
                            composition = createComposition(composition, visited_nodes, current_node->getRightChild(), "ascending");
                        }
                    }
                    if(direction.compare("ascending") == 0){
                        if(!visited_nodes.contains(current_node)){
                            // add to visiteds
                            visited_nodes.push_back(current_node);
                            composition = createComposition(composition, visited_nodes, current_node->getRightChild(), "ascending");
                        }
                    }
                }else{
                    if(current_node->getLabel().compare("*") == 0){
                        if(direction.compare("descending") == 0 || direction.compare("ascending") == 0){
                            if(!visited_nodes.contains(current_node)){
                                // add to visiteds
                                visited_nodes.push_back(current_node);
                                composition = createComposition(composition, visited_nodes, current_node->getLeftChild(), "descending");
                                composition = createComposition(composition, visited_nodes, current_node->getRightChild(), "ascending");
                            }
                        }
                    }else{
                        // leaf
                        if(!composition.contains(current_node))
                            composition.push_back(current_node);
                        return composition;
                    }
                }
            }
        }
    }else{
        if(!composition.contains(current_node))
            composition.push_back(current_node);
        return composition;
    }
    return composition;
}

/*
 * create left and right sub binary trees of a given root node from a regular expression
*/
Node* FiniteAutomataGenerator::createSubBynaryTree(QString reg_expression,
                                           unsigned begin_left_tree,unsigned end_left_tree,
                                           unsigned begin_right_tree, unsigned end_right_tree){

    Node *current_node, *left_child, *right_child, *left_of_left_child, *left_of_right_child;
    unsigned i_left_operator, i_right_operator;
    unsigned i_current = end_left_tree+1;

    current_node = new Node(NULL, NULL, NULL, reg_expression[i_current]+"", 0);

     // expression has only left tree, that is, it has only one terminal
     // (or subexpression ID) and one operator
    if(reg_expression[i_current].toLatin1() == '*' || reg_expression[i_current].toLatin1() == '?'){

        // terminal is not an ID of a subexpression (e.g. is not <1>)
        if(end_left_tree - begin_left_tree == 0){
            left_child = new Node(current_node, NULL, NULL, reg_expression[0]+"", 0); // here
            current_node->setLeftChild(left_child);
            left_child->setParent(current_node);
            return current_node;
        }
        // terminal is an ID of a subexpression (e.g. <1>)
        if(end_left_tree - begin_left_tree >= 2){
            left_child = new Node(current_node, NULL, NULL, reg_expression.section("", begin_left_tree, end_left_tree+1), 0); // here
            current_node->setLeftChild(left_child);
            left_child->setParent(current_node);
            return current_node;
        }
    }

     // get left child
    unsigned length_child = i_current - begin_left_tree;
    QString label;
    bool left_child_was_found = false;

    // expression is simimlar to 'a'
    if(length_child == 1){
        label = reg_expression[begin_left_tree]+"";
        left_child = new Node(current_node, NULL, NULL, label, 0); // here
        left_child_was_found = true;
    }

    // expression is simimlar to 'a*'
    if(length_child == 2){
        label = reg_expression[i_current-1]+"";
        left_child = new Node(current_node, NULL, NULL, label, 0);
        label = reg_expression[begin_left_tree]+"";
        left_of_left_child = new Node(left_child, NULL, NULL, label, 0); // here
        left_child->setLeftChild(left_of_left_child);
        left_of_left_child->setParent(left_child);
        left_child_was_found = true;
    }
    QString token = "";
    if(length_child >= 3 && reg_expression[begin_left_tree].toLatin1() == '<'){
        token = getToken(reg_expression, begin_left_tree);

        // expression is simimlar to '<123>'
        if((unsigned)token.length() == (end_left_tree - begin_left_tree + 1)){
            left_child = new Node(current_node, NULL, NULL, token, 0);
            left_child_was_found = true;
        }

        // expression is simimlar to '<123>*'
        if((unsigned)token.length() + 1 == (end_left_tree - begin_left_tree + 1)){
            label = reg_expression[end_left_tree];
            left_child = new Node(current_node, NULL, NULL, label, 0);
            left_of_left_child = new Node(left_child, NULL, NULL, token, 0);
            left_child->setLeftChild(left_of_left_child);
            left_of_left_child->setParent(left_child);
            left_child_was_found = true;
        }
    }

    // if expression is not one of the previous cases, then find left sub binary tree
    if(!left_child_was_found){
        i_left_operator = findLowestPriorityOperator(reg_expression, begin_left_tree, end_left_tree);
        left_child = createSubBynaryTree(reg_expression,
                                         begin_left_tree, i_left_operator-1, i_left_operator+1, end_left_tree);
    }

    // get right child
    length_child = end_right_tree - i_current;
    bool right_child_was_found = false;

    // expression is simimlar to 'a'
    if(length_child == 1){
        label = reg_expression[begin_right_tree]+"";
        right_child = new Node(current_node, NULL, NULL, label, 0); // here
        right_child_was_found = true;
    }

    // expression is simimlar to 'a*'
    if(length_child == 2){
        label = reg_expression[end_right_tree]+"";
        right_child = new Node(current_node, NULL, NULL, label, 0);
        label = reg_expression[end_right_tree-1]+"";
        left_of_right_child = new Node(right_child, NULL, NULL, label, 0); // here
        right_child->setLeftChild(left_of_right_child);
        left_of_right_child->setParent(right_child);
        right_child_was_found = true;
    }

    token = "";
    if(length_child >= 3 && reg_expression[begin_right_tree].toLatin1() == '<'){
        token = getToken(reg_expression, begin_right_tree);

        // expression is simimlar to '<123>'
        if((unsigned)token.length() == (end_right_tree - begin_right_tree + 1)){
            right_child = new Node(current_node, NULL, NULL, token, 0);
            right_child_was_found = true;
        }

        // expression is simimlar to '<123>*'
        if((unsigned)token.length() + 1 == (end_right_tree - begin_right_tree + 1)){
            label = reg_expression[end_right_tree];
            right_child = new Node(current_node, NULL, NULL, label, 0);
            left_of_right_child = new Node(right_child, NULL, NULL, token, 0);
            right_child->setLeftChild(left_of_right_child);
            left_of_right_child->setParent(right_child);
            right_child_was_found = true;
        }
    }

    // if expression is not one of the previous cases, then find left sub binary tree
    if(!right_child_was_found){
        i_right_operator = findLowestPriorityOperator(reg_expression, begin_right_tree, end_right_tree);
        right_child = createSubBynaryTree(reg_expression, begin_right_tree, i_right_operator-1,
                                          i_right_operator+1, end_right_tree);
        right_child->setParent(current_node);
    }

    // set left and right child of current node (root)
    current_node->setLeftChild(left_child);
    current_node->setRightChild(right_child);
    left_child->setParent(current_node);
    right_child->setParent(current_node);
    return current_node;
}

/*
 * try to find lowest priority operator of expression
 *
 * return values:
 * index:   index of lowest priority operator
 * -1:      expression is a single terminal (e.g. 'a')
 * -2:      expression has only one operator and one terminal (e.g. 'a*')
 * -3:      expression has an parenthesis expression ID only (e.g. '<12>')
 * -4:      expression has an parenthesis expression ID and one operator (e.g. '<132>*')
*/
int FiniteAutomataGenerator::findLowestPriorityOperator(QString re, unsigned begin, unsigned end){
    QChar lowest;
    unsigned index;
    unsigned length = end - begin + 1;
    if(length == 1)
        return -1;
    if(length == 2)
        return -2;
    if(length >= 3){
        if(re[begin].toLatin1() == '<' && getToken(re, 0).length() == re.length())
            return -3;
        if(re[begin].toLatin1() == '<' && getToken(re, 0).length() + 1 == re.length())
            return -4;
    }
    for(unsigned i = begin; i < end; i++){
        if(re[i].toLatin1() == '|' && lowest.toLatin1() != '|'){
            lowest = re[i];
            index = i;
        }
        if(re[i].toLatin1() == '.' && lowest.toLatin1() != '.' && lowest.toLatin1() != '|'){
            lowest = re[i];
            index = i;
        }
    }
    return index;
}

/*
 * create a list with all IDs of parenthesis expressions of a given regular expression
*/
QList<QString> FiniteAutomataGenerator::createListOfSubExpIDs(QList<QString> all_subexpression_IDs, QString expression){
    QString id;
    for(int i = 0; i < expression.length(); i++){
        QString token = getToken(expression, i);
        if(expression[i].toLatin1() == '<'){
            id = expression.section("", i+1, i+token.length());
            all_subexpression_IDs.push_back(id);
            i += 3;
        }
    }
    return all_subexpression_IDs;
}

/*
 * return token of a parenthesis subexpression (e.g. <123>
*/
QString FiniteAutomataGenerator::getToken(QString re, unsigned begin){
    QString token = "<";
    unsigned i = begin+1;
    do{
        token += re[i++];
    } while(re[i].toLatin1() != '>' && i < (unsigned)re.length());
    token += ">";
    return token;
}

/*
 * check if a list of composition contains a specific composition
*/
int FiniteAutomataGenerator::compContainsComp(QList<QList<Node *> > all_compositions, QList<Node *> composition){
    bool matches = true;
    for(int i = 0; i < all_compositions.length(); i++){
        if(all_compositions[i].length() == composition.length()){
            for(int j = 0; j < composition.length() && matches == true; j++){
                if(!all_compositions[i].contains(composition[j]))
                    matches = false;
            }
            if(matches == true)
                return i;
            else
                matches = true;
        }
    }
    return -1;
}

/*
 * get all nodes consisting of the nonterminal key (e.g. 1a, 3a)
*/
QList<Node*> FiniteAutomataGenerator::getNodesFromComposition(QList<Node*> composition, QString key){
    QList<Node*> nodes;
    for(int i = 0; i < composition.length(); i++){
        if(composition[i] != NULL){
            if(composition[i]->getLabel().compare(key) == 0)
                nodes.push_back(composition[i]);
        }
    }
    return nodes;
}

/*
 * find next parent of node, in which node is a right child of
 * the parent or some left child or left grandson of parent
*/
Node* FiniteAutomataGenerator::findNextRightParent(Node *node){
    Node *parent = node->getParent();
    while(parent != NULL){
        if(parent->getLeftChild() == node)
            return parent;
        Node *temp = parent;
        parent = parent->getParent();
        node = temp;
    }
    return NULL;
}

/*
 * set final states of a table that represents a FA
*/
void FiniteAutomataGenerator::setFinalStates(QTableWidget *table, QList<QList<Node *> > all_compositions){
    for(int i = 0; i < all_compositions.length(); i++){
        if(all_compositions[i].contains(NULL))
            table->setVerticalHeaderItem(i, new QTableWidgetItem("*" + table->verticalHeaderItem(i)->text()));
    }
}
