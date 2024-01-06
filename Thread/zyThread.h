// Created by huoweiyuan on 2022/11/26

#ifndef ZY_THREAD_H
#define ZY_THREAD_H

#include <thread>
#include <functional>

namespace zy {

class Thread
{
 private:
  std::function<void(void)> mJob;
  
 public:
  Thread() : mOneShot(true), mTerminated(false) {}
  explicit Thread(bool _oneShot) : mOneShot(_oneShot), mTerminated(false) {}
  virtual ~Thread() = default;

 public:
  virtual int thrdBegin(void) {return 0;}
  virtual int thrdRoutine(void)
  {
    if (mJob) {
      mJob();
    }
    return 0;
  }
  virtual int thrdEnd(void) {return 0;}

 public:
  template <typename Func, typename... Args>
    void execRoutine(Func &&func, Args&&... args)
  {
    auto job =
      std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    mJob = [job]() {
      job();
    };
  }

 private:
  void startRoutine()
  {
    if (thrdBegin() != 0) return;
    while (!mTerminated) {
      if (thrdRoutine() != 0) return;
    }
    if (thrdEnd() != 0) return;
  }

 public:
  int start(void)
  {
    mThread = new std::thread(&Thread::startRoutine, this);
    if (mThread) return 0;
    return -1;
  }
  
  int stop(void)
  {
    mTerminated = true;
    return 0;
  }
  
  int stopAndJoin(void)
  {
    if (stop() != 0) return -1;
    if (mThread->joinable()) {
      mThread->join();
    }
    return 0;
  }

  void join(void)
  {
    mThread->join();
  }
  
  void detach(void)
  {
    mThread->detach();
  }
  
  std::thread::id getThrdId(void) const
  {
    return mThread->get_id();
  }

 private:
  const bool mOneShot;
  bool mTerminated;
  std::thread *mThread;
};

}

#endif // ZY_THREAD_H
