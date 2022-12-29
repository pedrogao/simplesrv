#ifndef __TIGA_LOG_H__
#define __TIGA_LOG_H__

#include "singleton.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <vector>

#define TIGA_LOG_LEVEL(logger, level)                                          \
  if (logger->getLevel() <= level)                                             \
  tiga::LogEventWrap(tiga::LogEvent::ptr(new tiga::LogEvent(                   \
                         logger, level, __FILE__, __LINE__, 0,                 \
                         tiga::GetThreadId(), tiga::GetFiberId(), time(0))))   \
      .getSS()

#define TIGA_LOG_DEBUG(logger) TIGA_LOG_LEVEL(logger, tiga::LogLevel::DEBUG)
#define TIGA_LOG_INFO(logger) TIGA_LOG_LEVEL(logger, tiga::LogLevel::INFO)
#define TIGA_LOG_WARN(logger) TIGA_LOG_LEVEL(logger, tiga::LogLevel::WARN)
#define TIGA_LOG_ERROR(logger) TIGA_LOG_LEVEL(logger, tiga::LogLevel::ERROR)
#define TIGA_LOG_FATAL(logger) TIGA_LOG_LEVEL(logger, tiga::LogLevel::FATAL)

#define TIGA_LOG_FMT_LEVEL(logger, level, fmt, ...)                            \
  if (logger->getLevel() <= level)                                             \
  tiga::LogEventWrap(tiga::LogEvent::ptr(new tiga::LogEvent(                   \
                         logger, level, __FILE__, __LINE__, 0,                 \
                         tiga::GetThreadId(), tiga::GetFiberId(), time(0))))   \
      .getEvent()                                                              \
      ->format(fmt, __VA_ARGS__)

#define TIGA_LOG_FMT_DEBUG(logger, fmt, ...)                                   \
  TIGA_LOG_FMT_LEVEL(logger, tiga::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define TIGA_LOG_FMT_INFO(logger, fmt, ...)                                    \
  TIGA_LOG_FMT_LEVEL(logger, tiga::LogLevel::INFO, fmt, __VA_ARGS__)
#define TIGA_LOG_FMT_WARN(logger, fmt, ...)                                    \
  TIGA_LOG_FMT_LEVEL(logger, tiga::LogLevel::WARN, fmt, __VA_ARGS__)
#define TIGA_LOG_FMT_ERROR(logger, fmt, ...)                                   \
  TIGA_LOG_FMT_LEVEL(logger, tiga::LogLevel::ERROR, fmt, __VA_ARGS__)
#define TIGA_LOG_FMT_FATAL(logger, fmt, ...)                                   \
  TIGA_LOG_FMT_LEVEL(logger, tiga::LogLevel::FATAL, fmt, __VA_ARGS__)

#define TIGA_LOG_ROOT() tiga::LoggerMgr::GetInstance()->getRoot()

namespace tiga {
class Logger;

// 日志级别
class LogLevel {
public:
  enum Level {
    UNKNOW = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
  };

  static const char *ToString(LogLevel::Level level);
};

// 日志事件
class LogEvent {
public:
  typedef std::shared_ptr<LogEvent> ptr;

  LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
           const char *file, int32_t line, uint32_t elapse, uint32_t thread_id,
           uint32_t fiber_id, uint64_t time);

  const char *getFile() const { return m_file; }
  int32_t getLine() const { return m_line; }
  uint32_t getElapse() const { return m_elapse; }
  uint32_t getThreadId() const { return m_thread_id; }
  uint32_t getFiberId() const { return m_fiber_id; }
  uint64_t getTime() const { return m_time; }
  std::string getContent() const { return m_ss.str(); }
  std::shared_ptr<Logger> getLogger() const { return m_logger; }
  LogLevel::Level getLevel() const { return m_level; }

  std::stringstream &getSS() { return m_ss; }
  void format(const char *fmt, ...);
  void format(const char *fmt, va_list al);

private:
  const char *m_file = nullptr; // 文件名
  int32_t m_line = 0;           // 行号
  uint32_t m_elapse = 0;        // 程序运行时间，单位毫秒
  uint32_t m_thread_id = 0;     // 线程id
  uint32_t m_fiber_id = 0;      // 协程id
  uint64_t m_time = 0;          // 时间戳
  std::stringstream m_ss;

  std::shared_ptr<Logger> m_logger;
  LogLevel::Level m_level;
};

class LogEventWrap {
public:
  LogEventWrap(LogEvent::ptr e);
  ~LogEventWrap();

  LogEvent::ptr getEvent() const { return m_event; }
  std::stringstream &getSS();

private:
  LogEvent::ptr m_event;
};

// 日志格式转换器
class LogFormatter {
public:
  typedef std::shared_ptr<LogFormatter> ptr;
  LogFormatter(const std::string &pattern);

  std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     LogEvent::ptr event);

public:
  class FormatItem {
  public:
    typedef std::shared_ptr<FormatItem> ptr;
    virtual ~FormatItem() = default;
    virtual void format(std::ostream &os, std::shared_ptr<Logger> logger,
                        LogLevel::Level level, LogEvent::ptr event) = 0;
  };

  void init();

private:
  std::string m_pattern;
  std::vector<FormatItem::ptr> m_items;
};

// 日志输出
class LogAppender {
public:
  typedef std::shared_ptr<LogAppender> ptr;
  virtual ~LogAppender() = default;

  virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                   LogEvent::ptr event) = 0;

  void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
  LogFormatter::ptr getFormatter() const { return m_formatter; }

  LogLevel::Level getLevel() const { return m_level; }
  void setLevel(LogLevel::Level level) { m_level = level; }

protected:
  LogLevel::Level m_level = LogLevel::Level::DEBUG;
  LogFormatter::ptr m_formatter;
};

// 日志
class Logger : public std::enable_shared_from_this<Logger> {
public:
  typedef std::shared_ptr<Logger> ptr;

  Logger(const std::string &name = "root");

  void log(LogLevel::Level level, LogEvent::ptr event);

  void debug(LogEvent::ptr event);
  void info(LogEvent::ptr event);
  void warn(LogEvent::ptr event);
  void error(LogEvent::ptr event);
  void fatal(LogEvent::ptr event);

  void addAppender(LogAppender::ptr appender);
  void delAppender(LogAppender::ptr appender);

  LogLevel::Level getLevel() const { return m_level; }
  void setLevel(LogLevel::Level level) { m_level = level; }

  const std::string &getName() const { return m_name; }

private:
  std::string m_name; // 日志名
  LogLevel::Level m_level;
  std::list<LogAppender::ptr> m_appenders;
  LogFormatter::ptr m_formatter;
};

class StdoutLogAppender : public LogAppender {
public:
  typedef std::shared_ptr<StdoutLogAppender> ptr;

  void log(Logger::ptr logger, LogLevel::Level level,
           LogEvent::ptr event) override;
};

class FileLogAppender : public LogAppender {
public:
  typedef std::shared_ptr<FileLogAppender> ptr;

  FileLogAppender(const std::string &filename);

  void log(Logger::ptr logger, LogLevel::Level level,
           LogEvent::ptr event) override;

  bool reopen(); // 重新打开文件

private:
  std::string m_filename;
  std::ofstream m_filestream;
};

// 日志管理器
class LoggerManager {
public:
  LoggerManager();
  Logger::ptr getLogger(const std::string &name);

  void init();
  Logger::ptr getRoot() const { return m_root; }

private:
  std::map<std::string, Logger::ptr> m_loggers;
  Logger::ptr m_root;
};

typedef tiga::Singleton<LoggerManager> LoggerMgr;

} // namespace tiga

#endif