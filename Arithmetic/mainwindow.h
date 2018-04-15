#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTimer>
#include <vector>
#include <string>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QString> expression;
    QVector<QString> answer;
    string *coreExp;
    string *coreAns;
    QWidget *wSet;
    QWidget *wDisp;
    QVBoxLayout *vlayout;
    QHBoxLayout *hlayout[20];
    QLineEdit *labelExp[20];
    QLineEdit *labelAns[20];
    QLineEdit *edit[20];
    QLabel *labelConsq[20];
    QPushButton *buttonAnswer;
    QLabel *labelGrade;
    QLabel *labelHelp=new QLabel(tr("You have 20 seconds for every exercise before you cannot write your answer any more."));
    QTextEdit *editHistory;

    QPushButton *buttonGenerate;
    QLabel *labelExerciseNum;
    QLineEdit *editExerciseNum;
    QLabel *labelMaxOperator;
    QLineEdit *editMaxOperator;
    QLabel *labelMaxRange;
    QLineEdit *editMaxRange;
    QLabel *labelPrecision;
    QLineEdit *editPrecision;
    QLabel *labelFraction;
    QRadioButton *radioFraction;
    QLabel *labelDecimal;
    QRadioButton *radioDecimal;
    QLabel *labelMuldiv;
    QRadioButton *radioMuldiv;
    QLabel *labelPower;
    QRadioButton *radioPower;

    QTimer *myTimer;
    int nextForbid=0;

    void settingWindow(void);
    void randomGenerate(void);
    void readXML(void);
    void writeXML(void);
    void history(void);
    void showExpression(void);
    void showAnswer(void);
    void updateHistory(int count);
    void forbidWrite(void);
};

#endif // MAINWINDOW_H
