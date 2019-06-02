#include "MqttClientWrapper.h"



MqttClientWrapper::MqttClientWrapper(QString cert_path):
    cert_path_(cert_path)
{
    m_client_ = new QMqttClient(this);
    m_device_ = new WebSocketIODevice(cert_path_,this);
    connect(m_client_, &QMqttClient::messageReceived, this, &MqttClientWrapper::slotMessageRecieved);
    connect(m_client_, &QMqttClient::pingResponseReceived, this, &MqttClientWrapper::slotPingResieved);
    connect(m_client_, &QMqttClient::stateChanged, this, &MqttClientWrapper::slotStateChanged);
    connect(m_client_, &QMqttClient::disconnected, this, &MqttClientWrapper::slotDisconnected);

    startTimer(1000);
}

void MqttClientWrapper::slotConnect(ProtocolType protocol_type, bool clean_session)
{
    if (m_client_->state() == QMqttClient::Disconnected)
    {
        m_client_->setCleanSession(clean_session);
        if (protocol_type == ProtocolType::MQTT)
        {
            m_client_->setHostname(hostname_);
            m_client_->setPort(port_.toInt());
            m_client_->connectToHost();
        }
        if (protocol_type == ProtocolType::WEBSOCKETS)
        {
            m_device_->setUrl(url_);
            m_device_->setProtocol("mqtt");
            connect(m_device_, &WebSocketIODevice::socketConnected, this, [=]()
            {
                qDebug() << "WebSocket connected, initializing MQTT connection.";
                m_client_->setTransport(m_device_, QMqttClient::TransportType::IODevice);
//                m_client_->connectToHost();
                m_client_->connectToHostEncrypted("localhost");
            });
            if (!m_device_->open(QIODevice::ReadWrite))
            {
                qDebug() << "Could not open socket device";
            }
            kdr_count_ = 0;
        }
    }
}

void MqttClientWrapper::slotDisconnect(ProtocolType protocol_type)
{
    if (m_client_->state() == QMqttClient::Connected)
    {
        if (protocol_type == ProtocolType::MQTT)
        {
            m_client_->disconnectFromHost();
        }
        if (protocol_type == ProtocolType::WEBSOCKETS)
        {
            m_device_->close();
        }
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
    else
    {
        qDebug() << "Subscribed to topic " << topic << " success";
        return;
    }
}

void MqttClientWrapper::slotMessageRecieved(const QByteArray &message, const QMqttTopicName &topic)
{
    speed_++;
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

void MqttClientWrapper::timerEvent(QTimerEvent *event)
{
    emit signalSpeed("Speed " + QString::number(speed_));
    speed_ = 0;
}

void MqttClientWrapper::slotStateChanged()
{
    emit signalStatus(m_client_->state());
    if (m_client_->state() == QMqttClient::Connected)
    {
        emit MqttClientWrapper::signalConnectionEstablished();
    }
}



