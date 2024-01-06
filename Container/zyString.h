//
// Created by 霍伟元 on 2022/10/1.
//

#ifndef CPPTEC_STRING_H
#define CPPTEC_STRING_H

#include "zyDef.h"
#include <cstring>
#include <cstdlib>

namespace zy
{

class String
{
 private:
  char *mStr;
  static char mStaticStr[8];
  Size mLen;
  Size mReserveLen;

 public:
  String() : mStr(mStaticStr), mLen(0), mReserveLen(0)
  {}

  String(const String &str) : mStr(mStaticStr), mLen(0), mReserveLen(0)
  {
    *this = str;
  }

  String(String &&str) noexcept : mStr(mStaticStr), mLen(0), mReserveLen(0)
  {
    *this = str;
  }

  String(const char *cStr) noexcept : mStr(mStaticStr), mLen(0), mReserveLen(0)
  {
    if (cStr) {
      Size _mLen = strlen(cStr);
      reserve(_mLen);
      memcpy(mStr, cStr, _mLen);
      mLen = _mLen;
    }
  }

  ~String()
  {
    clear();
  }

 public:
  const char* c_str() const
  {
    return mStr;
  }

 public:
  String& operator=(const String &str)
  {
    if (this == &str) return *this;
    if (str.mLen) {
      if (mLen > str.mLen) {
        clear();
      }
      reserve(str.mLen);
      memcpy(mStr, str.mStr, str.mLen);
    } else {
      clear();
    }
    return *this;
  }

  String& operator=(String &&str) noexcept
  {
    if (str.mLen) {
      clear();
      mStr = str.mStr;
      mLen = str.mLen;
      mReserveLen = str.mReserveLen;
    } else {
      mStr = mStaticStr;
      mLen = 0;
    }

    str.mStr = mStaticStr;
    str.mLen = str.mReserveLen = 0;

    return *this;
  }

  void clear()
  {
    if (mLen) {
      free(mStr);
      mStr = mStaticStr;
      mLen = 0;
      mReserveLen = 0;
    }
  }

 private:
  void reserve(Size len)
  {
    if (mLen >= len || mReserveLen >= len) return;
    char *tmpStr = static_cast<char*>(malloc(len + 1/*for terminal character*/));
    memcpy(tmpStr, mStr, mLen);
    if (mStr != mStaticStr)
      free(mStr);
    mStr = tmpStr;
    mReserveLen = len;
  }
};

char String::mStaticStr[8] = {0};
}

#endif //CPPTEC_STRING_H
