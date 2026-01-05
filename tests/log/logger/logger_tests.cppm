module;

#include <gtest/gtest.h>

#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>

export module tests.logger;

import log.logger;

import types.smart_pointers;
import types.base_types;

namespace gg
{
namespace log
{
namespace tests
{
    class LoggerTest : public ::testing::Test
    {
        protected:
            void SetUp() override
            {
                log4cplus::initialize();
        
                log4cplus_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TestLogger"));
        
                log4cplus::SharedAppenderPtr console_appender(new log4cplus::ConsoleAppender);
                UniquePtr<log4cplus::PatternLayout> layout = std::make_unique<log4cplus::PatternLayout>(LOG4CPLUS_TEXT("%d{%Y-%m-%d %H:%M:%S} [%t] %-5p %c - %m%n"));
                console_appender->setLayout(std::move(layout));
                log4cplus_logger.addAppender(console_appender);
        
                logger = std::make_unique<gg::log::Logger>(log4cplus_logger);
            }

            void TearDown() override
            {
                logger.reset();
                log4cplus::Logger::shutdown();
            }

            log4cplus::Logger log4cplus_logger;
            UniquePtr<gg::log::Logger> logger;
    }; // class LoggerTest;

    TEST_F(LoggerTest, ConstructorWithValidLogger)
    {
        EXPECT_NO_THROW({
                log4cplus::Logger test_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Test"));
                gg::log::Logger logger_instance(test_logger);
            });
    }

    TEST_F(LoggerTest, MoveConstructor)
    {
        log4cplus::Logger test_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("MoveTest"));
        gg::log::Logger original_logger(test_logger);
        
        gg::log::Logger moved_logger(std::move(original_logger));
    }

    TEST_F(LoggerTest, MoveAssignmentOperator)
    {
        log4cplus::Logger test_logger1 = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("MoveTest1"));
        log4cplus::Logger test_logger2 = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("MoveTest2"));
    
        gg::log::Logger logger1(test_logger1);
        gg::log::Logger logger2(test_logger2);

        LogLevel original_level = LogLevel::INFO;
        logger2.setLogLevel(original_level);
    
        logger1 = std::move(logger2);

