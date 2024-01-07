#include "logger.h"
#include <cstdarg>
#include <cstdio>
#include <pthread.h>
#include <thread>
#include <vector>
#include "time.h"

using namespace y::lib;
using namespace std;

Logger::LogLevel Logger::lowLevel = LogLevel::DEBUG;
vector<char> Logger::log;

void Logger::LogConfig(LogLevel level, unsigned int logLen)
{
  lowLevel = level;
  log.resize(logLen);
}

string Logger::LogLevelStr(LogLevel level)
{
  switch (level) {
    case LogLevel::DEBUG: {
      return "DEBUG";
    }
    case LogLevel::INFO: {
      return "INFO";
    }
    case LogLevel::WARN: {
      return "WARN";
    }
    case LogLevel::ERROR: {
      return "ERROR";
    }
      // case LogLevel::CRITICAL: {
    default: {
      return "CRITICAL";
    }
  }
}

int Logger::LogWrite(LogLevel level, const char *fileName, unsigned int line,
                     const char *funName, int fd, const char *format, ...)
{
  if (level < lowLevel) return 0;
  if (log.size() == 0) {
    log.resize(4096);
  }

  struct tm tmInfo;
  time_t now = time(nullptr);
  localtime_r(&now, &tmInfo);
  unsigned int len =
      snprintf(&log[0], log.size() - 1,
               "[%d-%d-%d %d:%d:%d][%lu][%s][%s][%s:%u]:",
               tmInfo.tm_year + 1900, tmInfo.tm_mon + 1, tmInfo.tm_mday,
               tmInfo.tm_hour, tmInfo.tm_min, tmInfo.tm_sec, pthread_self(),
               LogLevelStr(level).c_str(), funName, fileName, line);

  va_list args;
  va_start(args, format);
  len += vsnprintf(&log[0] + len, log.size() - len - 1, format, args);
  va_end(args);
  if (len >= log.size() - 2)
  {
    log[log.size() - 2] = '\n';
    log[log.size() - 1] = '\0';
    len = log.size();
  }
  else
  {
    log[len] = '\n';
    log[len + 1] = '\0';
    len += 1;
  }
  write(fd, &log[0], len);
  return 0;
}


