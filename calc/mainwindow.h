#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define DIGITS_NUM 10
#define OPERATORS_NUM 6

#include <QMainWindow>
#include <QSignalMapper>
#include <QToolButton>
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

signals:
    void valueChanged(int value);

private slots:
    void digetClicked(const QString &digit); /// 0..9
    void unaryOpClicked(const QString &op);
    void addOpClicked(const QString &op);
    void multOpClicked(const QString &op);
    void commaClicked(); /// запятая
    void equalClicked(); /// равно
    void signClicked();  /// поменять знак
    void clearClicked();
    void backspaceClicked();

private:
    Ui::MainWindow *ui;
    QSignalMapper *signalMapper;
    QMessageBox errorMessage;
    QString addOperatorPushed;
    QString multOperatorPushed;
    bool operatorPushed;
    double summand;
    double factor;

};

#endif // MAINWINDOW_H
