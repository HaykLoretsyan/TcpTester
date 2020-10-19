#include "Controller.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

Controller::Controller()
{
    connect(&m_socket, &QTcpSocket::stateChanged, this, [this](QAbstractSocket::SocketState state) {

        if (state == QAbstractSocket::ConnectedState)
            emit this->connectionChanged(true);
        else if(state == QAbstractSocket::UnconnectedState)
            emit this->connectionChanged(false);
    });

    connect(&m_socket, &QTcpSocket::readyRead, this, [this]() {

        auto data = m_socket.readAll();
        emit dataReceived(QString::fromUtf8(data));
    });
}

void Controller::connectToServer(QString ip, int port)
{
    m_socket.connectToHost(ip, port);
}

void Controller::disconnectFromServer()
{
    m_socket.disconnectFromHost();
}

bool Controller::isConnected()
{
    return m_socket.isValid();
}

void Controller::activateAttack(bool activate)
{
    if (!isConnected())
        return;

    QJsonDocument doc;
    QJsonObject object;
    object.insert("command_type", "activate_attack");
    object.insert("activate", QJsonValue(activate));

    doc.setObject(object);

    m_socket.write(doc.toJson());

    emit dataSent(QString::fromUtf8(doc.toJson()));
}

void Controller::sendPoint(int x, int y)
{
    if (!isConnected())
        return;

    QJsonDocument doc;
    QJsonObject object;
    object.insert("command_type", "change_point");
    object.insert("x", QJsonValue(x));
    object.insert("y", QJsonValue(y));

    doc.setObject(object);

    m_socket.write(doc.toJson());

    emit dataSent(QString::fromUtf8(doc.toJson()));
}

void Controller::changeCoef(int coefficient)
{
    if (!isConnected())
        return;

    QJsonDocument doc;
    QJsonObject object;
    object.insert("command_type", "change_coef");
    object.insert("coefficient", QJsonValue(coefficient));

    doc.setObject(object);

    m_socket.write(doc.toJson());

    emit dataSent(QString::fromUtf8(doc.toJson()));
}

void Controller::changeCenter(int center)
{
    if (!isConnected())
        return;

    QJsonDocument doc;
    QJsonObject object;
    object.insert("command_type", "change_center");
    object.insert("center", QJsonValue(center));

    doc.setObject(object);

    m_socket.write(doc.toJson());

    emit dataSent(QString::fromUtf8(doc.toJson()));
}
