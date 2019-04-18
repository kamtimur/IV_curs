#include "MqttClientWrapper.h"



MqttClientWrapper::MqttClientWrapper()
{
    m_client_ = new QMqttClient(this);

    connect(m_client_, &QMqttClient::messageReceived, this, &MqttClientWrapper::slotMessageRecieved);
    connect(m_client_, &QMqttClient::pingResponseReceived, this, &MqttClientWrapper::slotPingResieved);
    connect(m_client_, &QMqttClient::stateChanged, this, &MqttClientWrapper::slotStateChanged);
    connect(m_client_, &QMqttClient::disconnected, this, &MqttClientWrapper::slotDisconnected);

    m_client_->setHostname(hostname_);
    m_client_->setPort(port_);
}

void MqttClientWrapper::slotConnect()
{
    if (m_client_->state() == QMqttClient::Disconnected)
    {
        qDebug() << "Connect";
        m_client_->connectToHost();
        time_.start();
        kdr_count_ = 0;
    }
    else
    {
        qDebug() << "Disconnect";
        m_client_->disconnectFromHost();
        total_rec_bits_ = 0;
    }
}

void MqttClientWrapper::slotPublish(const QMqttTopicName &topic, const QByteArray &message, quint8 qos, bool retain)
{
        m_client_->publish(topic, message, qos, retain);
}

void MqttClientWrapper::slotSubscribe(const QMqttTopicFilter &topic, quint8 qos)
{
    bool subscription = m_client_->subscribe(topic, qos);
    if (!subscription)
    {
        qDebug() << "Could not subscribe. Is there a valid connection?";
        return;
    }
}

void MqttClientWrapper::slotMessageRecieved(const QByteArray &message, const QMqttTopicName &topic)
{
    int32_t len = message.length();
    total_rec_bits_ += len;
    int32_t sec = time_.elapsed()/1000;
    if(sec > 0)
    {
        speed_ = total_rec_bits_/(sec);
    }

    emit signalSpeed("Speed " + QString::number(speed_));
    emit signalMessageRecieved(message, topic);
    if (topic.name() == vis_topic_)
    {
        kdr_count_++;
        int32_t * data = reinterpret_cast<int32_t *>(const_cast<char *>(message.data()));
        int32_t counter = *(data + 8);
        if ( (kdr_count_ != counter) && (kdr_count_ != 1) )
        {
            error_count_ += counter - kdr_count_;
            emit signalError("Error "+QString::number(error_count_));
        }
        kdr_count_ = counter;
        emit signalVisRecieved(data);
    }
}

void MqttClientWrapper::slotDisconnected()
{
    emit signalDisconnected();
}

void MqttClientWrapper::slotPingResieved()
{
    emit signalPing();
}

void MqttClientWrapper::slotStateChanged()
{
    if (m_client_->state() == QMqttClient::Connected)
    {
        emit MqttClientWrapper::signalConnectionEstablished();
    }
}


