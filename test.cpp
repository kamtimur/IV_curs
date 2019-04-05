#include "test.h"

#include "mqtt_client_wrapper.h"
#include "generator.h"



Test::Test()
{
    QString vis_topic = "inc/vis";

    MqttClient * sender = new MqttClient();
    MqttClient * reciever = new MqttClient();
    Generator * gen = new Generator(1);
    QObject::connect(reciever, &MqttClient::ConnectionEstablished, this, [=]()
    {
        reciever->Subscribe(vis_topic);
    });

    QObject::connect(gen, &Generator::SignalGenerated, this, [=]()
    {
        uint8_t buf[gen->signal_buf_size_];
        gen->GetSignalBuf(buf);
        QByteArray arr = QByteArray::fromRawData( reinterpret_cast<const char*>(buf), gen->signal_buf_size_ );
        sender->Publish(vis_topic, arr);
    });

    sender->Connect();
    reciever->Connect();



}

