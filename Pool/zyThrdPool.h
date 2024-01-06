#ifndef _THRD_POOL_
#define _THRD_POOL_

#include <memory>
#include <functional>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <algorithm>

namespace zy
{

class ThrdPool
{
 protected:
   struct Job
   {
     Job() : mTimeout(0), mExpireTime(0) {}
     explicit Job(int64_t timeout)
       : mTimeout(timeout), mExpireTime(0)
     {}
     std::function<void()> mFunc;
     int64_t mTimeout;
     int64_t mExpireTime;
   };
   using JobPtr = std::shared_ptr<Job>;

 public:
   ThrdPool();

   // @para thrdNum: 初始化线程个数
   explicit ThrdPool(int thrdNum);

   virtual ~ThrdPool() = default;

 public:
   // 初始化线程个数
   // @param thrdNum: 线程个数
   // @return int: 成功与否
   int setThrdNum(size_t thrdNum);

   // 初始化线程
   // @return int: 成功与否
   int start();

   // 停止线程
   // @return int: 成功与否
   int stop();

   // 获取线程个数
   // @return size_t: 线程个数
   size_t getThrdNum() const;

   // 添加线程执行任务
   // @param timeout_ms: 超时时间
   // @param f: 任务函数
   // @param p: 任务函数参数
   // @return std::future<decltype(f(p...))>: 任务函数返回值
   template<typename F, typename... P>
     auto exec(uint64_t timeout_ms, F&& f, P&&... p) -> std::future<decltype(f(p...))>;

   // 添加线程执行任务
   // @param f: 任务函数
   // @param p: 任务函数参数
   // @return std::future<decltype(f(p...))>: 任务函数返回值
   template<typename F, typename... P>
     auto exec(F&& f, P&&... p) -> std::future<decltype(f(p...))>;

   // 等待所有任务结束
   // @para timeout_ms: 等待超时
   int wait(int64_t timeout_ms = -1);

   // 线程池是否退出
   // @return bool: true 退出，false 没退出
   bool isTerminate() const;

   // 从队列中获取任务
   // @return JobPtr: 任务
   JobPtr get();

 private:
   // 返回当前时间戳
   // @return int64_t: 时间戳
   int64_t now() const;

   // 线程运行函数
   void run();

 protected:
   // 任务队列
   std::queue<JobPtr> mJobs;

   // 线程
   std::vector<std::thread*> mThreads;

   mutable std::mutex mMutex;

   std::condition_variable mCondition, mWaitCondition;

   // 线程数量
   size_t mThrdNum;

   // 线程池停止
   bool mTerminated;

   std::atomic<int> mAtomic{0};
};
ThrdPool::ThrdPool()
  : mThrdNum(0),
  mTerminated(true)
{}

ThrdPool::ThrdPool(int thrdNum)
  : mThrdNum(thrdNum),
  mTerminated(true)
{}


int ThrdPool::setThrdNum(size_t thrdNum)
{
  if (mTerminated)
  {
    mThrdNum = thrdNum;
    return true;
  } 
  else
  {
    return false;
  }
}

int ThrdPool::start()
{
  std::unique_lock<std::mutex> _lock(mMutex);

  if (!mThreads.empty())
  {
    return -1;
  }

  mTerminated = false;

  for (size_t i = 0; i < mThrdNum; i++)
  {
    mThreads.push_back(new std::thread(&ThrdPool::run, this));
  }

  return 0;
}

int ThrdPool::stop()
{
  std::vector<std::thread*> _threads;
  {
    std::unique_lock<std::mutex> _lock(mMutex);
    mTerminated = true;
    mCondition.notify_all();
    _threads.swap(mThreads);
  }

  std::for_each(_threads.begin(), _threads.end(), [] (std::thread *thrd) {
    if (thrd->joinable())
    {
      thrd->join();
    }
  });

  _threads.clear();
  return 0;
}


size_t ThrdPool::getThrdNum() const
{
  return mThrdNum;
}

int64_t ThrdPool::now() const
{
  return std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}


void ThrdPool::run()
{
  while (!mTerminated)
  {
    JobPtr job = get();
    if (job != nullptr)
    {
      ++mAtomic;
      if (!(job->mExpireTime != 0 && job->mExpireTime < now())) { // 执行未超时任务, 超时任务直接释放
        job->mFunc();
      }
      --mAtomic;
      mWaitCondition.notify_one();
    }

  }
}

template<typename F, typename... P>
auto ThrdPool::exec(uint64_t timeout_ms, F&& f, P&&... p) -> std::future<decltype(f(p...))>
{
  using RetType = decltype(f(p...));
  auto job = std::make_shared<std::packaged_task<RetType()>>
    (std::bind(std::forward<F>(f), std::forward<P>(p)...));

  JobPtr jobPtr = std::make_shared<Job>(timeout_ms);
  if (jobPtr->mTimeout) jobPtr->mExpireTime = now() + timeout_ms;
  jobPtr->mFunc = [job]() {
    (*job)();
  };

  std::unique_lock<std::mutex> _lock(mMutex);
  mJobs.push(jobPtr);
  mCondition.notify_one();
  return job->get_future();
}

template<typename F, typename... P>
auto ThrdPool::exec(F&& f, P&&... p) -> std::future<decltype(f(p...))>
{
  return exec(0, f, p...);
}

int ThrdPool::wait(int64_t timeout_ms)
{
  std::cout << "ThrdPool::wait(int64_t timeout_ms)" << std::endl;
  int64_t expireTime = now() + timeout_ms;
  std::unique_lock<std::mutex> _lock(mMutex);
  while (!mJobs.empty() || mAtomic != 0) {
    if (expireTime < now()) {
      return -1;
    }
    mWaitCondition.wait_for(_lock, std::chrono::milliseconds(timeout_ms), [this]
    {
      return mTerminated || (mJobs.empty() && mAtomic == 0);
    });
  }
  return 0;
}

bool ThrdPool::isTerminate() const
{
  return mTerminated;
}

ThrdPool::JobPtr ThrdPool::get()
{
  std::unique_lock<std::mutex> _lock(mMutex);
  if (mJobs.empty())
  {
    mCondition.wait(_lock, [this] {return mTerminated || !mJobs.empty();});
  }

  if (mTerminated) return nullptr;

  if (!mJobs.empty())
  {
    JobPtr job = mJobs.front();   
    mJobs.pop();
    return job;
  }
  return nullptr;
}

}


#endif //_THRD_POOL_
