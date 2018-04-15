#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include <string>
#include <vector>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QtCore/QCoreApplication>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QString>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Arithmetic"));
    resize(500,500);

    QAction *generateAction=new QAction(QIcon(":/images/neptune"),tr("Generate"),this);
    generateAction->setStatusTip(tr("Generate new exercises randomly"));
    generateAction->setShortcut(tr("ctrl+g"));

    QAction *readXMLAction = new QAction(QIcon(":/images/jupiter.png"), tr("Review"), this);
    readXMLAction->setStatusTip(tr("Review the wrong exercises"));
    readXMLAction->setShortcut(tr("ctrl+r"));

    QAction *writeXMLAction = new QAction(QIcon(":/images/venus.png"), tr("Save"), this);
    writeXMLAction->setStatusTip(tr("Save the wrong exercises"));
    writeXMLAction->setShortcut(tr("ctrl+w"));

    QAction *historyAction = new QAction(QIcon(":/images/mars.png"), tr("History"), this);
    historyAction->setStatusTip(tr("Show history"));
    historyAction->setShortcut(tr("ctrl+h"));

    QAction *closeAction=new QAction(QIcon(":/images/mercury"),tr("Close"),this);
    closeAction->setStatusTip(tr("Close the application"));
    closeAction->setShortcut(tr("esc"));

    QMenu *menu = menuBar()->addMenu(tr("Function"));
    menu->addAction(generateAction);
    menu->addAction(readXMLAction);
    menu->addAction(writeXMLAction);
    menu->addAction(historyAction);
    menu->addAction(closeAction);

    QToolBar *toolBar = addToolBar(tr("Function"));
    toolBar->addAction(generateAction);
    toolBar->addAction(readXMLAction);
    toolBar->addAction(writeXMLAction);
    toolBar->addAction(historyAction);
    toolBar->addAction(closeAction);

    statusBar()->showMessage(tr("Ready"));

    connect(generateAction,&QAction::triggered,this,&MainWindow::settingWindow);
    connect(readXMLAction,&QAction::triggered,this,&MainWindow::readXML);
    connect(writeXMLAction,&QAction::triggered,this,&MainWindow::writeXML);
    connect(historyAction,&QAction::triggered,this,&MainWindow::history);
    connect(closeAction,&QAction::triggered,this,&MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::settingWindow()
{
    wSet=new QWidget;
    vlayout=new QVBoxLayout;

    labelExerciseNum=new QLabel("Exercise Num");
    editExerciseNum=new QLineEdit;
    editExerciseNum->setPlaceholderText("10");
    hlayout[0]=new QHBoxLayout;
    hlayout[0]->addWidget(labelExerciseNum);
    hlayout[0]->addWidget(editExerciseNum);
    vlayout->addItem(hlayout[0]);

    labelMaxOperator=new QLabel("Max Operator Num");
    editMaxOperator=new QLineEdit;
    editMaxOperator->setPlaceholderText("5");
    hlayout[1]=new QHBoxLayout;
    hlayout[1]->addWidget(labelMaxOperator);
    hlayout[1]->addWidget(editMaxOperator);
    vlayout->addItem(hlayout[1]);

    labelMaxRange=new QLabel("Range of Numbers");
    editMaxRange=new QLineEdit;
    editMaxRange->setPlaceholderText("1000");
    hlayout[2]=new QHBoxLayout;
    hlayout[2]->addWidget(labelMaxRange);
    hlayout[2]->addWidget(editMaxRange);
    vlayout->addItem(hlayout[2]);

    labelPrecision=new QLabel("Precision of Decimals");
    editPrecision=new QLineEdit;
    editPrecision->setPlaceholderText("2");
    hlayout[3]=new QHBoxLayout;
    hlayout[3]->addWidget(labelPrecision);
    hlayout[3]->addWidget(editPrecision);
    vlayout->addItem(hlayout[3]);

    labelFraction=new QLabel("Has Fraction");
    radioFraction=new QRadioButton("Fraction");
    radioFraction->setChecked(false);
    radioFraction->setAutoExclusive(false);
    hlayout[4]=new QHBoxLayout;
    hlayout[4]->addWidget(labelFraction);
    hlayout[4]->addWidget(radioFraction);
    vlayout->addItem(hlayout[4]);

    labelDecimal=new QLabel("Has Decimal");
    radioDecimal=new QRadioButton("Decimal");
    radioDecimal->setChecked(false);
    radioDecimal->setAutoExclusive(false);
    hlayout[5]=new QHBoxLayout;
    hlayout[5]->addWidget(labelDecimal);
    hlayout[5]->addWidget(radioDecimal);
    vlayout->addItem(hlayout[5]);

    labelMuldiv=new QLabel("Has Multiply/Divide");
    radioMuldiv=new QRadioButton("* /");
    radioMuldiv->setChecked(true);
    radioMuldiv->setAutoExclusive(false);
    hlayout[6]=new QHBoxLayout;
    hlayout[6]->addWidget(labelMuldiv);
    hlayout[6]->addWidget(radioMuldiv);
    vlayout->addItem(hlayout[6]);

    labelPower=new QLabel("Has Power");
    radioPower=new QRadioButton("^");
    radioPower->setChecked(false);
    radioPower->setAutoExclusive(false);
    hlayout[7]=new QHBoxLayout;
    hlayout[7]->addWidget(labelPower);
    hlayout[7]->addWidget(radioPower);
    vlayout->addItem(hlayout[7]);

    buttonGenerate=new QPushButton("Random Generate");
    vlayout->addWidget(buttonGenerate);
    wSet->setLayout(vlayout);
    wSet->show();

    connect(buttonGenerate,&QPushButton::clicked,this,&MainWindow::randomGenerate);
}

void MainWindow::randomGenerate()
{
    expression.clear();
    answer.clear();

    coreExp=new string();
    coreAns=new string();

    int exerciseNum=editExerciseNum->text().toInt();
    if(exerciseNum==0)
        exerciseNum=10;
    else if(exerciseNum>20)
    {
        QMessageBox::warning(this,tr("Exercise Num"),tr("Please input exercise num <= 20"));
        return;
    }

    int maxOperator=editMaxOperator->text().toInt();
    if(maxOperator==0)
        maxOperator=5;

    int maxRange=editMaxRange->text().toInt();
    if(maxRange==0)
        maxRange=1000;
    else if(maxRange<20)
    {
        QMessageBox::warning(this,tr("Max Range"),tr("Please input max range >= 20"));
        return;
    }

    int precision=editPrecision->text().toInt();
    if(precision==0)
        precision=2;

    int fraction=radioFraction->isChecked()?1:0;
    int decimal=radioDecimal->isChecked()?1:0;
    int muldiv=radioMuldiv->isChecked()?1:0;
    int power=radioPower->isChecked()?1:0;

    set_setting(maxOperator,maxRange,precision,fraction,decimal,muldiv,power);

    for(int i=0;i<exerciseNum;i++)
    {
        generate(coreExp,coreAns);
        expression.push_back(QString::fromStdString(*coreExp));
        answer.push_back(QString::fromStdString(*coreAns));
    }

    wSet->hide();
    showExpression();
}

void MainWindow::writeXML()
{
    if(expression.size()==0)
    {
        QMessageBox::warning(this,tr("File"),tr("No exercises."));
        return;
    }

    QString path = QFileDialog::getSaveFileName(this,tr("Save File"),"/",tr("XML Files(*.xml)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("File"),tr("Cannot Save File."));
            return ;
        }

        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);

        writer.writeStartDocument();
        writer.writeStartElement("exercise");

        for(int i=0;i<expression.size();i++)
        {
            if(QString::compare(labelConsq[i]->text(),QStringLiteral("Wrong"))==0)
            {
                writer.writeTextElement("expression",expression[i]);
                writer.writeTextElement("answer",answer[i]);
            }

        }

        writer.writeEndElement();
        writer.writeEndDocument();
        file.close();
    }

    else {
        QMessageBox::warning(this, tr("Path"),tr("You did not save any file."));
        return;
    }
}

