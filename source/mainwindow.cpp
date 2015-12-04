#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finiteautomatagenerator.h"
#include "regularexpression.h"
#include "controller.h"
#include "regulargrammar.h"
#include <QList>
#include <QFileDialog>
#include <qfile.h>
#include <qtextstream.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    c.createAndSetFA(ui->tableWidget_1, 0);
    c.createAndSetFA(ui->tableWidget_2, 1);
    c.createAndSetFA(ui->tableWidget_3, 2);
    c.createAndSetFA(ui->tableWidget_4, 3);
    c.createAndSetFA(ui->tableWidget_5, 4);
    c.createAndSetFA(ui->tableWidget_6, 5);
    c.createAndSetFA(ui->tableWidget_7, 6);
    c.createAndSetFA(ui->tableWidget_8, 7);
    c.createAndSetFA(ui->tableWidget_9, 8);

    connect(ui->tableWidget_1->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_2->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_3->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_4->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_5->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_6->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_7->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_8->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));
    connect(ui->tableWidget_9->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_vertical_header(int)));

    connect(ui->tableWidget_1->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_2->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_3->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_4->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_5->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_6->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_7->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_8->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
    connect(ui->tableWidget_9->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),this, SLOT(double_clicked_horizontal_header(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_RGtoFA_clicked()
{
    QString text = ui->textEdit_RG->toPlainText();

    if(text.compare("") != 0){
        c.createAndSetRegularGrammar(text);
        c.generateFAFromRG(ui->tabWidget->currentIndex());
    }
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    update_output_text_cursor();    
    ui->textEdit_output->insertHtml("<font color=\"Green\">A GR foi transformada em AF e pode ser vista em M"+afNumber+".</font><br>");

}

void MainWindow::on_pushButton_REtoFA_clicked()
{
    QString text = ui->lineEdit_RE->text();

    if(text.compare("") != 0){
        c.createAndSetRegularExpression(text);
        c.generateFAFromRE(ui->tabWidget->currentIndex());
    }
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    update_output_text_cursor();    
    ui->textEdit_output->insertHtml("<font color=\"Green\">A ER foi transformada em AFD e pode ser vista em M"+afNumber+".</font><br>");

}

void MainWindow::on_pushButton_determinize_clicked()
{
    bool result = c.determinizeFA(ui->tabWidget->currentIndex());
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    update_output_text_cursor();    
    if(result){
        ui->textEdit_output->insertHtml("<font color=\"Red\">Autômato M"+afNumber+" já esta determinizado.</font><br>");
    } else {
        ui->textEdit_output->insertHtml("<font color=\"Green\">Autômato M"+afNumber+" determinizado.</font><br>");
    }

    
}

void MainWindow::on_pushButton_clicked()
{
    bool result = c.minimizeFA(ui->tabWidget->currentIndex());
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    update_output_text_cursor();
    if(result){
        ui->textEdit_output->insertHtml("<font color=\"Red\">Autômato M"+afNumber+" já é minimo.</font><br>");
    } else {
        ui->textEdit_output->insertHtml("<font color=\"Green\">Autômato M"+afNumber+" minimizado.</font><br>");
    }


}


void MainWindow::on_pushButton_pattern_clicked()
{
    update_output_text_cursor();
   QList<QList<int> > old_list;
   QString begin_html = "<font color=\"Red\">";
   QString end_html = "</font>";

   QString text_pattern = ui->lineEdit_ER_pattern->text();

   if(text_pattern.compare("") != 0){
       QString text = ui->textEdit_pattern_text->toPlainText();
       ui->textEdit_pattern_text->clear();
       ui->textEdit_pattern_text->insertPlainText(text);
       if(text.compare("") != 0){
           QList<QList<int> > found_patterns_index;
           old_list = c.findTextPattern(text_pattern, text);

           if(old_list.isEmpty() == false)
               found_patterns_index = c.formatFontPatterns(old_list);

           if(found_patterns_index.isEmpty()){
               ui->textEdit_output->insertPlainText("Nenhum padrão encontrado!\n");
           }else{
               for(int i = 0; i < found_patterns_index.length(); i++){
                   if(found_patterns_index[i].length() > 1){
                       int begin = found_patterns_index[i][0];
                       int end = found_patterns_index[i][found_patterns_index[i].length()-1];

                       QTextCharFormat fmt;
                       fmt.setBackground(Qt::yellow);

                       QTextCursor cursor(ui->textEdit_pattern_text->document());
                       cursor.setPosition(begin, QTextCursor::MoveAnchor);
                       cursor.setPosition(end+1, QTextCursor::KeepAnchor);
                       cursor.setCharFormat(fmt);
                   }
               }
               // show found patterns to user
               text = ui->textEdit_pattern_text->toPlainText();
               QStringList all_paterns;
               ui->textEdit_output->insertHtml("Padrões encontrados:<br>");
               for(int i = 0; i < old_list.length(); i++){
                   if(old_list[i].length() > 1){
                       int b = old_list[i][0];
                       for(int j = 1; j < old_list[i].length(); j++){
                           int e = old_list[i][j];

                           QString pattern;
                           pattern = text.mid(b, e-b+1);
                           if(!all_paterns.contains(pattern)){
                               ui->textEdit_output->insertHtml(begin_html + pattern + end_html + "<br>");
                               all_paterns.push_back(pattern);
                           }
                       }
                   }
               }
               ui->textEdit_output->insertPlainText("\n");
           }
       }
   }
}

void MainWindow::on_pushButton_new_row_clicked()
{
    QTableWidget * tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    default:
        break;
    }

    // check where to insert row
    QItemSelectionModel *select = tableWidget->selectionModel();
    QModelIndexList selected_rows;
    //check if there's any selection
    if(select->hasSelection()){
       //check if there's only one selected row
       selected_rows = select->selectedRows();
       if(selected_rows.length() == 1){
           //check if is not row 0 (that should be moved)
           //if not then add new row
            int rowIndex = selected_rows.at(0).row();
            if(rowIndex != 0){
                tableWidget->insertRow(rowIndex);
                tableWidget->setVerticalHeaderItem(rowIndex, new QTableWidgetItem("-"));
            }else{
                if((rowIndex == 0 and tableWidget->rowCount() == 1)){
                    tableWidget->insertRow(1);
                    tableWidget->setVerticalHeaderItem(1, new QTableWidgetItem("-"));
                }else{
                    update_output_text_cursor(); 
                    ui->textEdit_output->insertPlainText("Impossível adicionar uma linha nesta posição\n");
                }
            }
       }
       else{
            update_output_text_cursor(); 
            ui->textEdit_output->insertPlainText("Selecione somente uma linha indicando onde inserir a nova linha\n");
       }

    }
}

void MainWindow::double_clicked_vertical_header(int index){
    bool ok;
    QString current_label;
    QTableWidget *tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }
    current_label = tableWidget->verticalHeaderItem(index)->text()[tableWidget->verticalHeaderItem(index)->text().length()-1];
    QString text = QInputDialog::getText(this, tr("Nome do Estado"),
                                      tr("Insira o nome do estado"), QLineEdit::Normal,
                                      current_label, &ok);
     QMessageBox messageBox;
     messageBox.setFixedSize(500,200);
     if (ok && !text.isEmpty()){
         if(text.size() > 1){
             messageBox.critical(0,"Erro","Somente um caractere permitido!");
         }else{
             if(!text.at(0).isLetter() || text.at(0).isLower())
                 messageBox.critical(0,"Erro","Insira uma letra maíuscula!");
             else{
                 if(tableWidget->verticalHeaderItem(index)->text().contains("->")){
                     text =  "->" + text;
                 }
                if(tableWidget->verticalHeaderItem(index)->text().contains("*")){
                    text =  "*" + text;
                }
                tableWidget->verticalHeaderItem(index)->setText(text);
             }
         }
     }
}

void MainWindow::double_clicked_horizontal_header(int index){
    bool ok;
    QString current_label;
    QTableWidget *tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }
    QString text = QInputDialog::getText(this, tr("Nome da Transição"),
                                      tr("Insira o nome da transição"), QLineEdit::Normal,
                                      current_label, &ok);
     QMessageBox messageBox;
     messageBox.setFixedSize(500,200);
     if (ok && !text.isEmpty()){
         if(text.size() > 1){
             messageBox.critical(0,"Erro","Somente um caractere permitido!");
         }else{
             if(!((text.at(0).isLetter() && text.at(0).isLower())|| text.at(0).isDigit()))
                 messageBox.critical(0,"Erro","Insira uma letra minúscula ou um dígito!");
             else{
                 tableWidget->horizontalHeaderItem(index)->setText(text);
             }
         }
     }
}

