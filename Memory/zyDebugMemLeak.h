//
// Created by 霍伟元 on 2022/12/1.
//

#ifndef CPPTEC_LIB_MEMORY_ZYDEBUGMEMLEAK_H_
#define CPPTEC_LIB_MEMORY_ZYDEBUGMEMLEAK_H_

#include "zyAllocator.h"
#include <set>
#include <stdlib.h>
#include <sys/mman.h>

namespace zy
{
class DebugMemLeak : public Allocator {
 private:
  const static uint mMagicNumber = 0xD1251CAE;
  std::size_t mPageSize = 4096;
  std::set<void *, ulong> mMemPtr;
 public:
  char *alloc_memory(std::size_t size) override {
    char *pRet = nullptr;
    posix_memalign((void **) &pRet, mPageSize, size + mPageSize);
    if (mprotect(pRet, mPageSize, PROT_READ) != 0) {
      return nullptr;
    }
    return pRet + mPageSize;
  }
};
}

#endif //CPPTEC_LIB_MEMORY_ZYDEBUGMEMLEAK_H_