void MainWindow::readXML()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),"/",tr("XML Files(*.xml)"));
        if(!path.isEmpty()) {
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("File"),tr("Cannot Open File."));
                return ;
            }

            expression.clear();
            answer.clear();

            QXmlStreamReader reader;
            reader.setDevice(&file);

            while (!reader.atEnd()) {

                QXmlStreamReader::TokenType type = reader.readNext();

                if (type == QXmlStreamReader::StartElement) {
                    QString elementName=reader.name().toString();
                    if(QString::compare(elementName,QStringLiteral("expression"))==0)
                        expression.push_back(reader.readElementText());
                    if(QString::compare(elementName,QStringLiteral("answer"))==0)
                        answer.push_back(reader.readElementText());
                }

            }

            if (reader.hasError()) {
                QMessageBox::warning(this, tr("XML"),reader.errorString());
            }

            file.close();

            if(expression.size()!=answer.size())
            {
                QMessageBox::warning(this, tr("XML"),tr("Wrong XML Format."));
                return;
            }

            showExpression();
        }

        else {
            QMessageBox::warning(this, tr("Path"),tr("You did not select any file."));
            return;
        }
}

void MainWindow::showExpression()
{
    wDisp=new QWidget;
    this->setCentralWidget(wDisp);
    vlayout=new QVBoxLayout;
    vlayout->addWidget(labelHelp);

    for(int i=0;i<expression.size();i++)
    {
        hlayout[i]=new QHBoxLayout;
        labelExp[i]=new QLineEdit(expression[i]);
        labelExp[i]->setReadOnly(true);
        labelAns[i]=new QLineEdit;
        labelAns[i]->setReadOnly(true);
        labelAns[i]->setPlaceholderText("Click to show the answer");
        labelConsq[i]=new QLabel;
        edit[i]=new QLineEdit;
        edit[i]->setPlaceholderText("Input your answer");

        hlayout[i]->addWidget(labelExp[i]);
        hlayout[i]->addWidget(edit[i]);
        hlayout[i]->addWidget(labelAns[i]);
        hlayout[i]->addWidget(labelConsq[i]);
        vlayout->addItem(hlayout[i]);
    }

    buttonAnswer=new QPushButton(tr("Show Answers"));
    labelGrade=new QLabel(" Grade: ");
    QHBoxLayout *consq=new QHBoxLayout;
    consq->addWidget(buttonAnswer);
    consq->addWidget(labelGrade);
    vlayout->addItem(consq);

    wDisp->setLayout(vlayout);

    connect(buttonAnswer,&QPushButton::clicked,this,&MainWindow::showAnswer);

    myTimer=new QTimer();
    myTimer->setInterval(20000);
    connect(myTimer,&QTimer::timeout,this,&MainWindow::forbidWrite);
    myTimer->start();
}

