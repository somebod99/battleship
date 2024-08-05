#ifndef GAME_H
#define GAME_H

#include <QTcpSocket>

class Game : public QObject
{
    Q_OBJECT

public:
    Game(int p1_sock_descriotor, int p2_sock_descriotor, int num, QObject *parent = nullptr);
    ~Game();

signals:
    void close(int num_game);

public slots:
    void onReadyRead();

private:
    QTcpSocket* _player1;
    QTcpSocket* _player2;

    int num_game;
};

#endif // GAME_H
