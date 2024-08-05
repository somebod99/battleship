#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "game.h"

class Worker : public QThread
{
    Q_OBJECT

public:
    Worker(int p1_sock_descriotor, int p2_sock_descriotor, int num, QObject* parent = nullptr);

protected:
    void run();

public slots:
    void onCloseGame(int num);

signals:
    void closeGame(int num);

private:
    Game* game;

    int _p1_sock_descriotor;
    int _p2_sock_descriotor;
    int _num;
};

#endif // WORKER_H
