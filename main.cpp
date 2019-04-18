#include <QApplication>
#include "MainWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget gMainWidget;
    gMainWidget.show();


    return a.exec();
}
