//
// Created by 霍伟元 on 2022/10/13.
//

#ifndef CPPTEC_LIB_TOOL_SERVICE_H_
#define CPPTEC_LIB_TOOL_SERVICE_H_

#include <functional>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <future>
#include "zyThrowError.h"

namespace zy
{
namespace utillity
{

template<typename F, typename... P>
auto Daemonize(F&& daemonize_main, P&&... params) -> decltype(daemonize_main(params...))
{
  if (!daemonize_main) {
    throw zy::ThrowError("Main function is empty!", zy::ThrowError::E_DEFAULT);
  }

  // Create daemonize process
  pid_t pid = -1;
  if (-1 == (pid = fork())) {
    throw zy::ThrowError("Fork child process error!", zy::ThrowError::E_DEFAULT);
  }

  // Father process exit
  if (pid > 0) {
    exit(0);
  }

  // Create new session
  if (-1 == (pid = setsid())) {
    throw zy::ThrowError("Create new session error!", zy::ThrowError::E_DEFAULT);
  }

  umask(0);

  return std::bind(std::forward<F>(daemonize_main), std::forward<P>(params)...)();
}

}
}


#endif //CPPTEC_LIB_TOOL_SERVICE_H_
