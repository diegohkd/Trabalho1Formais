#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "finiteautomatagenerator.h"
#include "threadedbinarytree.h"
#include "node.h"
#include "finiteautomata.h"
#include "controller.h"
#include <QMainWindow>
#include <iostream>
#include <QTableWidget>
#include <QInputDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_RGtoFA_clicked();

    void on_pushButton_REtoFA_clicked();

    void on_pushButton_determinize_clicked();

    void on_pushButton_clicked();

    void on_pushButton_pattern_clicked();

    void double_clicked_vertical_header(int);

    void double_clicked_horizontal_header(int);

    void on_pushButton_new_row_clicked();

    void on_pushButton_delete_row_clicked();

    void on_pushButton_new_column_clicked();

    void on_pushButton_delete_column_clicked();

    void on_pushButton_final_clicked();

    void on_pushButton_not_final_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_actionAbrir_triggered();

    void on_actionSalvar_triggered();

    void on_pushButton_pattern_2_clicked();

    void on_pushButton_copy_to_M1_clicked();

    void on_pushButton_copy_to_M2_clicked();

    void update_output_text_cursor();

private:
    Controller c;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
