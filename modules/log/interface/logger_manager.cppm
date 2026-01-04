module;

#include <log4cplus/logger.h>
#include <log4cplus/initializer.h>

export module log.logger_manager;

import log.logger;

import types.unordered_map;
import types.string;
import types.smart_pointers;

namespace gg
{
namespace log
{
    export class LoggerManager
    {
        public:
            LoggerManager();
            ~LoggerManager() = default;

            LoggerManager(const LoggerManager&) = delete;
            LoggerManager& operator=(LoggerManager&) = delete;

        public:
            [[nodiscard]] Logger* getLogger(const String& name = DEFAULT_LOGGER);

            void setLogLevel(LogLevel level) noexcept;

            void setGlobalLogLevel(log4cplus::LogLevel level) noexcept;
        private:
            using LoggerCache = UnorderedMap<String, UniquePtr<Logger>>;

            static constexpr const char* LOG_FILE_NAME = "gg_engine.log";
            static constexpr const char* DEFAULT_LOGGER = "gg_engine";
            static constexpr const char* LOG_PATTERN = "%d{%H:%M:%S,%q} [%t] %-5p %c{1} %x- %m%n";

            log4cplus::Logger root_logger;
            LoggerCache cache;
            log4cplus::Initializer initializer;
    }; // class LoggerManager;
} // namespace log;
} // namespace gg;
