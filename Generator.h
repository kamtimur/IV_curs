#pragma once

#include <QObject>
#include <mutex>
#include <memory>

class Generator : public QObject
{
    Q_OBJECT
public:
    explicit Generator(int32_t period, QObject *parent = nullptr);
    void GetSignalBuf( uint8_t * outer_buf );

    static const int8_t channel_num_ = 8;
    static const std::size_t signal_buf_size_ = channel_num_ * 4 + 2 * 4;

signals:
    void SignalGenerated();
public slots:

protected:

private:
    void timerEvent(QTimerEvent *event);
    uint8_t signal_buf_[signal_buf_size_];
    std::mutex signal_buf_mutex_;
    int32_t counter_ = 0;

};


