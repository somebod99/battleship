#include "server.h"
#include "worker.h"
#include <QThread>

Server::Server(QTcpServer *parent)
    : QTcpServer{parent}
{
    qDebug() << "start server at" << QThread::currentThreadId() << '\n';
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    two_clients[count_cons++] = socketDescriptor;
    QTcpSocket* sock = new QTcpSocket;
    sock->setSocketDescriptor(socketDescriptor);
    QObject::connect(sock, SIGNAL(disconnected()), this, SLOT(onCloseConnection()));

    qDebug() << "new user connected: " << socketDescriptor << '\n';

    if(count_cons == 2)
    {
        Worker* worker = new Worker(two_clients[0], two_clients[1], count_games);
        QObject::connect(worker, SIGNAL(closeGame(int)), this, SLOT(onCloseGame(int)));

        games[count_games++] = worker;
        worker->start();

        count_cons = 0;
    }
}

void Server::onCloseGame(int num_game)
{
    games[num_game]->quit();
    games[num_game]->wait();

    delete games[num_game];
    games.remove(num_game);
}

void Server::onCloseConnection()
{
    QTcpSocket* sock = (QTcpSocket*)sender();
    QObject::disconnect(sock, SIGNAL(disconnected()), this, SLOT(onCloseConnection()));
    sock->close();
    sock->deleteLater();
    count_cons--;
}
