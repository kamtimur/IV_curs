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


    QObject::connect(reciever, &MqttClient::MessageRecieved, this, [=](const QByteArray &message, const QMqttTopicName &topic)
    {
        int32_t buf[gen->signal_buf_size_];
        memset(buf, 0, gen->signal_buf_size_);

        for (int32_t i =0;i < 8; i++)
        {
            buf[i] = (message[3*i]) | ( message[3*i+1] << 8) | ( message[3*i+1] << 16);
        }
        if(topic.name() == vis_topic)
        {
            emit VisRecieved(reinterpret_cast<int32_t *>(buf));
        }
    });
    sender->Connect();
    reciever->Connect();



}

