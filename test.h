#pragma once
#include <QObject>


class Test : public QObject
{
    Q_OBJECT
public:
    Test();
signals:
    void VisRecieved(int32_t * data);
};

