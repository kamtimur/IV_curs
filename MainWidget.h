#pragma once

#include <QWidget>

#include "MqttClientWrapper.h"
#include "Generator.h"
#include "DeviceWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
public slots:
    void GetSignal();

protected:

private:

    void timerEvent(QTimerEvent *event) override;


    MqttClientWrapper* sender_;
    MqttClientWrapper* reciever_;
    Generator* generator_;
    DeviceWidget* dev_wid_sender_;
    DeviceWidget* dev_wid_reciever_;
    bool is_sender_publish_ = false;
    volatile int32_t publish_speed_ = 0;
    QString vis_topic_ = "inc/vis";
};

