#include "lib/log.h"
#include "lib/util.h"
#include <iostream>

int main(int argc, char **argv) {
  tiga::Logger::ptr logger(new tiga::Logger);
  logger->addAppender(tiga::LogAppender::ptr(new tiga::StdoutLogAppender));

  tiga::FileLogAppender::ptr file_appender(
      new tiga::FileLogAppender("./log.txt"));
  tiga::LogFormatter::ptr fmt(new tiga::LogFormatter("%d%T%p%T%m%n"));
  file_appender->setFormatter(fmt);
  file_appender->setLevel(tiga::LogLevel::ERROR);

  logger->addAppender(file_appender);

  // tiga::LogEvent::ptr event(new tiga::LogEvent(__FILE__, __LINE__, 0,
  // tiga::GetThreadId(), tiga::GetFiberId(), time(0))); event->getSS() <<
  // "hello tiga log"; logger->log(tiga::LogLevel::DEBUG, event);
  std::cout << "hello tiga log" << std::endl;

  TIGA_LOG_INFO(logger) << "test macro";
  TIGA_LOG_ERROR(logger) << "test macro error";

  TIGA_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

  auto l = tiga::LoggerMgr::GetInstance()->getLogger("xx");
  TIGA_LOG_INFO(l) << "xxx";
  return 0;
}