#include "mqtt_client_wrapper.h"



MqttClient::MqttClient()
{
    m_client_ = new QMqttClient(this);
    connect(m_client_, &QMqttClient::messageReceived, this, [=](const QByteArray &message, const QMqttTopicName &topic)
    {
        const QString content = QLatin1String(" Received Topic: ")
                    + topic.name()
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        qDebug() << content;
        emit MessageRecieved(message, topic);
    });
    connect(m_client_, &QMqttClient::pingResponseReceived, this, [=]()
    {
        qDebug() << "PingResponse";
    });
    connect(m_client_, &QMqttClient::stateChanged, this, &MqttClient::StateChanged);
    connect(m_client_, &QMqttClient::disconnected, this, [this]
    {
        emit Disconnected();
    });

    m_client_->setHostname(hostname_);
    m_client_->setPort(port_);
}

void MqttClient::Connect()
{
    if (m_client_->state() == QMqttClient::Disconnected) {
        qDebug() << "Disconnect";
        m_client_->connectToHost();
    } else {
        qDebug() << "Connect";
        m_client_->disconnectFromHost();
    }
}

void MqttClient::Publish(QString topic, const QByteArray & message)
{
        m_client_->publish(topic, message);
}

void MqttClient::Subscribe(QString topic)
{
    auto subscription = m_client_->subscribe(topic);
    if (!subscription) {
        qDebug() << "Could not subscribe. Is there a valid connection?";
        return;
    }
}

void MqttClient::StateChanged()
{
    const QString content = QLatin1String(": State Change")
                    + QString::number(m_client_->state())
                    + QLatin1Char('\n');
    qDebug() << content;
    if (m_client_->state() == QMqttClient::Connected)
    {
        emit MqttClient::ConnectionEstablished();
    }
}


