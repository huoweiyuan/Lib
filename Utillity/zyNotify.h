#ifndef ZYNOTIFY_H
#define ZYNOTIFY_H

#include "zyHandler.h"
#include "zyAssert.h"
namespace zy
{
class Notify
{
 public:
   virtual ~Notify() = default;

 public:
   template<typename T>
   int notify(Handler<T> *handler, T *data)
   {
     Assert(handler != nullptr && data != nullptr);
     return handler->handing(data);
   }
};
}

#endif
