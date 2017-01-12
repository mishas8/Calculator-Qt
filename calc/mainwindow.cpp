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
    connect( ui->equalButton, SIGNAL(clicked()), this, SLOT(equalClicked()) );
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

    QString text = ui->display->text();

    if (operatorPushed) {
        ui->display->clear();
        operatorPushed = false;
    }

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

void MainWindow::addOpClicked(const QString &op)
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
}

void MainWindow::clear()
{
    summand = 0.0;
    factor = 0.0;
    addOperatorPushed.clear();
    multOperatorPushed.clear();
    ui->display->setText("0");
    operatorPushed = true;
}

void MainWindow::backspaceClicked(){

}

bool MainWindow::calculate(double rightOperand, const QString &op)
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
}

QString MainWindow::textFormat(const double &text)
{
    QString temp = QLocale::system().toString(text, 'f', 21);
    return temp.remove(QRegExp(",?0+$"));
}
