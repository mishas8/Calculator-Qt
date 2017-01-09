#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QToolButton>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void digetClicked(const QString &s); /// 0..9
    void commaClicked(); /// запятая
    void equalClicked(); /// равно
    void signClicked();  /// поменять знак
    void unaryOperatorClicked();  /// корень, квадрат, модуль
    void binaryOperatorClicked(); /// логарифм, степень
    void additOperatorClicked();  /// +, -
    void multOperatorClicked();   /// *, %
    void clearClicked();
    void backspaceClicked();

private:
    QSignalMapper *signalMapper;
    Ui::MainWindow *ui;
    bool operandClicked;
    QToolButton *buttons[10];
};

#endif // MAINWINDOW_H
