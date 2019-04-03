#include "DeviceWidget_V0.h"

#include <QDebug>
#include <QSplitter>
#include "generator.h"

DeviceWidget::DeviceWidget(QWidget *parent) : QWidget(parent)
{
    flgState = false;

    qPainterWidget = new QPainterWidget();

    QHBoxLayout* hlayout1 = new QHBoxLayout;
    hlayout1->addWidget( new QLabel("<H1>mqtt</H1>") );

    QHBoxLayout* hlayout2 = new QHBoxLayout;

    QHBoxLayout* hlayout3 = new QHBoxLayout;
    hlayout3->addWidget(qPainterWidget);

    QVBoxLayout* vlayoutW = new QVBoxLayout();
    vlayoutW->addItem(hlayout1);
    vlayoutW->addItem(hlayout2);
    vlayoutW->addItem(hlayout3);
    setLayout(vlayoutW);
}



