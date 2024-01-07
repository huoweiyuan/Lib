#ifndef _ZY_TOOL_
#define _ZY_TOOL_

#include <cstddef>

namespace zy
{
namespace utillity
{

class DISABLE_NEW_OPERATE
{
  void* operator new(size_t) = delete;
  void* operator new[](size_t) = delete;
  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;
};

class DISABLE_COPY_OPERATE
{
 protected:
   DISABLE_COPY_OPERATE() = default;
   ~DISABLE_COPY_OPERATE() = default;
   DISABLE_COPY_OPERATE(const DISABLE_COPY_OPERATE&) = delete;
   DISABLE_COPY_OPERATE& operator=(const DISABLE_COPY_OPERATE&) = delete;
};

}
}

#endif // _ZY_TOOL_
