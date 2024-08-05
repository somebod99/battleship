#include "game.h"
#include <QThread>


Game::Game(int p1_sock_descriotor, int p2_sock_descriotor, int num, QObject *parent)
    : QObject{parent},
      num_game{num}
{
    qDebug() << "new game ctor at" << QThread::currentThreadId() << '\n';
    _player1 = new QTcpSocket;
    _player1->setSocketDescriptor(p1_sock_descriotor);

    _player2 = new QTcpSocket;
    _player2->setSocketDescriptor(p2_sock_descriotor);

    qDebug() << "sending message '" << 1 << "' to " << _player1 << '\n';
    QTextStream p1(_player1);
    p1 << 1;

    qDebug() << "sending message '" << 0 << "' to " << _player2 << '\n';
    QTextStream p2(_player2);
    p2 << 0;

    QObject::connect(_player1, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    QObject::connect(_player2, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

Game::~Game()
{
    delete _player1;
    delete _player2;
}

void Game::onReadyRead()
{
    qDebug() << "new game read at" << QThread::currentThreadId() << '\n';
    QTcpSocket* p_in = Q_NULLPTR;
    QTcpSocket* p_out = Q_NULLPTR;

    if(sender() == _player1)
    {
        p_in = _player1;
        p_out = _player2;
    }
    else
    {
        p_in = _player2;
        p_out = _player1;
    }

    QString in_mes = p_in->readAll();
    qDebug() << "get message " << in_mes << " from " << p_in <<  '\n';

    if(in_mes == "close")
    {
        p_in->close();

        p_out->write("close");
        p_out->flush();
        p_out->close();

        emit close(num_game);

        return;
    }

    if(in_mes != "turn")
    {
        qDebug() << "sending message " << in_mes << " to " << p_out << '\n';
        QTextStream out(p_out);
        out << in_mes;
    }
    else
    {
        qDebug() << "sending message '0'" << " to " << p_in << '\n';
        p_in->write("0");

        qDebug() << "sending message '1'" << " to " << p_out << '\n';
        p_out->write("1");
    }
}
