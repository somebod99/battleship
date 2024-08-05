#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QTcpServer *parent = nullptr);

signals:

public slots:
    void onCloseGame(int num_game);
    void onCloseConnection();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    int count_cons = 0;
    int count_games = 0;

    QMap<int, QThread*> games;
    int two_clients[2];
};

#endif // SERVER_H
