#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    operandClicked = true;
    QSignalMapper *mapper = new QSignalMapper( this );

    mapper->setMapping( ui->Button0, ui->Button0->text() );
    mapper->setMapping( ui->Button1, ui->Button1->text() );
    mapper->setMapping( ui->Button2, ui->Button2->text() );
    mapper->setMapping( ui->Button3, ui->Button3->text() );
    mapper->setMapping( ui->Button4, ui->Button4->text() );
    mapper->setMapping( ui->Button5, ui->Button5->text() );
    mapper->setMapping( ui->Button6, ui->Button6->text() );
    mapper->setMapping( ui->Button7, ui->Button7->text() );
    mapper->setMapping( ui->Button8, ui->Button8->text() );
    mapper->setMapping( ui->Button9, ui->Button9->text() );


    connect( ui->Button0, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button1, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button2, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button3, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button4, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button5, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button6, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button7, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button8, SIGNAL(clicked()), mapper, SLOT(map()) );
    connect( ui->Button9, SIGNAL(clicked()), mapper, SLOT(map()) );

    connect( mapper, SIGNAL(mapped(QString)), this, SLOT(digetClicked(QString)) );
    connect( ui->changeSignButton, SIGNAL(clicked()), this, SLOT(signClicked()) );
    connect( ui->clearButton, SIGNAL(clicked()), this, SLOT(clearClicked()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digetClicked(const QString &s)
{
       /* Можно будет нажать только один 0, вместо 000000.. */
    if (ui->display->text() == "0" && s == "0") {
        return;
    }

    /* Если нажали на операнд, то экран очистился */
    if (operandClicked) {
        ui->display->clear();
        operandClicked = false;
    }

    ui->display->setText(ui->display->text() + s);
}

void MainWindow::commaClicked(){

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

void MainWindow::additOperatorClicked(){

}

void MainWindow::multOperatorClicked(){

}

void MainWindow::clearClicked()
{
    ui->display->setText("0");
    operandClicked = true;
}

void MainWindow::backspaceClicked(){

}
