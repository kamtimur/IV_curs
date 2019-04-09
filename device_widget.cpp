#include "device_widget.h"

#include <QDebug>
#include <QSplitter>
#include "generator.h"

DeviceWidget::DeviceWidget(QWidget *parent) : QWidget(parent)
{
    flg_state = false;

    qPainterWidget = new QPainterWidget();

    QHBoxLayout* hlayout1 = new QHBoxLayout();
    hlayout1->addWidget( new QLabel("<H1>mqtt</H1>"));

    QHBoxLayout* hlayout2 = new QHBoxLayout();
    speed_ = new QLabel();
    err_count_ = new QLabel();


    hlayout2->addWidget( speed_ );
    hlayout2->addWidget( err_count_ );

    QHBoxLayout* hlayout3 = new QHBoxLayout();
    hlayout3->addWidget(qPainterWidget);


    QVBoxLayout* vlayoutW = new QVBoxLayout();
    vlayoutW->addItem(hlayout1);
    vlayoutW->addItem(hlayout2);
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



