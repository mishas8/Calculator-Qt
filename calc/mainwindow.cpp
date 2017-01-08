#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    //counter(0)
{
    ui->setupUi(this);

    QSignalMapper *mapper = new QSignalMapper( this );

    mapper->setMapping( ui->Button0, "0" );
    mapper->setMapping( ui->Button1, "1" );
    mapper->setMapping( ui->Button2, "2" );
    mapper->setMapping( ui->Button3, "3" );
    mapper->setMapping( ui->Button4, "4" );
    mapper->setMapping( ui->Button5, "5" );
    mapper->setMapping( ui->Button6, "6" );
    mapper->setMapping( ui->Button7, "7" );
    mapper->setMapping( ui->Button8, "8" );
    mapper->setMapping( ui->Button9, "9" );

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digetClicked(const QString &str)
{
    ui->display->setText(str);
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
