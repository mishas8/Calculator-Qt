#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digetClicked(){

}

void MainWindow::commaClicked(){

}

void MainWindow::equalClicked(){

}

void MainWindow::signClicked(){

}

void MainWindow::unaryOperatorClicked(){

}

void MainWindow::binaryOperatorClicked(){

}

void MainWindow::additOperatorClicked(){

}

void MainWindow::multOperatorClicked(){

}

void MainWindow::clearClicked(){

}

void MainWindow::backspaceClicked(){

}
