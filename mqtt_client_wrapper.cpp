#include "mqtt_client_wrapper.h"



MqttClient::MqttClient()
{
    QString vis_topic = "inc/vis";
    m_client_ = new QMqttClient(this);
    DeviceWidget * dev_wid_ = new DeviceWidget();


    connect(this, &MqttClient::Speed, dev_wid_, &DeviceWidget::SetSpeed);
    connect(this, &MqttClient::Error, dev_wid_, &DeviceWidget::SetErrCount);

    connect(m_client_, &QMqttClient::messageReceived, this, [=](const QByteArray &message, const QMqttTopicName &topic)
    {
        int32_t len = message.length();
        total_rec_bits_ += len;
        int32_t sec = time_.elapsed()/1000;
        if(sec > 0)
        {
            speed_ = total_rec_bits_/(sec);
        }

        emit Speed("Speed "+QString::number(speed_));
        emit MessageRecieved(message, topic);
        if (topic.name() == vis_topic)
        {
            kdr_count_++;
            int32_t * data = reinterpret_cast<int32_t *>(const_cast<char *>(message.data()));
            int32_t counter = *(data + 8);
            if ( (kdr_count_ != counter) && (kdr_count_ != 1) )
            {
                error_count_ += counter - kdr_count_;
                emit Error("Error "+QString::number(error_count_));
            }
            kdr_count_ = counter;
            emit VisRecieved(data);
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
    connect(this, &MqttClient::VisRecieved, dev_wid_ , &DeviceWidget::PaintData);

    dev_wid_->show();
    m_client_->setHostname(hostname_);
    m_client_->setPort(port_);
}

void MqttClient::Connect()
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

void MqttClient::Publish(QString topic, const QByteArray & message)
{
        m_client_->publish(topic, message);
}

void MqttClient::Subscribe(QString topic)
{
    bool subscription = m_client_->subscribe(topic);
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


