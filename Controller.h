#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTcpSocket>

class Controller : public QObject
{
    Q_OBJECT

public:

    Controller();

    void connectToServer(QString ip, int port);

    void disconnectFromServer();

    bool isConnected();

    void activateAttack(bool activate);

    void sendPoint(int x, int y);

    void changeCoef(int coef);

    void changeCenter(int center);

signals:

    void connectionChanged(bool connected);

    void dataSent(QString data);

    void dataReceived(QString data);

private:

    QTcpSocket m_socket;
};

#endif // CONTROLLER_H