void MainWindow::on_pushButton_delete_row_clicked()
{
    QTableWidget * tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }
    QItemSelectionModel *select = tableWidget->selectionModel();
    QModelIndexList selected_rows;
    //check if there's any selection
    if(select->hasSelection()){
       //check if there's only one selected row
       selected_rows = select->selectedRows();
       if(selected_rows.length() == 1){
            int rowIndex = selected_rows.at(0).row();
            if(rowIndex != 0){
                tableWidget->removeRow(rowIndex);
            }else{
                update_output_text_cursor(); 
                ui->textEdit_output->insertPlainText("Impossível remover esta linha\n");
            }
       }
       else{
            update_output_text_cursor(); 
            ui->textEdit_output->insertPlainText("Selecione somente uma linha para ser removida\n");
       }

    }
}



void MainWindow::on_pushButton_new_column_clicked()
{
    QTableWidget * tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }
    QItemSelectionModel *select = tableWidget->selectionModel();
    QModelIndexList selected_columns;
    //check if there's any selection
    if(select->hasSelection()){
       selected_columns = select->selectedColumns();
       if(selected_columns.length() == 1){
            int columnIndex = selected_columns.at(0).column();
            tableWidget->insertColumn(columnIndex);
            tableWidget->setHorizontalHeaderItem(columnIndex, new QTableWidgetItem("-"));
       }
       else{
            update_output_text_cursor(); 
            ui->textEdit_output->insertPlainText("Selecione somente uma linha indicando onde inserir a nova linha\n");
       }

    }
}

