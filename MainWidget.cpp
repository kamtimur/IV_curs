#include "MainWidget.h"
#include <QLayout>


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QString vis_topic = "inc/vis";

    sender_ = new MqttClientWrapper();
    reciever_ = new MqttClientWrapper();
    generator_ = new Generator(1);

//    QObject::connect(reciever, &MqttClientWrapper::ConnectionEstablished, this, [=]()
//    {
//        reciever->Subscribe(vis_topic);
//    });

//    QObject::connect(gen, &Generator::SignalGenerated, this, [=, &count]()
//    {
//        int32_t buf[Generator::channel_num_ + 2];
//        gen->GetSignalBuf(reinterpret_cast<uint8_t*>(buf));
//        buf[Generator::channel_num_] = count;
//        QByteArray message = QByteArray::fromRawData( reinterpret_cast<const char*>(buf), Generator::signal_buf_size_ + 4 );
//        sender->Publish(vis_topic, message);
//        count++;
//    });

//    sender->Connect();
//    reciever->Connect();

    dev_wid_sender_ = new DeviceWidget("sender");
    dev_wid_reciever_ = new DeviceWidget("reciever");

    connect(dev_wid_sender_, &DeviceWidget::signalConnect, sender_, &MqttClientWrapper::slotConnect);
    connect(dev_wid_reciever_, &DeviceWidget::signalConnect, reciever_, &MqttClientWrapper::slotConnect);



    connect(dev_wid_sender_, &DeviceWidget::signalStartPublish, this, [=]()
    {

    });
    connect(dev_wid_reciever_, &DeviceWidget::signalStartPublish, reciever_, &MqttClientWrapper::slotConnect);


    QVBoxLayout* vLayout = new QVBoxLayout;

    vLayout->addWidget(dev_wid_sender_);
    vLayout->addWidget(dev_wid_reciever_);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addLayout(vLayout);

    setLayout(mainLayout);
}

void MainWidget::GetSignal()
{

}
