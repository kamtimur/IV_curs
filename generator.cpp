#include "generator.h"

#include <cmath>
#include <cstring>
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
      for ( int32_t i = 0; i < 8; i++ )
      {
          int32_t tmp = (sin(2.*3.14*counter_/500.+ 3.14/3.*i)+1)*50;
          signal_buf_[i*3]= tmp;
          signal_buf_[i*3+1]= tmp>>8;
          signal_buf_[i*3+2]= tmp>>16;
      }
      signal_buf_mutex_.unlock();
      emit SignalGenerated();
      counter_++;

}
