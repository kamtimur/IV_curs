#pragma once

#include <QtMqtt/QMqttClient>
#include "DeviceWidget_V0.h"
class MqttClient : public QMqttClient
{
    Q_OBJECT
public:
    MqttClient();
    void Connect();
    void Publish(QString topic, const QByteArray & message);
    void Subscribe(QString topic);

     DeviceWidget * dev_wid_;
signals:
    void ConnectionEstablished();
    void Disconnected();
    void MessageRecieved(const QByteArray &message, const QMqttTopicName &topic);
    void VisRecieved(int32_t * data);
public slots:
    void StateChanged();



protected:

private:
    QMqttClient * m_client_ = nullptr;
    QString hostname_ = "127.0.0.1";
    uint16_t port_ = 1883;
};

