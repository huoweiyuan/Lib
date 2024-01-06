//
// Created by 霍伟元 on 2022/10/16.
//

#ifndef CPPTEC_LIB_TOOL_ZY_THROW_ERROR_H_
#define CPPTEC_LIB_TOOL_ZY_THROW_ERROR_H_

#include <string.h>
#include <string>

//#define E_DEFAULT (-1)

namespace zy
{

class ThrowError
{
 public:
  enum ENUMBER {E_DEFAULT=-1};
 public:
  ThrowError() = default;
  ThrowError(const char* eMsg, ENUMBER eNo)
  {
    mEMsg = eMsg;
    mENo = eNo;
  }

  ~ThrowError() = default;

 public:
  const char* eMsg() const
  {
    return mEMsg.c_str();
  }

  ENUMBER eNo() const
  {
    return mENo;
  }

 private:
  ENUMBER mENo;
  std::string mEMsg;
};

}

#endif //CPPTEC_LIB_TOOL_ZY_THROW_ERROR_H_
