#ifndef _ZY_SINGLETON_H
#define _ZY_SINGLETON_H

#include <mutex>
#include <iostream>

namespace zy {
namespace utillity {

template <typename ClassName>
  class Singleton final
{
 private:
  static ClassName *mInstance;
  static std::mutex mMutex;

 public:
  template <typename... Args>
  static ClassName *getInstance(Args&&... args)
  {
    if (mInstance == nullptr) {
      std::unique_lock<std::mutex> lock(mMutex);
      if (mInstance == nullptr) {
        mInstance = new ClassName(std::forward<Args>(args)...);
      }
    }
    return mInstance;
  }

 public:
  Singleton() = delete;
  ~Singleton() = delete;
  Singleton(const ClassName &) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(const Singleton&) = delete;
};

template<typename ClassName>
  ClassName *Singleton<ClassName>::mInstance = nullptr;

template<typename ClassName>
  std::mutex Singleton<ClassName>::mMutex;

}
}

#endif /* _ZY_SINGLETON_H */
