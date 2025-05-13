#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void Init(const QUrl &url);

signals:

private:
    void onConnected();
    void onDisconnected();
    void onStateChanged(QAbstractSocket::SocketState state);

    void onSslError(const QList<QSslError> &errors);

    void onMessageReceived(const QString &message);

private:
    QWebSocket socket_;
};

#endif // CLIENT_H
