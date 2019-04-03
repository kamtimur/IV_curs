#include "widget.h"
#include <QApplication>
#include "test.h"
#include "DeviceWidget_V0.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test test;



    DeviceWidget dev_wid;
    dev_wid.show();
    QObject::connect(&test, &Test::VisRecieved, dev_wid.qPainterWidget , &QPainterWidget::interrupt);
    return a.exec();
}
