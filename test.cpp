#include "test.h"

#include "mqtt_client_wrapper.h"
#include "generator.h"



Test::Test()
{
    QString vis_topic = "inc/vis";

    MqttClient * sender = new MqttClient();
    MqttClient * reciever = new MqttClient();
    Generator * gen = new Generator(1);
    int32_t count = 0;

    QObject::connect(reciever, &MqttClient::ConnectionEstablished, this, [=]()
    {
        reciever->Subscribe(vis_topic);
    });

    QObject::connect(gen, &Generator::SignalGenerated, this, [=, &count]()
    {
        int32_t buf[Generator::channel_num_ + 2];
        gen->GetSignalBuf(reinterpret_cast<uint8_t*>(buf));
        buf[Generator::channel_num_] = count;
        QByteArray message = QByteArray::fromRawData( reinterpret_cast<const char*>(buf), Generator::signal_buf_size_ + 4 );
        if(count % 2 != 0)
        sender->Publish(vis_topic, message);
        count++;
    });

    sender->Connect();
    reciever->Connect();



}