void MainWindow::on_pushButton_delete_column_clicked()
{
    QTableWidget * tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }

    QItemSelectionModel *select = tableWidget->selectionModel();
    QModelIndexList selected_columns;
    //check if there's any selection
    if(select->hasSelection()){
       //check if there's only one selected column
       selected_columns = select->selectedColumns();
       if(selected_columns.length() == 1){
            int columnIndex = selected_columns.at(0).column();
            if(tableWidget->columnCount() > 1){
                tableWidget->removeColumn(columnIndex);
            }else{
                update_output_text_cursor(); 
                ui->textEdit_output->insertPlainText("Impossível remover esta coluna\n");
            }
       }
       else{
            update_output_text_cursor(); 
            ui->textEdit_output->insertPlainText("Selecione somente uma coluna para ser removida\n");
       }

    }
}

void MainWindow::on_pushButton_final_clicked()
{
    QTableWidget * tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }
    QItemSelectionModel *select = tableWidget->selectionModel();
    QModelIndexList selected_rows;
    //check if there's any selection
    if(select->hasSelection()){
       //check if there's only one selected row
       selected_rows = select->selectedRows();
       if(selected_rows.length() == 1){
            int rowIndex = selected_rows.at(0).row();
            if(!tableWidget->verticalHeaderItem(rowIndex)->text().contains("*"))
                tableWidget->setVerticalHeaderItem(rowIndex, new QTableWidgetItem("*" + tableWidget->verticalHeaderItem(rowIndex)->text()));
       }
       else{
            update_output_text_cursor(); 
            ui->textEdit_output->insertPlainText("Selecione somente uma linha indicando qual estado será final\n");
       }
    }
}

