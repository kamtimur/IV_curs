#include "widget.h"
#include <QApplication>
#include "test.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test test;

    return a.exec();
}
