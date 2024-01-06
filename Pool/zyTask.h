#ifndef _ZY_TASK_
#define _ZY_TASK_
#include <functional>
namespace zy
{

using Callback = std::function<void(void*)>;
struct Task
{
  Task()
    : callback(nullptr),
    arg(nullptr)
  {}

  Task(Callback _callback, void *_arg)
    : callback(_callback),
    arg(_arg)
  {}

  Callback callback;
  void *arg;
};



}
#endif // _ZY_TASK_
