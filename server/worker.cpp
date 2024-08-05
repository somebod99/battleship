#include "worker.h"

Worker::Worker(int p1_sock_descriotor, int p2_sock_descriotor, int num, QObject* parent)
    : QThread{parent},
      _p1_sock_descriotor{p1_sock_descriotor},
      _p2_sock_descriotor{p2_sock_descriotor},
      _num{num}
{
}

void Worker::run()
{
    game = new Game(_p1_sock_descriotor, _p2_sock_descriotor, _num);

    qDebug() << "new game start at" << QThread::currentThreadId() << '\n';

    QObject::connect(game, SIGNAL(close(int)), this, SLOT(onCloseGame(int)));

    exec();
}

void Worker::onCloseGame(int num)
{
    emit closeGame(num);
}