        EXPECT_EQ(logger1.getLogLevel(), original_level);
    }

    TEST_F(LoggerTest, CopyConstructorDeleted)
    {
        static_assert(!std::is_copy_constructible_v<gg::log::Logger>, "Logger should not be copy constructible");
    }

    TEST_F(LoggerTest, CopyAssignmentOperatorDeleted)
    {
        static_assert(!std::is_copy_assignable_v<gg::log::Logger>, "Logger should not be copy assignable");
    }

    TEST_F(LoggerTest, SetLogLevelAllLevels)
    {
        for (U8 level = static_cast<U8>(LogLevel::TRACE); level < static_cast<U8>(LogLevel::COUNT); ++level)
        {
            LogLevel log_level = static_cast<LogLevel>(level);
            logger->setLogLevel(log_level);
            EXPECT_EQ(logger->getLogLevel(), log_level);
        }
    }

    TEST_F(LoggerTest, SetLogLevelCount)
    {
        logger->setLogLevel(LogLevel::COUNT);
        EXPECT_EQ(logger->getLogLevel(), LogLevel::COUNT);
    }

    TEST_F(LoggerTest, GetLogLevel)
    {
        logger->setLogLevel(LogLevel::INFO);
        EXPECT_EQ(logger->getLogLevel(), LogLevel::INFO);
    
        logger->setLogLevel(LogLevel::DEBUG);
        EXPECT_EQ(logger->getLogLevel(), LogLevel::DEBUG);
    
        logger->setLogLevel(LogLevel::ERROR);
        EXPECT_EQ(logger->getLogLevel(), LogLevel::ERROR);
    }

    TEST_F(LoggerTest, IsLogLevelEnabledWhenEnabled)
    {
        logger->setLogLevel(LogLevel::INFO);
    
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::TRACE));
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::DEBUG));
        EXPECT_TRUE(logger->isLogLevelEnabled(LogLevel::INFO));
        EXPECT_TRUE(logger->isLogLevelEnabled(LogLevel::WARNING));
        EXPECT_TRUE(logger->isLogLevelEnabled(LogLevel::ERROR));
        EXPECT_TRUE(logger->isLogLevelEnabled(LogLevel::FATAL));
    }

    TEST_F(LoggerTest, IsLogLevelEnabledWhenDisabled)
    {
        logger->setLogLevel(LogLevel::FATAL);
    
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::TRACE));
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::DEBUG));
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::INFO));
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::WARNING));
        EXPECT_FALSE(logger->isLogLevelEnabled(LogLevel::ERROR));
        EXPECT_TRUE(logger->isLogLevelEnabled(LogLevel::FATAL));
    }

    TEST_F(LoggerTest, LogTrace)
    {
        logger->setLogLevel(LogLevel::TRACE);
        EXPECT_NO_THROW(logger->logTrace("Test trace message"));
        EXPECT_NO_THROW(logger->logTrace("Test trace message with {} and {}", 42, "string"));
    }

    TEST_F(LoggerTest, LogDebug)
    {
        logger->setLogLevel(LogLevel::DEBUG);
        EXPECT_NO_THROW(logger->logDebug("Test debug message"));
        EXPECT_NO_THROW(logger->logDebug("Test debug message with {} and {}", 42, "string"));
    }

    TEST_F(LoggerTest, LogInfo)
    {
        logger->setLogLevel(LogLevel::INFO);
        EXPECT_NO_THROW(logger->logInfo("Test info message"));
        EXPECT_NO_THROW(logger->logInfo("Test info message with {} and {}", 42, "string"));
    }

    TEST_F(LoggerTest, LogWarning)
    {
        logger->setLogLevel(LogLevel::WARNING);
        EXPECT_NO_THROW(logger->logWarning("Test warning message"));
        EXPECT_NO_THROW(logger->logWarning("Test warning message with {} and {}", 42, "string"));
    }

    TEST_F(LoggerTest, LogError)
    {
        logger->setLogLevel(LogLevel::ERROR);
        EXPECT_NO_THROW(logger->logError("Test error message"));
        EXPECT_NO_THROW(logger->logError("Test error message with {} and {}", 42, "string"));
    }

    TEST_F(LoggerTest, LogFatal)
    {
        logger->setLogLevel(LogLevel::FATAL);
        EXPECT_NO_THROW(logger->logFatal("Test fatal message"));
        EXPECT_NO_THROW(logger->logFatal("Test fatal message with {} and {}", 42, "string"));
    }

    TEST_F(LoggerTest, LogWhenDisabled)
    {
        logger->setLogLevel(LogLevel::ERROR);
    
        EXPECT_NO_THROW(logger->logTrace("This should not be logged"));
        EXPECT_NO_THROW(logger->logDebug("This should not be logged"));
        EXPECT_NO_THROW(logger->logInfo("This should not be logged"));
        EXPECT_NO_THROW(logger->logWarning("This should not be logged"));
    
        EXPECT_NO_THROW(logger->logError("This should be logged"));
        EXPECT_NO_THROW(logger->logFatal("This should be logged"));
    }

    TEST_F(LoggerTest, LogWithFormatArguments)
    {
        logger->setLogLevel(LogLevel::INFO);
    
        EXPECT_NO_THROW(logger->logInfo("Number: {}", 42));
        EXPECT_NO_THROW(logger->logInfo("String: {}", std::string("test")));
        EXPECT_NO_THROW(logger->logInfo("Multiple: {}, {}, {}", 1, 2.5, "three"));
        EXPECT_NO_THROW(logger->logInfo("No arguments"));
    }

    TEST_F(LoggerTest, LogLevelEnumValues)
    {
        EXPECT_EQ(static_cast<U8>(LogLevel::TRACE), 0);
        EXPECT_EQ(static_cast<U8>(LogLevel::DEBUG), 1);
        EXPECT_EQ(static_cast<U8>(LogLevel::INFO), 2);
        EXPECT_EQ(static_cast<U8>(LogLevel::WARNING), 3);
        EXPECT_EQ(static_cast<U8>(LogLevel::ERROR), 4);
        EXPECT_EQ(static_cast<U8>(LogLevel::FATAL), 5);
        EXPECT_EQ(static_cast<U8>(LogLevel::COUNT), 6);
    }

    TEST_F(LoggerTest, Destructor) 
    {
        EXPECT_NO_THROW({
                log4cplus::Logger test_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DestructorTest"));
                gg::log::Logger logger_instance(test_logger);
            });
    }
} // namespace tests;
} // namespace log;
} // namespace gg;
