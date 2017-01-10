#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    operandClicked = true;
    QSignalMapper *mapperDigit = new QSignalMapper(this),
                  *mapperUnary = new QSignalMapper(this),
                  *mapperBinary = new QSignalMapper(this);

    /* digits */
    for(int i=0; i<10; i++) {
        QToolButton* btn = findChild<QToolButton*>( QString("Button%1").arg(i) );
        mapperDigit->setMapping( btn, QString("%1").arg(i) );
        connect( btn, SIGNAL(clicked()), mapperDigit, SLOT(map()) );
    }

    connect( mapperDigit, SIGNAL(mapped(QString)), this, SLOT(digetClicked(QString)) );
    connect( ui->changeSignButton, SIGNAL(clicked()), this, SLOT(signClicked()) );
    connect( ui->clearButton, SIGNAL(clicked()), this, SLOT(clearClicked()) );
    connect( ui->commaButton, SIGNAL(clicked()), this, SLOT(commaClicked()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digetClicked(const QString &digit)
{
    QString text = ui->display->text();

    /* Можно будет нажать только один 0, вместо 000000.. */
    if (text == "0" && digit == "0") {
        return;
    }

    /* Если нажали на операнд, то экран очистился */
    if (operandClicked) {
        ui->display->clear();
        operandClicked = false;
    }

    ui->display->setText(text + digit);
}

void MainWindow::commaClicked()
{
    QString text = ui->display->text();

    if (operandClicked) {
        ui->display->setText("0");
    }
    if (!text.contains(",")) {
        ui->display->setText(text + ",");
    }
    operandClicked = false;
}

void MainWindow::equalClicked(){

}

void MainWindow::signClicked()
{
    QString text = ui->display->text();
    double value = text.toDouble();
    if (value > 0) {
        ui->display->setText(text.insert(0, '-') );
    } else if (value < 0) {
        ui->display->setText(text.remove(0,1));
    }
}

void MainWindow::unaryOperatorClicked(){

}

void MainWindow::binaryOperatorClicked(){

}

/*void MainWindow::additOperatorClicked(){

}

void MainWindow::multOperatorClicked(){

}*/

void MainWindow::clearClicked()
{
    ui->display->setText("0");
    operandClicked = true;
}

void MainWindow::backspaceClicked(){

}
