#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QHostInfo>
#include <QHostAddress>

class Client : public QObject
{
    static const QString host_name;
    static const int host_port = 80;
    static const QString request;

    Q_OBJECT
    QTcpSocket socket;
    QVector<QString> host_ip_addresses;
    QString received_data;

private slots:
    void connected();
    void disconnected();
    void state_changed(QAbstractSocket::SocketState socketState);
    void error_occurred(QAbstractSocket::SocketError socketError);
    void ready_read();

    void lookup_host();
    void lookedup_host(const QHostInfo& host);
    void connect_to_host();

    void send_request();
    void process_data();

public:
    explicit Client(QObject *parent = nullptr);

signals:
    void host_lookup_finished();
    void json_ready(const QJsonDocument& j_doc);
    void state(const QString& state);
};

#endif // CLIENT_H
