// Create by huoweiyuan on 2022/11/26

#ifndef ZY_TIMER_H
#define ZY_TIMER_H

#include <vector>
#include <functional>
#include "zyThread.h"
#include <iostream>

namespace zy
{
namespace utillity
{

class Timer
{
 public:
  // 毫秒
  using TimerMillis = int64_t;

 private:
  struct TimerItem
  {
    std::function<void(void)> mFunc;
    TimerMillis mExpireTime, mWaitTime;
  };
  std::vector<TimerItem> mItems;

 protected:
  int64_t mSleepMillis;

 public:
  Timer()
    : mSleepMillis(0)
  {}
  
  Timer(TimerMillis sleepTimeMillis)
    : mSleepMillis(sleepTimeMillis)
  {
    if (mSleepMillis < 0)
      sleepTimeMillis = 0;
  }

  ~Timer() = default;
  
 public:
  template <typename Func, typename... Args>
    int setTimer(TimerMillis milli, Func &&func, Args&&... args)
  {
    auto job = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    TimerItem timerItem;
    timerItem.mFunc = [job]() {
      job();
    };
    timerItem.mWaitTime = milli;
    timerItem.mExpireTime = now() + milli;
    mItems.emplace_back(timerItem);
    return 0;
  }

  void checkTimeExpire(void)
  {
    int64_t _now = now();
    for (TimerItem &item : mItems) {
      if (item.mExpireTime < _now) {
        item.mFunc();
        item.mExpireTime = _now + item.mWaitTime;
      }
    }
  }

  void sleepTimeMillis(TimerMillis milli = 0)
  {
    if (milli <= 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(mSleepMillis));
    }
    else {
      std::this_thread::sleep_for(std::chrono::milliseconds(milli));
    }
  }

 private:
  int64_t now() const
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  }

};

class TimerImpl : public Timer, public Thread
{
 public:
  TimerImpl(TimerMillis sleepTimeMillis)
    : Timer(sleepTimeMillis), Thread(false)
  {
  }
  ~TimerImpl() = default;
  
 public:

  int thrdRoutine(void) override
  {
    sleepTimeMillis(mSleepMillis);
    checkTimeExpire();
    return 0;
  }
};
}
}

#endif /* ZY_TIMER_H */
