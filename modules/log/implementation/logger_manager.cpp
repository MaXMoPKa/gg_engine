module;

#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/appender.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/layout.h"
#include "log4cplus/tstring.h"

module log.logger_manager;

namespace gg
{
namespace log
{
    LoggerManager::LoggerManager()
    {
        const log4cplus::tstring pattern = LOG4CPLUS_TEXT(LOG_PATTERN);

        log4cplus::SharedAppenderPtr file_app(new log4cplus::FileAppender(LOG4CPLUS_TEXT(LOG_FILE_NAME)));
        file_app->setLayout(UniquePtr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));

        this->root_logger = log4cplus::Logger::getRoot();
        this->root_logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);
        this->root_logger.addAppender(file_app);
    }

    Logger* LoggerManager::getLogger(const String& name)
    {
        auto it = this->cache.find(name);
        if(it == this->cache.end())
        {
            log4cplus::Logger log4cplus_logger = this->root_logger.getInstance(name);
            UniquePtr<Logger> logger = std::make_unique<Logger>(log4cplus_logger);
            this->cache.emplace(name, std::move(logger));
        }

        return this->cache[name].get();
    }

    void LoggerManager::setLogLevel(LogLevel level) noexcept
    {
        for(auto& [name, logger] : cache)
        {
            logger->setLogLevel(level);
        }
    }

    void LoggerManager::setGlobalLogLevel(log4cplus::LogLevel level) noexcept
    {
        this->root_logger.setLogLevel(level);
    }
} // namespace log;
} // namespace gg;
