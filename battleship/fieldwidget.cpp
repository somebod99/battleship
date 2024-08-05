#include "fieldwidget.h"
#include "ui_fieldwidget.h"
#include <QMessageBox>


FieldWidget::FieldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->readyPushButton, SIGNAL(clicked()), this, SLOT(onReadyButton()));
    QObject::connect(this, SIGNAL(checkField()), ui->widget, SLOT(checkField()));
    QObject::connect(ui->widget, SIGNAL(ready(bool)), this, SLOT(onReady(bool)));
    QObject::connect(ui->widget, SIGNAL(press(int,int)), this, SLOT(drawCell(int,int)));
    QObject::connect(ui->widget, SIGNAL(lose()), this, SLOT(onLose()));
}

FieldWidget::~FieldWidget()
{
    delete ui;
}

int FieldWidget::getCell(int i, int j)
{
    return ui->widget->getCell(i, j);
}

void FieldWidget::setCell(int i, int j, int val)
{
    ui->widget->setCell(i, j, val);
}

void FieldWidget::checkLose()
{
    ui->widget->checkLose();
}

void FieldWidget::onReadyButton()
{
    emit checkField();
}

void FieldWidget::drawCell(int i, int j)
{
    ui->widget->setCell(i, j, !ui->widget->getCell(i, j));
}

void FieldWidget::onReady(bool val)
{
    if(val)
    {
        ui->widget->setEnabled(false);
        ui->readyPushButton->hide();

        emit ready();
    }
    else
    {
        QMessageBox::critical(this, "Упс", "Необходимо расположить все корабли\n"
                                           "4 - однопалубных\n3 - двухпалубных\n2 - трехпалубных\n1 - четырехпалубный");
    }
}

void FieldWidget::onLose()
{
    emit loseGame();
}
