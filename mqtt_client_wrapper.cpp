#include "mqtt_client_wrapper.h"



MqttClient::MqttClient()
{
    QString vis_topic = "inc/vis";
    m_client_ = new QMqttClient(this);
    DeviceWidget * dev_wid_ = new DeviceWidget();
    connect(m_client_, &QMqttClient::messageReceived, this, [=](const QByteArray &message, const QMqttTopicName &topic)
    {
        const QString content = QLatin1String(" Received Topic: ")
                    + topic.name()
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        qDebug() << content;
        emit MessageRecieved(message, topic);
        if (topic.name() == vis_topic)
        {
            int32_t len = message.length();
            int32_t buf[64];
            memset(buf, 0, 64);

            for (int32_t i =0;i < 8; i++)
            {
                buf[i] = (message[3*i]) | ( message[3*i+1] << 8) | ( message[3*i+1] << 16);
            }
            emit VisRecieved(reinterpret_cast<int32_t *>(buf));
        }
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


    dev_wid_->show();
    QObject::connect(this, &MqttClient::VisRecieved, dev_wid_->qPainterWidget , &QPainterWidget::interrupt);


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


