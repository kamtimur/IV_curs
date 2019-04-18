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
    MqttClientWrapper* sender_;
    MqttClientWrapper* reciever_;
    Generator* generator_;
    DeviceWidget* dev_wid_sender_;
    DeviceWidget* dev_wid_reciever_;
};

