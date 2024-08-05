#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    ui->enemyFieldWidget->setEnabled(false);

    sock = Q_NULLPTR;

    ui->enemyFieldWidget->hide();
    ui->label->hide();

    QObject::connect(ui->fieldWidget, SIGNAL(ready()), this, SLOT(ready()));
    QObject::connect(ui->enemyFieldWidget, SIGNAL(press(int,int)), this, SLOT(choice(int,int)));
}

MainWidget::~MainWidget()
{
    delete ui;

    if(sock)
    {
        if(sock->isOpen())
        {
            sock->write("close");
            sock->flush();
            sock->close();
        }

        delete sock;
    }
}

void MainWidget::ready()
{
    QObject::connect(ui->fieldWidget, SIGNAL(loseGame()), this, SLOT(lose()));

    ui->enemyFieldWidget->show();
    ui->label->show();

    sock = new QTcpSocket(this);
    sock->connectToHost("127.0.0.1", 9999, QIODevice::ReadWrite);

    QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void MainWidget::choice(int i, int j)
{
    QTextStream out(sock);
    out << i << ',' << j;
}

void MainWidget::onReadyRead()
{
    QString mes = sock->readAll();

    if(mes == "1")
    {
        ui->enemyFieldWidget->setEnabled(true);
        ui->label->setText("Ваш ход");
    }
    else if(mes == "0")
    {
        ui->enemyFieldWidget->setEnabled(false);
        ui->label->setText("Ход противника");
    }
    else if(mes == "close")
    {
        ui->enemyFieldWidget->setEnabled(false);
        sock->close();
        ui->label->setText("Победа");
        QMessageBox::information(this, "Ахо", "Противник вышел из игры\nВы выиграли!!!");
    }
    else
    {
        auto in = mes.split(',');

        int i = in[0].toInt();
        int j = in[1].toInt();

        if(in.size() >= 3)
        {
            int val = in[2].toInt();

            switch(val)
            {
            case 0:
                ui->enemyFieldWidget->setCell(i, j, 3);
                break;

            case 1:
                ui->enemyFieldWidget->setCell(i, j, 2);
                break;
            }

            if(in.size() == 4)
            {
                ui->enemyFieldWidget->setEnabled(false);
                sock->close();
                ui->label->setText("Победа");
                QMessageBox::information(this, "Ахо", "Вы выиграли!!!");

                return;
            }

            if(val != 1)
            {
                QTextStream resp(sock);
                resp << "turn";
            }
        }
        else
        {
            QString out;
            out = QString::number(i) + ',' + QString::number(j) + ',' + QString::number(ui->fieldWidget->getCell(i, j));
            sock->write(out.toStdString().c_str());

            if(ui->fieldWidget->getCell(i, j) == 0)
            {
                ui->fieldWidget->setCell(i, j, 3);
            }
            else
            {
                ui->fieldWidget->setCell(i, j, 2);
                ui->fieldWidget->checkLose();
            }
        }
    }
}

void MainWidget::lose()
{
    QObject::disconnect(ui->fieldWidget, SIGNAL(loseGame()), this, SLOT(lose()));

    if(sock)
    {
        sock->write(",win");
        sock->flush();
        sock->close();
    }

    ui->enemyFieldWidget->setEnabled(false);
    ui->label->setText("Поражение");
    QMessageBox::information(this, "Ахо", "Вы проиграли....");
}
