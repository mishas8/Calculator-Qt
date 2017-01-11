#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include <QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    summand = 0.0;
    factor = 0.0;
    operatorPushed = true;
    QSignalMapper *mapperDigit = new QSignalMapper(this),
                  *unarMapper = new QSignalMapper(this),
                  *addMapper = new QSignalMapper(this),
                  *multMapper = new QSignalMapper(this);

    /* digits */
    for(int i = 0; i < DIGITS_NUM; i++) {
        QToolButton* btn = findChild<QToolButton*>( QString("Button%1").arg(i) );
        mapperDigit->setMapping( btn, QString("%1").arg(i) );
        connect( btn, SIGNAL(clicked()), mapperDigit, SLOT(map()) );
    }

    /* unary operators */
    QString operators[] = {"exp", "sqrt", "pov2", "fact", "rec", "ln" , "plus", "sub", "mult", "div"};
    for(int i = 0; i < OPERATORS_NUM; i++) {
        if (i < 6){
            QToolButton* unarBtn = findChild<QToolButton*>( QString("%1Button").arg(operators[i]) );
            unarMapper->setMapping( unarBtn, QString("%1").arg(operators[i]) );
            connect( unarBtn, SIGNAL(clicked()), unarMapper, SLOT(map()) );
        } else if (i < 8){
            QToolButton* addBtn = findChild<QToolButton*>( QString("%1Button").arg(operators[i]) );
            addMapper->setMapping( addBtn, QString("%1").arg(operators[i]) );
            connect( addBtn, SIGNAL(clicked()), addMapper, SLOT(map()) );
        } else if (i < 10) {
            QToolButton* multBtn = findChild<QToolButton*>( QString("%1Button").arg(operators[i]) );
            multMapper->setMapping( multBtn, QString("%1").arg(operators[i]) );
            connect( multBtn, SIGNAL(clicked()), multMapper, SLOT(map()) );
        }

    }

    connect( mapperDigit, SIGNAL(mapped(QString)), this, SLOT(digetClicked(QString)) );
    connect( unarMapper, SIGNAL(mapped(QString)), this, SLOT(unaryOpClicked(QString)) );
    connect( addMapper, SIGNAL(mapped(QString)), this, SLOT(addOpClicked(QString)) );
    connect( multMapper, SIGNAL(mapped(QString)), this, SLOT(multOpClicked(QString)) );
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
    if (operatorPushed) {
        ui->display->clear();
        operatorPushed = false;
    }

    ui->display->setText(text + digit);
}

void MainWindow::commaClicked()
{
    QString text = ui->display->text();

    if (operatorPushed) {
        ui->display->setText("0");
    }
    if (!text.contains(",")) {
        ui->display->setText(text + ",");
    }
    operatorPushed = false;
}

void MainWindow::equalClicked(){

}

void MainWindow::signClicked()
{
    QString text = ui->display->text();
    double value = QLocale::system().toDouble(text);
    if (value > 0) {
        ui->display->setText(text.insert(0, '-') );
    } else if (value < 0) {
        ui->display->setText(text.remove(0,1));
    }
}

void MainWindow::unaryOpClicked(const QString &op)
{
    double value = QLocale::system().toDouble(ui->display->text());
    double result = 0.0;

    if (op == "sqrt"){
        if (value < 0.0) {
            errorMessage.setText("Отрицательное число под корнем!");
            errorMessage.exec();
            return;
        }
        result = sqrt(value);
    } else  if (op == "exp") {
        result = exp(value);
    } else if (op == "pov2") {
        result = value*value;
    } else if (op == "fact") {
        bool isInt = (value == int(value));
        if (value >= 0 && isInt){
            for (result = 1.0; value > 1; result *= (value--))
                ;
        } else {
            errorMessage.setText("Факториал для этого аргумента не определен!");
            errorMessage.exec();
            return;
        }

    } else if (op == "rec") {
        if (!value) {
            errorMessage.setText("Ноль в знаменателе!");
            errorMessage.exec();
            return;
        }
        result = 1.0 / value;
    } else if (op == "ln"){
        if (value <= 0.0) {
            errorMessage.setText("Логарифм от неположительного аргумента!");
            errorMessage.exec();
            return;
        }
        result = log(value);
    }
    ui->display->setText(QLocale::system().toString(result));
    operatorPushed = true;
}

void MainWindow::addOpClicked(const QString &op){

}

void MainWindow::multOpClicked(const QString &op){

}

void MainWindow::clearClicked()
{
    ui->display->setText("");
    operatorPushed = true;
}

void MainWindow::backspaceClicked(){

}
