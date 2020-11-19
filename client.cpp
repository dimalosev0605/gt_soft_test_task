#include "client.h"

const QString Client::host_name = "jsonplaceholder.typicode.com";
const QString Client::request = "GET /users HTTP/1.0\n"
                                "HOST: jsonplaceholder.typicode.com\r\n\r\n";

Client::Client(QObject *parent)
    : QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::stateChanged, this, &Client::state_changed);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error_occurred);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::ready_read);
    connect(this, &Client::host_lookup_finished, this, &Client::connect_to_host);
}

void Client::connected()
{
    send_request();
}

void Client::disconnected()
{
    received_data.clear();
    received_data.shrink_to_fit();
}

void Client::state_changed(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
    case QAbstractSocket::UnconnectedState: {
        emit state("Not conneted");
        break;
    }
    case QAbstractSocket::HostLookupState: {
        emit state("Host lookup");
        break;
    }
    case QAbstractSocket::ConnectingState: {
        emit state("Connecting");
        break;
    }
    case QAbstractSocket::ConnectedState: {
        emit state("Connected");
        break;
    }
    case QAbstractSocket::ClosingState: {
        emit state("Closing");
        break;
    }
    }
}

void Client::error_occurred(QAbstractSocket::SocketError socketError)
{
    emit state(socket.errorString());
}

void Client::ready_read()
{
    const auto bytes_available = socket.bytesAvailable();
    received_data += socket.read(bytes_available);
    process_data();
}

void Client::lookup_host()
{
    QHostInfo::lookupHost(host_name, this, &Client::lookedup_host);
}

void Client::lookedup_host(const QHostInfo& host)
{
    if(host.error() != QHostInfo::NoError) {
        emit state(host.errorString());
        return;
    }
    else {
        host_ip_addresses.clear();
        const auto addresses = host.addresses();
        for(const auto& address : addresses) {
            host_ip_addresses.push_back(address.toString());
        }
        emit host_lookup_finished();
    }
}

void Client::connect_to_host()
{
    if(!host_ip_addresses.isEmpty()) {
        const auto ip_address = host_ip_addresses.first();
        socket.connectToHost(ip_address, host_port);
    }
    else {
        lookup_host();
    }
}

void Client::send_request()
{
    socket.write(request.toStdString().c_str());
}

void Client::process_data()
{
    static int is_http_header_read = false;

    if(!is_http_header_read) {
        const int index = received_data.indexOf("\r\n\r\n");
        if(index != -1) {
            received_data = received_data.remove(0, index + 4);
            is_http_header_read = true;
        }
    }
    else {
        const auto j_doc = QJsonDocument::fromJson(QByteArray(received_data.toStdString().c_str()));
        if(!j_doc.isNull()) {
            emit json_ready(j_doc);
            received_data.clear();
            is_http_header_read = false;
        }
    }
}
