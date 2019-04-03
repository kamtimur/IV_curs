#pragma once

#include <QtMqtt/QMqttClient>

class MqttClient : public QMqttClient
{
    Q_OBJECT
public:
    MqttClient();
    void Connect();
    void Publish(QString topic, const QByteArray & message);
    void Subscribe(QString topic);

signals:
    void ConnectionEstablished();
    void Disconnected();
    void MessageRecieved(const QByteArray &message, const QMqttTopicName &topic);
public slots:
    void StateChanged();
protected:

private:
    QMqttClient * m_client_ = nullptr;
    QString hostname_ = "127.0.0.1";
    uint16_t port_ = 1883;
};

