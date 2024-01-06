#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <vector>
#include <string>
#include "unistd.h"

namespace y { namespace lib {

class Logger {
 public:
  Logger() = default;
  virtual ~Logger() = default;

  enum class LogLevel {DEBUG, INFO, WARN, ERROR, CRITICAL};
  static LogLevel lowLevel;
  static std::vector<char> log;
public:
  static int LogWrite(LogLevel level, const char *fileName, unsigned int line, const char *funName, int fd, const char *format, ...);
  static void LogConfig(LogLevel level, unsigned int logLen);
  static std::string LogLevelStr(LogLevel level);
};

#define LOG_DEBUG(fmt, args...)                                                \
  y::lib::Logger::LogWrite(y::lib::Logger::LogLevel::DEBUG, __FILE__,          \
                           __LINE__, __FUNCTION__, STDOUT_FILENO, fmt, ##args)
#define LOG_INFO(fmt, args...)                                                 \
  y::lib::Logger::LogWrite(y::lib::Logger::LogLevel::INFO, __FILE__, __LINE__, \
                           __FUNCTION__, STDOUT_FILENO, fmt, ##args)
#define LOG_WARN(fmt, args...)                                                 \
  y::lib::Logger::LogWrite(y::lib::Logger::LogLevel::WARN, __FILE__, __LINE__, \
                           __FUNCTION__, STDOUT_FILENO, fmt, ##args)
#define LOG_ERROR(fmt, args...)                                                \
  y::lib::Logger::LogWrite(y::lib::Logger::LogLevel::ERROR, __FILE__,          \
                           __LINE__, __FUNCTION__, STDOUT_FILENO, fmt, ##args)
#define LOG_CRITICAL(fmt, args...)                                             \
  y::lib::Logger::LogWrite(y::lib::Logger::LogLevel::CRITICAL, __FILE__,       \
                           __LINE__, __FUNCTION__, STDOUT_FILENO, fmt, ##args)
} // namespace lib
}

#endif
