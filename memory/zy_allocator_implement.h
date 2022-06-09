#ifndef _ZY_ALLOCATOR_IMPLEMENT_
#define _ZY_ALLOCATOR_IMPLEMENT_

#include "zy_allocator.h"

namespace zy
{
class AllocatorImple : public Allocator
{
  private:
    char* alloc_memory(std::size_t size);
    char* realloc_memory(void *ptr, size_t size);
    void free_memory(void *ptr);
};
}
#endif // _ZY_ALLOCATOR_IMPLEMENT_
