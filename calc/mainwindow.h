#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define DIGITS_NUM 10
#define OPERATORS_NUM 11

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
    void otherOpClicked(const QString &op);
    void commaClicked(); /// запятая
    void signClicked();  /// поменять знак
    void clear();
    void backspaceClicked();

private:
    Ui::MainWindow *ui;
    QString addOperatorPushed;
    QString multOperatorPushed;
    bool operatorPushed;
    double summand;
    double factor;

    void equalClicked();
    void errorMes(const char &c);
    void digitConnect();
    void operatorConnect();
    void calculate(double rightOperand, const QString &op);
    QString textFormat(const double &text);

};

#endif // MAINWINDOW_H
