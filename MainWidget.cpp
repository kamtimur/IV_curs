#include "MainWidget.h"

#include <QLayout>
#include <QInputDialog>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    sender_ = new MqttClientWrapper();
    reciever_ = new MqttClientWrapper();
    generator_ = new Generator(1);

    dev_wid_sender_ = new DeviceWidget("sender");
    dev_wid_reciever_ = new DeviceWidget("reciever");

    connect(dev_wid_sender_, &DeviceWidget::signalConnect, this, [=]()
    {
        if(sender_->State() == QMqttClient::ClientState::Disconnected)
        {
            sender_->slotConnect(MqttClientWrapper::ProtocolType::WEB_SOCKET, false);
        }

        if(sender_->State() == QMqttClient::ClientState::Connected)
        {
            sender_->slotDisconnect(MqttClientWrapper::ProtocolType::WEB_SOCKET);
        }
    });
    connect(dev_wid_reciever_, &DeviceWidget::signalConnect, this, [=]()
    {
        if(reciever_->State() == QMqttClient::ClientState::Disconnected)
        {
            reciever_->slotConnect(MqttClientWrapper::ProtocolType::WEB_SOCKET,  false);
        }

        if(reciever_->State() == QMqttClient::ClientState::Connected)
        {
            reciever_->slotDisconnect(MqttClientWrapper::ProtocolType::WEB_SOCKET);
        }
    });


    connect(dev_wid_reciever_, &DeviceWidget::signalSubscribe, this, [=]()
    {
//        bool ok;
//        QString write_text = QInputDialog::getText(this, "Input topic", "Data:", QLineEdit::Normal, "", &ok);

        reciever_->slotSubscribe(vis_topic_, 0);
    });


    connect(dev_wid_sender_, &DeviceWidget::signalStartPublish, this, [=]()
    {
        if (sender_publish_ == false)
        {
            qDebug() << "publishing";
            sender_publish_ = true;
        }
        else
        {
            qDebug() << "stop publishing";
            sender_publish_ = false;
        }
    });

    connect(generator_, &Generator::SignalGenerated, this, &MainWidget::GetSignal);
    connect(reciever_, &MqttClientWrapper::signalVisRecieved, dev_wid_reciever_, &DeviceWidget::PaintData);
    connect(reciever_, &MqttClientWrapper::signalError, dev_wid_reciever_, &DeviceWidget::SetErrCount);
    connect(reciever_, &MqttClientWrapper::signalSpeed, dev_wid_reciever_, &DeviceWidget::SetSpeed);
    connect(reciever_, &MqttClientWrapper::signalMessageRecieved, this, [=](const QByteArray &message, const QMqttTopicName &topic)
    {
        if(topic.name() == "$SYS")
        {
            qDebug() << "SYS topic" << topic;
            qDebug() << "message" << message;
        }
    });
    connect(reciever_, &MqttClientWrapper::signalStatus, dev_wid_reciever_, &DeviceWidget::SetStatus);
    connect(sender_, &MqttClientWrapper::signalStatus, dev_wid_sender_, &DeviceWidget::SetStatus);
    QVBoxLayout* vLayout = new QVBoxLayout;

    vLayout->addWidget(dev_wid_sender_);
    vLayout->addWidget(dev_wid_reciever_);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addLayout(vLayout);

    setLayout(mainLayout);
}

void MainWidget::GetSignal()
{
    if(sender_publish_ == true)
    {
        int32_t buf[Generator::channel_num_ + 2];
        generator_->GetSignalBuf(reinterpret_cast<uint8_t*>(buf));
        QByteArray message = QByteArray::fromRawData( reinterpret_cast<const char*>(buf), Generator::signal_buf_size_ + 4 );
        sender_->slotPublish(vis_topic_, message, 0, false);
    }
}
