#include "Client.h"

Client::Client(QObject *parent) : QObject{parent} {}

void Client::Init(const QUrl &url) {

    QObject::connect(&socket_, &QWebSocket::connected, this, &Client::onConnected);
    QObject::connect(&socket_, &QWebSocket::disconnected, this, &Client::onDisconnected);
    QObject::connect(&socket_, &QWebSocket::stateChanged, this, &Client::onStateChanged);
    QObject::connect(&socket_, &QWebSocket::sslErrors, this, &Client::onSslError);
    QObject::connect(&socket_, &QWebSocket::textMessageReceived, this, &Client::onMessageReceived);

    socket_.open(url);
}

void Client::onConnected() {
    qDebug() << "on connected";
    socket_.sendTextMessage("hello world你好");
    socket_.sendTextMessage("hello world你好");
}

void Client::onDisconnected() { qDebug() << "on disconnected"; }

void Client::onStateChanged(QAbstractSocket::SocketState state) { qDebug() << "on state change " << state; }

void Client::onSslError(const QList<QSslError> &errors) {
    for(auto &it : errors) {
        qDebug() << it;
    }
    socket_.ignoreSslErrors();
}

void Client::onMessageReceived(const QString &message) { qDebug() << "recv : " << message; }
