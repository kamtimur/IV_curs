#include "DeviceWidget.h"

#include <QDebug>
#include <QSplitter>
#include "Generator.h"

DeviceWidget::DeviceWidget(QString label, QWidget *parent) : QWidget(parent)
{
    flg_state = false;

    qPainterWidget = new QPainterWidget();

    QHBoxLayout* hlayout1 = new QHBoxLayout();
    hlayout1->addWidget( new QLabel(label));

    QHBoxLayout* hlayout2 = new QHBoxLayout();
    speed_ = new QLabel();
    err_count_ = new QLabel();
    QHBoxLayout* button_layout = new QHBoxLayout();

    QPushButton* connect_button = new QPushButton("connect", this);
    QPushButton* start_pub_button = new QPushButton("start_publish", this);
    QPushButton* subscribe_button = new QPushButton("subscribe", this);

    connect(connect_button, &QPushButton::clicked, this, [=]()
    {
        emit signalConnect();
    });

    connect(start_pub_button, &QPushButton::clicked, this, [=]()
    {
        emit signalStartPublish();
    });

    connect(subscribe_button, &QPushButton::clicked, this, [=]()
    {
        emit signalSubscribe();
    });

    hlayout2->addWidget( speed_ );
    hlayout2->addWidget( err_count_ );
    button_layout->addWidget( connect_button );
    button_layout->addWidget( start_pub_button );
    button_layout->addWidget( subscribe_button );

    QHBoxLayout* hlayout3 = new QHBoxLayout();
    hlayout3->addWidget(qPainterWidget);


    QVBoxLayout* vlayoutW = new QVBoxLayout();
    vlayoutW->addItem(hlayout1);
    vlayoutW->addItem(hlayout2);
    vlayoutW->addItem(button_layout);
    vlayoutW->addItem(hlayout3);
    setLayout(vlayoutW);
}

void DeviceWidget::SetErrCount(const QString &err)
{
    err_count_->setText(err);
}

void DeviceWidget::SetSpeed(const QString &speed)
{
    speed_->setText(speed);
}

void DeviceWidget::PaintData(int32_t *data)
{
    qPainterWidget->interrupt(data);
}



