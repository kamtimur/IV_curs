#pragma once

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
    explicit DeviceWidget(QString label, QWidget *parent = nullptr);
signals:
    void signalConnect();
    void signalStartPublish();
    void signalSubscribe();
public slots:
    void SetErrCount(const QString & err);
    void SetSpeed(const QString & speed);
    void PaintData(int32_t* data);
protected:

    QPainterWidget * qPainterWidget;
    QLabel * speed_;
    QLabel * err_count_;
    QPushButton*    btn_connect_;
    QPushButton*    btn_clear_;

    bool    flg_state;
};

