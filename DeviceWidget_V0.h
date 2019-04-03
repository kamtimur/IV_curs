#ifndef DEVICEWIDGET_V0_H
#define DEVICEWIDGET_V0_H

#include <QWidget>

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "QPainterWidget.h"


class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceWidget(QWidget *parent = nullptr);
    QPainterWidget* qPainterWidget;

protected:

    QPushButton*    btnConnect;
    QPushButton*    btnClear;

    bool    flgState;
};

#endif // DEVICEWIDGET_V0_H
