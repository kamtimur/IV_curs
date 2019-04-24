#include "Generator.h"

#include <cmath>
#include <cstring>
#include <QDebug>
Generator::Generator(int32_t period, QObject *parent) : QObject(parent)
{
    startTimer(period);
    memset(signal_buf_,0,signal_buf_size_);
}

void Generator::GetSignalBuf(uint8_t *outer_buf)
{
    signal_buf_mutex_.lock();
    memcpy(outer_buf, signal_buf_, signal_buf_size_);
    signal_buf_mutex_.unlock();
}




void Generator::timerEvent(QTimerEvent * )
{
    signal_buf_mutex_.lock();
    memset(signal_buf_,0,signal_buf_size_);
    for ( int32_t i = 0; i < channel_num_; i++ )
    {
      int32_t tmp = static_cast<int32_t>((sin(2.*3.14*counter_/500.+ 3.14/3.)+1)*10);
      signal_buf_[i*4]   = static_cast<uint8_t>(tmp);
      signal_buf_[i*4+1] = static_cast<uint8_t>(tmp >> 8);
      signal_buf_[i*4+2] = static_cast<uint8_t>(tmp >> 16);
      signal_buf_[i*4+3] = static_cast<uint8_t>(tmp >> 24);
    }
    signal_buf_[channel_num_*4]   = static_cast<uint8_t>(counter_);
    signal_buf_[channel_num_*4+1] = static_cast<uint8_t>(counter_ >> 8);
    signal_buf_[channel_num_*4+2] = static_cast<uint8_t>(counter_ >> 16);
    signal_buf_[channel_num_*4+3] = static_cast<uint8_t>(counter_ >> 24);
    signal_buf_mutex_.unlock();
    emit SignalGenerated();
    counter_++;

}
