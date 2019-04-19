#include "MainWidget.h"
#include <QLayout>


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    sender_ = new MqttClientWrapper();
    reciever_ = new MqttClientWrapper();
    generator_ = new Generator(1);

    dev_wid_sender_ = new DeviceWidget("sender");
    dev_wid_reciever_ = new DeviceWidget("reciever");

    connect(dev_wid_sender_, &DeviceWidget::signalConnect, sender_, &MqttClientWrapper::slotConnect);
    connect(dev_wid_reciever_, &DeviceWidget::signalConnect, reciever_, &MqttClientWrapper::slotConnect);

    connect(dev_wid_reciever_, &DeviceWidget::signalSubscribe, this, [=]()
    {
        reciever_->slotSubscribe(vis_topic_, 1);
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
        int32_t count = 0;
        int32_t buf[Generator::channel_num_ + 2];
        generator_->GetSignalBuf(reinterpret_cast<uint8_t*>(buf));
        buf[Generator::channel_num_] = count;
        QByteArray message = QByteArray::fromRawData( reinterpret_cast<const char*>(buf), Generator::signal_buf_size_ + 4 );
        sender_->slotPublish(vis_topic_, message, 0, false);
        count++;
    }
}
