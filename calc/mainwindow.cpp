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
    result = 0.0;
    factor = 0.0;
    operatorPushed = true;

    digitConnect();
    operatorConnect();
    connect( ui->changeSignButton, SIGNAL(clicked()), this, SLOT(signClicked()) );
    connect( ui->clearButton, SIGNAL(clicked()), this, SLOT(clear()) );
    connect( ui->backspaceButton, SIGNAL(clicked()), this, SLOT(backspaceClicked()) );
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
        operatorPushed = false;
    }

    QString text = ui->display->text();
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
            errorMes('s');
            return;
        }
        result = sqrt(value);
    } else if (op == "exp") {
        result = exp(value);
    } else if (op == "pov2") {
        result = value*value;
    } else if (op == "fact") {
        bool isInt = (value == int(value));
        if (value >= 0 && isInt){
            for (result = 1.0; value > 1; result *= (value--));
        } else {
            errorMes('f');
            return;
        }
    } else if (op == "rec") {
        if (!value) {
            errorMes('r');
            return;
        }
        result = 1.0 / value;
    } else if (op == "ln"){
        if (value <= 0.0) {
            errorMes('l');
            return;
        }
        result = log(value);
    }
    ui->display->setText(textFormat(result));
    operatorPushed = true;
}

void MainWindow::otherOpClicked(const QString &op)
{
    double curValue = QLocale::system().toDouble(ui->display->text());;

    if (op == "add" || op == "sub") {
        if (!multOperatorPushed.isEmpty()) {
            calculate(curValue, multOperatorPushed);
            ui->display->setText(textFormat(factor));
            curValue = factor;
            factor = 0.0;
            multOperatorPushed.clear();
        }
        if (!addOperatorPushed.isEmpty()) {
            calculate(curValue, addOperatorPushed);
            ui->display->setText(textFormat(summand));
        } else {
            summand = curValue;
        }
        addOperatorPushed = op;
        operatorPushed = true;
    } else

    if (op == "mult" || op == "div") {
        if (!multOperatorPushed.isEmpty()) {
            calculate(curValue, multOperatorPushed);
            ui->display->setText(textFormat(factor));
        } else {
            factor = curValue;
        }
        multOperatorPushed = op;
        operatorPushed = true;
    } else

    if (op == "eq") {
        if (!multOperatorPushed.isEmpty()) {
            calculate(curValue, multOperatorPushed);
            ui->display->setText(textFormat(factor));
            curValue = factor;
            factor = 0.0;
            multOperatorPushed.clear();
        }
        if (!addOperatorPushed.isEmpty()) {
            calculate(curValue, addOperatorPushed);
            addOperatorPushed.clear();
        } else {
            summand = curValue;
        }
        ui->display->setText(textFormat(summand));
        summand = 0.0;
        operatorPushed = true;
    }
}

void MainWindow::clear()
{
    summand = 0.0;
    result = 0.0;
    addOperatorPushed.clear();
    multOperatorPushed.clear();
    ui->display->setText("0");
    operatorPushed = true;
}

void MainWindow::backspaceClicked()
{
    if (operatorPushed)
        return;

    QString text = ui->display->text();
    text = ui->display->text().left(ui->display->text().size()-1);
    if (text.isEmpty()) {
        text = "0";
        operatorPushed = true;
    }
    ui->display->setText(text);
}

void MainWindow::calculate(double rightOperand, const QString &op)
{
    if (op == "add") {
        summand += rightOperand;
    } else if (op == "sub") {
        summand -= rightOperand;
    } else if (op == "mult") {
        factor *= rightOperand;
    } else if (op == "div") {
        if (rightOperand == 0.0) {
            errorMes('d');
        }
        factor /= rightOperand;
    }
}

void MainWindow::operatorConnect()
{
    QString operators[] = {"exp", "sqrt", "pov2", "fact", "rec", "ln" , "add", "sub", "mult", "div", "eq"};
    QSignalMapper *unarOpMapper = new QSignalMapper(this),
                  *otherOpMapper = new QSignalMapper(this);

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
    connect( unarOpMapper, SIGNAL(mapped(QString)), this, SLOT(unaryOpClicked(QString)) );
    connect( otherOpMapper, SIGNAL(mapped(QString)), this, SLOT(otherOpClicked(QString)) );
}

void MainWindow::digitConnect()
{
    QSignalMapper *mapperDigit = new QSignalMapper(this);

    for(int i = 0; i < DIGITS_NUM; i++) {
        QToolButton* btn = findChild<QToolButton*>( QString("Button%1").arg(i) );
        mapperDigit->setMapping( btn, QString("%1").arg(i) );
        connect( btn, SIGNAL(clicked()), mapperDigit, SLOT(map()) );
    }
    connect( mapperDigit, SIGNAL(mapped(QString)), this, SLOT(digetClicked(QString)) );
}

QString MainWindow::textFormat(const double &text)
{
    QString temp = QLocale::system().toString(text, 'f', 15);
    return temp.remove(QRegExp(",?0+$"));
}

void MainWindow::errorMes(const char &c)
{
    QMessageBox errorMessage;
    switch (c) {

    case 'd' :
        errorMessage.setText("Ноль в знаменателе!");
        errorMessage.exec();
        break;

    case 'f' :
        errorMessage.setText("Факториал для этого аргумента не определен!");
        errorMessage.exec();
        break;

    case 'l' :
        errorMessage.setText("Логарифм от неположительного аргумента!");
        errorMessage.exec();
        break;

    case 'r' :
        errorMessage.setText("Ноль в знаменателе!");
        errorMessage.exec();
        break;

    case 's':
        errorMessage.setText("Отрицательное число под корнем!");
        errorMessage.exec();
        break;

        default:
            break;
    }
}
