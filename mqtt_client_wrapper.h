#pragma once

#include <QtMqtt/QMqttClient>
#include "device_widget.h"
#include <QTime>
#include <memory>
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
    void Speed(const QString &);
    void Error(const QString &);
public slots:
    void StateChanged();



protected:

private:
    QMqttClient * m_client_ = nullptr;
//    QString hostname_ = "192.168.4.184";
    QString hostname_ = "localhost";
    uint16_t port_ = 1883;
    int32_t speed_ = 0;
    int32_t total_rec_bits_ = 0;
    QTime time_;
    int32_t kdr_count_ = 0;
    int32_t error_count_ = 0;
};

