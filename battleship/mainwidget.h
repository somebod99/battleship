#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void ready();
    void choice(int i, int j);
    void onReadyRead();
    void lose();

private:
    Ui::MainWidget *ui;

    QTcpSocket* sock;

};
#endif // MAINWIDGET_H