void MainWindow::showAnswer()
{
    int count=0;
    int i;
    int grade;

    buttonAnswer->setEnabled(false);

    myTimer->stop();
    nextForbid=0;

    for(i=0;i<answer.size();i++)
        edit[i]->setReadOnly(true);

    for(i=0;i<answer.size();i++)
    {
        labelAns[i]->setText(answer[i]);
        if(QString::compare(edit[i]->text(),labelAns[i]->text())==0)
        {
            labelConsq[i]->setText("Correct");
            count++;
        }
        else
            labelConsq[i]->setText("Wrong");
    }

    grade=(double)count/answer.size()*100;
    labelGrade->setText(QStringLiteral(" Grade: ")+QString("%1").arg(grade));

    updateHistory(count);
}

void MainWindow::updateHistory(int count)
{
    QString path="history.txt";
    QFile file(path);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("File"),tr("Cannot Update History."));
        return;
    }

    QTextStream os(&file);
    os.seek(file.size());
    os<< count << " / " << answer.size() << " Grade: " << int((double)count/answer.size()*100) << "\n";
    file.close();
}

void MainWindow::history()
{
    QString path="history.txt";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("File"), tr("No History"));
        return;
        }
    QTextStream is(&file);
    editHistory=new QTextEdit;
    editHistory->setText(is.readAll());
    file.close();
    editHistory->show();
}

void MainWindow::forbidWrite()
{
    if(nextForbid<expression.size()-1)
    {
        edit[nextForbid]->setReadOnly(true);
        nextForbid++;
    }
    else
        showAnswer();
}

void MainWindow::on_pushButton_clicked()
{
    settingWindow();
}

void MainWindow::on_pushButton_2_clicked()
{
    readXML();
}

void MainWindow::on_pushButton_3_clicked()
{
    history();
}

void MainWindow::on_pushButton_4_clicked()
{
    this->close();
}
