#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>


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
    void digetClicked(const QString &str); /// 0..9
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
    enum { NumDigitButtons = 10 };
};

#endif // MAINWINDOW_H
