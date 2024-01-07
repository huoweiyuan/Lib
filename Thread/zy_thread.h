// Created by huoweiyuan on 2022/11/26

#ifndef ZY_THREAD_H
#define ZY_THREAD_H

#include <thread>
#include <functional>

namespace zy {

class Thread
{
 private:
  std::function<void(void)> m_job;
  
 public:
  Thread() : m_oneshot(true), m_terminated(false) {}
  explicit Thread(bool _oneShot) : m_oneshot(_oneShot), m_terminated(false) {}
  virtual ~Thread() = default;

 public:
  virtual int thrd_begin(void) {return 0;}
  virtual int thrd_routine(void)
  {
    if (m_job) {
      m_job();
    }
    return 0;
  }
  virtual int thrd_end(void) {return 0;}

 public:
  template <typename Func, typename... Args>
    void exec_routine(Func &&func, Args&&... args)
  {
    auto job =
      std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    m_job = [job]() {
      job();
    };
  }

 private:
  void start_routine()
  {
    if (thrd_begin() != 0) return;
    while (!m_terminated) {
      if (thrd_routine() != 0) return;
    }
    if (thrd_end() != 0) return;
  }

 public:
  int start(void)
  {
    m_thread = new std::thread(&Thread::start_routine, this);
    if (m_thread) return 0;
    return -1;
  }
  
  int stop(void)
  {
    m_terminated = true;
    return 0;
  }
  
  int stopAndJoin(void)
  {
    if (stop() != 0) return -1;
    if (m_thread->joinable()) {
      m_thread->join();
    }
    return 0;
  }

  void join(void)
  {
    m_thread->join();
  }
  
  void detach(void)
  {
    m_thread->detach();
  }
  
  std::thread::id getThrdId(void) const
  {
    return m_thread->get_id();
  }

 private:
  const bool m_oneshot;
  bool m_terminated;
  std::thread *m_thread;
};

}

#endif // ZY_THREAD_H
