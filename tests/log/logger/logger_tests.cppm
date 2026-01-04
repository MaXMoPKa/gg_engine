module;

#include <gtest/gtest.h>

#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>

export module tests.logger;

import log.logger;

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_logger_ = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("SimpleTestLogger"));
        test_logger_.setLogLevel(log4cplus::INFO_LOG_LEVEL);

        log4cplus::SharedAppenderPtr console_appender(new log4cplus::ConsoleAppender());
        console_appender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(LOG4CPLUS_TEXT("%d{%H:%M:%S} %-5p %c{1} - %m%n"))));

        test_logger_.addAppender(console_appender);
    }

    log4cplus::Logger test_logger_;
};

export TEST_F(LoggerTest, LogSimpleMessage) {
    gg::log::Logger logger(test_logger_);

    logger.logInfo("Hello, this is a simple log message!");

    EXPECT_EQ(logger.getLogLevel(), gg::log::LogLevel::INFO);
}
