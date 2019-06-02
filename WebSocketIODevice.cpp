#include "websocketiodevice.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
WebSocketIODevice::WebSocketIODevice(QString path, QObject *parent)
    : QIODevice(parent)
{
    connect(&m_socket, &QWebSocket::connected, this, &WebSocketIODevice::onSocketConnected);
    connect(&m_socket, &QWebSocket::binaryMessageReceived, this, &WebSocketIODevice::handleBinaryMessage);
    QFile certFile(path+"/client.crt");
    QFile keyFile(path+"/client.key");
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    ssl_conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    ssl_conf.setLocalCertificate(certificate);
    ssl_conf.setPrivateKey(sslKey);
    ssl_conf.setProtocol(QSsl::TlsV1SslV3);
}

bool WebSocketIODevice::open(QIODevice::OpenMode mode)
{
    // Cannot use an URL because of missing sub protocol support
    // Have to use QNetworkRequest, see QTBUG-38742
    QNetworkRequest request;


    request.setSslConfiguration(ssl_conf);
    request.setUrl(m_url);
    request.setRawHeader("Sec-WebSocket-Protocol", m_protocol.constData());

    m_socket.open(request);

    return QIODevice::open(mode);
}

void WebSocketIODevice::close()
{
    m_socket.close();
    QIODevice::close();
}

qint64 WebSocketIODevice::readData(char *data, qint64 maxlen)
{
    qint64 bytesToRead = qMin(maxlen, (qint64)m_buffer.size());
    memcpy(data, m_buffer.constData(), bytesToRead);
    m_buffer = m_buffer.right(m_buffer.size() - bytesToRead);
    return bytesToRead;
}

qint64 WebSocketIODevice::writeData(const char *data, qint64 len)
{
    QByteArray msg(data, len);
    const int length = m_socket.sendBinaryMessage(msg);
    return length;
}

void WebSocketIODevice::setUrl(const QUrl &url)
{
    m_url = url;
}

void WebSocketIODevice::setProtocol(const QByteArray &data)
{
    m_protocol = data;
}

void WebSocketIODevice::handleBinaryMessage(const QByteArray &msg)
{
    m_buffer.append(msg);
    emit readyRead();
}

void WebSocketIODevice::onSocketConnected()
{
    emit socketConnected();
}
