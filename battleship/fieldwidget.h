#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>

namespace Ui {
class FieldWidget;
}

class FieldWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FieldWidget(QWidget *parent = nullptr);
    ~FieldWidget();

    int getCell(int i, int j);
    void setCell(int i, int j, int val);

    void checkLose();

public slots:
    void onReadyButton();
    void drawCell(int i, int j);
    void onReady(bool val);
    void onLose();

signals:
    void checkField();
    void ready();
    void loseGame();

private:
    Ui::FieldWidget *ui;
};

#endif // FIELDWIDGET_H
