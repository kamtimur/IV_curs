#pragma once

#include <QtMqtt/QMqttClient>
#include <QTime>
#include <memory>

class MqttClientWrapper : public QObject
{
    Q_OBJECT
public:
    MqttClientWrapper();


signals:
    void signalConnectionEstablished();
    void signalDisconnected();
    void signalPing();
    void signalMessageRecieved(const QByteArray &message, const QMqttTopicName &topic);
    void signalVisRecieved(int32_t * data);
    void signalSpeed(const QString &);
    void signalError(const QString &);
public slots:
    void slotConnect();
    void slotPublish(const QMqttTopicName &topic,
                 const QByteArray &message = QByteArray(),
                 quint8 qos = 0,
                 bool retain = false);
    void slotSubscribe(const QMqttTopicFilter &topic, quint8 qos = 0);
private slots:
    void slotStateChanged();
    void slotMessageRecieved(const QByteArray &message, const QMqttTopicName &topic);
    void slotDisconnected();
    void slotPingResieved();



protected:

private:
    QString vis_topic_ = "inc/vis";
    QMqttClient * m_client_ = nullptr;
    QString hostname_ = "localhost";
    uint16_t port_ = 1883;
    int32_t speed_ = 0;
    int32_t total_rec_bits_ = 0;
    QTime time_;
    int32_t kdr_count_ = 0;
    int32_t error_count_ = 0;
};