void MainWindow::on_pushButton_not_final_clicked()
{
    QTableWidget * tableWidget;

    switch(ui->tabWidget->currentIndex()){
    case 0:
        tableWidget = ui->tableWidget_1;
        break;
    case 1:
        tableWidget = ui->tableWidget_2;
        break;
    case 2:
        tableWidget = ui->tableWidget_3;
        break;
    case 3:
        tableWidget = ui->tableWidget_4;
        break;
    case 4:
        tableWidget = ui->tableWidget_5;
        break;
    case 5:
        tableWidget = ui->tableWidget_6;
        break;
    case 6:
        tableWidget = ui->tableWidget_7;
        break;
    case 7:
        tableWidget = ui->tableWidget_8;
        break;
    case 8:
        tableWidget = ui->tableWidget_9;
        break;
    default:
        break;
    }
    QItemSelectionModel *select = tableWidget->selectionModel();
    QModelIndexList selected_rows;
    //check if there's any selection
    if(select->hasSelection()){
       //check if there's only one selected row
       selected_rows = select->selectedRows();
       if(selected_rows.length() == 1){
            int rowIndex = selected_rows.at(0).row();
            tableWidget->setVerticalHeaderItem(rowIndex, new QTableWidgetItem(tableWidget->verticalHeaderItem(rowIndex)->text().replace("*", "")));
       }
       else{
            update_output_text_cursor(); 
            ui->textEdit_output->insertPlainText("Selecione somente uma linha indicando qual estado será final\n");
       }
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    c.complementsFA(ui->tabWidget->currentIndex());
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    update_output_text_cursor(); 
    ui->textEdit_output->insertHtml("<font color=\"Green\">Autômato M"+afNumber+" complementado.</font><br>");
 
}

void MainWindow::on_pushButton_6_clicked()
{
    c.intersecFAs();
    update_output_text_cursor();  
    ui->textEdit_output->insertHtml("<font color=\"Green\">A intersecção foi feita entre M1 e M2 e o resultado final está em M6</font><br>");    

    ui->textEdit_output->insertPlainText("\nM3 = ¬M1; M4 = ¬M2;\nM5 = ¬M1U¬M2; M6 = ¬M5\n");
    update_output_text_cursor(); 
}

void MainWindow::on_actionAbrir_triggered()
{
    QString path = QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir arquivo"), path, tr("(*.tf)"));
    cout << fileName.toStdString() << endl;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){

    }
    QTextStream stream ( &file );
    QString line;
    QString rg = "", re = "";
    bool reading_RG = false, reading_RE = false;
    while(!stream.atEnd()) {
         line = stream.readLine();
         if(reading_RG == true && line.compare("<Expressao Regular>") != 0){
             rg += line + "\n";
         }
         if(reading_RE == true && line.compare("<Gramatica Regular>") != 0){
             re += line + "\n";
         }
         if(line.compare("<Gramatica Regular>") == 0){
             reading_RG = true;
             reading_RE = false;
         }
         if(line.compare("<Expressao Regular>") == 0){
             reading_RE = true;
             reading_RG = false;
         }
    }
    file.close(); // when your done.

    re.remove(re.length()-1, 2);
    rg.remove(rg.length()-1, 2);

    re.replace("\n\n", "");
    rg.replace("\n\n", "\n");
    re.replace(" ", "");
    rg.replace(" ", "");

    if(re.length() > 0)
        ui->lineEdit_RE->setText(re);
    if(rg.length() > 0){
        ui->textEdit_RG->clear();
        ui->textEdit_RG->insertPlainText(rg);
    }
}

void MainWindow::on_actionSalvar_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Salvar"),QDir::currentPath(),tr("(*.tf)") );
    if( !filename.isNull() )
    {
        if(filename.contains(".tf") == false)
            filename += ".tf";
        QFile file(filename);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&file);
            out << "<Gramatica Regular>\n";
            out << ui->textEdit_RG->toPlainText() + "\n";
            out << "<Expressao Regular>\n";
            out << ui->lineEdit_RE->text() + "\n";
    }
}

void MainWindow::on_pushButton_pattern_2_clicked()
{
    QString text = ui->textEdit_RG->toPlainText();

    if(text.compare("") != 0){
        c.createAndSetRegularGrammar(text);
    }

    QString text2 = ui->lineEdit_RE->text();

    if(text2.compare("") != 0){
        c.createAndSetRegularExpression(text2);
    }
    bool result = c.equalsFAs();
    update_output_text_cursor();
    if(result){
        ui->textEdit_output->insertHtml("<font color=\"Green\">São equivalentes</font><br>");    
    } else {
       ui->textEdit_output->insertHtml("<font color=\"Green\">Não são equivalentes</font><br>");     
    }

    ui->textEdit_output->insertPlainText("M1 = ER; M2 = GR;\nM3 = ¬M1; M4 = ¬M2;\nM5 = ¬M1UM2; M6 = ¬M2UM1;\nM7 = ¬M5; M8 = ¬M6; M9 = M7UM8 \n");
    update_output_text_cursor(); 
}

void MainWindow::on_pushButton_copy_to_M1_clicked()
{
    c.modifyFA(0, ui->tabWidget->currentIndex());
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    update_output_text_cursor();
    ui->textEdit_output->insertHtml("<font color=\"Green\">Autômato M"+afNumber+" copiado para M1.</font><br>");
}

void MainWindow::on_pushButton_copy_to_M2_clicked()
{
    c.modifyFA(1, ui->tabWidget->currentIndex());
    update_output_text_cursor();
    QString afNumber = QString::number(ui->tabWidget->currentIndex()+1);
    ui->textEdit_output->insertHtml("<font color=\"Green\">Autômato M"+afNumber+" copiado para M2.</font><br>");
}

void MainWindow::update_output_text_cursor(){
    QTextCursor c = ui->textEdit_output->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEdit_output->setTextCursor(c);
}
