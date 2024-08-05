#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include "config.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    PaintWidget(QWidget *parent = nullptr);

    int getCell(int i, int j);
    void setCell(int i, int j, int val);
    void drawCell(QPainter& p, int i, int j);
    void drawHit(QPainter& p, int i, int j);
    void drawMiss(QPainter& p, int i, int j);

    void checkLose();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

public slots:
    void checkField();

signals:
    void press(int i, int j);
    void lose();
    void ready(bool val);

private:
    int count = config::count;
    int size = config::size;
    int offset = config::offset;

    QVector<QVector<int>> field;
};

#endif // PAINTWIDGET_H
