#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include <QObject>
#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->display->setText("0");
    summand = 0.0;
    factor = 0.0;
    operatorPushed = true;
    QSignalMapper *mapperDigit = new QSignalMapper(this),
                  *unarOpMapper = new QSignalMapper(this),
                  *otherOpMapper = new QSignalMapper(this);

    /* digits */
    for(int i = 0; i < DIGITS_NUM; i++) {
        QToolButton* btn = findChild<QToolButton*>( QString("Button%1").arg(i) );
        mapperDigit->setMapping( btn, QString("%1").arg(i) );
        connect( btn, SIGNAL(clicked()), mapperDigit, SLOT(map()) );
    }

    /* unary operators */
    QString operators[] = {"exp", "sqrt", "pov2", "fact", "rec", "ln" , "add", "sub", "mult", "div", "eq"};
    for(int i = 0; i < OPERATORS_NUM; i++) {
        if (i < 6){
            QToolButton* unarOpBtn = findChild<QToolButton*>( QString("%1Button").arg(operators[i]) );
            unarOpMapper->setMapping( unarOpBtn, QString("%1").arg(operators[i]) );
            connect( unarOpBtn, SIGNAL(clicked()), unarOpMapper, SLOT(map()) );
        } else {
            QToolButton* otherOpBtn = findChild<QToolButton*>( QString("%1Button").arg(operators[i]) );
            otherOpMapper->setMapping( otherOpBtn, QString("%1").arg(operators[i]) );
            connect( otherOpBtn, SIGNAL(clicked()), otherOpMapper, SLOT(map()) );
        }
    }

    connect( mapperDigit, SIGNAL(mapped(QString)), this, SLOT(digetClicked(QString)) );
    connect( unarOpMapper, SIGNAL(mapped(QString)), this, SLOT(unaryOpClicked(QString)) );
    connect( otherOpMapper, SIGNAL(mapped(QString)), this, SLOT(otherOpClicked(QString)) );
    connect( ui->changeSignButton, SIGNAL(clicked()), this, SLOT(signClicked()) );
    connect( ui->clearButton, SIGNAL(clicked()), this, SLOT(clear()) );
    connect( ui->commaButton, SIGNAL(clicked()), this, SLOT(commaClicked()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digetClicked(const QString &digit)
{
    QLineEdit *lineEdit = new QLineEdit(this);
    QDoubleValidator *v = new QDoubleValidator(this);
    v->setNotation(QDoubleValidator::StandardNotation);
    lineEdit->setValidator(v);



    if (operatorPushed) {
        ui->display->setText("");
        qDebug() << "I'm clearing!!!";
        operatorPushed = false;
    }

    QString text = ui->display->text();
    qDebug() << "It's clear!!!";
    QString s = text + digit;
    int i;

    if (text == "0" && digit != ",") {
        ui->display->setText(digit);
    } else if (lineEdit->validator()->validate(s,i) != QValidator::Invalid) {
        ui->display->setText(s);
    }
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
    ui->display->setText(textFormat(result));
    operatorPushed = true;
}

void MainWindow::otherOpClicked(const QString &op)
{
    qDebug() << "operator pushed!!!";
    double value = QLocale::system().toDouble(ui->display->text());
    if (op == "add") {
        operatorPushed = true;
        act = ADD;
        summand = value;
    }
    if (op == "sub") {
        operatorPushed = true;
        act = SUB;
        summand = value;
    }
    if (op == "mult") {
        operatorPushed = true;
        act = MULT;
        summand = value;
    }
    if (op == "div") {
        operatorPushed = true;
        act = DIV;
        summand = value;
    }
    if (op == "eq") {
        //operatorPushed = true;
        equalClicked();
        act = EQUAL;
    }
}

void MainWindow::equalClicked()
{
    double rightOperand = QLocale::system().toDouble(ui->display->text());
    double temp;

    switch (act){
    case ADD: {
        temp = summand + rightOperand;
        break;
    }
    case SUB: {
        temp = summand - rightOperand;
        break;
    }
    case MULT: {
        temp  = summand * rightOperand;
        break;
    }
    case DIV: {
        if (!rightOperand) {
            errorMessage.setText("Ноль в знаменателе!");
            errorMessage.exec();
            return;
        }
        else {
            temp = summand / rightOperand;
        }
        break;
    }
    case EQUAL: {
        ui->display->setText(textFormat(temp));
        summand = temp;
        break;
    }
    default: {
        temp = rightOperand;
        break;
    }
    }
    if (act != EQUAL)
        ui->display->setText(textFormat(temp));
}

/*void MainWindow::addOpClicked(const QString &op)
{
    qDebug() << "addOpClicked";
    double curValue = QLocale::system().toDouble(ui->display->text());

    if (!multOperatorPushed.isEmpty()) { /// если нажат мультипликативный опреатор
        qDebug() << "i'm here!multOperatorPushed";
        if (!calculate(curValue, multOperatorPushed)) { /// и нажато '='
            clear();
            errorMessage.setText("error na!");
            errorMessage.exec();
            return;
        }
        ui->display->setText(textFormat(factor)); /// множитель
        curValue = factor;
        factor = 0.0;
        multOperatorPushed.clear();
    }

    if (!addOperatorPushed.isEmpty()) { /// если нажат аддитивный оператор
        qDebug() << "i'm here addOperatorPushed!";
        if (!calculate(curValue, addOperatorPushed)) {
            clear();
            errorMessage.setText("error na!");
            errorMessage.exec();
            return;
        }
        ui->display->setText(textFormat(summand));
    } else {
        summand = curValue;
    }

    addOperatorPushed = op;
    operatorPushed = true;
}

void MainWindow::multOpClicked(const QString &op)
{
    qDebug() << "multOpClicked";
    double curValue = QLocale::system().toDouble(ui->display->text());

    if (!addOperatorPushed.isEmpty()) { ///  если нажат аддитивный оператор
        if (!calculate(curValue, multOperatorPushed)) {
            clear();
            errorMessage.setText("error na!");
            errorMessage.exec();
            return;
        }
        ui->display->setText(textFormat(factor));
    } else {
        factor = curValue;
        multOperatorPushed = op;
        operatorPushed = true;
    }
}

void MainWindow::equalClicked()
{
    qDebug() << "equalOpClicked";
    double curValue = QLocale::system().toDouble(ui->display->text());

    if (!multOperatorPushed.isEmpty()) { /// если нажат мультипликативный опреатор
        if (!calculate(curValue, multOperatorPushed)) { /// и нажато '='
            clear();
            errorMessage.setText("error na!");
            errorMessage.exec();
            return;
        }
        ui->display->setText(textFormat(factor)); /// множитель
        curValue = factor;
        factor = 0.0;
        multOperatorPushed.clear();
    }

    if (!addOperatorPushed.isEmpty()) { /// если нажат аддитивный оператор
        if (!calculate(curValue, addOperatorPushed)) {
            clear();
            errorMessage.setText("error na!");
            errorMessage.exec();
            return;
        }
        addOperatorPushed.clear();
    } else {
        summand = curValue;
    }
    ui->display->setText(textFormat(summand));
    summand = 0.0;
    operatorPushed = true;
}*/

void MainWindow::clear()
{
    summand = 0.0;
    factor = 0.0;
    addOperatorPushed.clear();
    multOperatorPushed.clear();
    ui->display->setText("0");
    operatorPushed = true;
}

void MainWindow::backspaceClicked()
{

}

/*bool MainWindow::calculate(double rightOperand, const QString &op)
{
    if (op == "plus") {
        summand += rightOperand;
    } else if (op == "sub") {
        summand -= rightOperand;
    } else if (op == "mult") {
        factor *= rightOperand;
    } else if (op == "div") {
        if (rightOperand == 0.0)
            return false;
        factor /= rightOperand;
    }
    return true;
}*/

QString MainWindow::textFormat(const double &text)
{
    QString temp = QLocale::system().toString(text, 'f', 21);
    return temp.remove(QRegExp(",?0+$"));
}
