module;

#include <utility>
#include <format>

#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/loglevel.h"

export module log.logger;

import types.string;
import types.base_types;

namespace gg
{
namespace log
{
    export enum class LogLevel : U8
    {
        TRACE = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,

        COUNT
    }; // enum class LogLevel;
    
    export class Logger
    {
       
        public:
            explicit Logger(log4cplus::Logger& logger)
                : logger{std::move(logger)}
            {}
            ~Logger() = default;

            Logger(Logger&&) = default;
            Logger& operator=(Logger&&) = default;
            
            Logger(const Logger&) = delete;
            Logger& operator=(Logger&) = delete;

        public:
            void setLogLevel(LogLevel log_level) noexcept
            {
                log4cplus::LogLevel log4cplus_level;
                switch (log_level)
                {
                    case LogLevel::TRACE:
                    {
                        log4cplus_level = log4cplus::TRACE_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::DEBUG:
                    {
                        log4cplus_level = log4cplus::DEBUG_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::INFO:
                    {
                        log4cplus_level = log4cplus::INFO_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::WARNING:
                    {
                        log4cplus_level = log4cplus::WARN_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::ERROR:
                    {
                        log4cplus_level = log4cplus::ERROR_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::FATAL:
                    {
                        log4cplus_level = log4cplus::FATAL_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::COUNT:
                        [[fallthrough]];
                    default:
                    {
                        log4cplus_level = log4cplus::NOT_SET_LOG_LEVEL;
                        break;
                    }
                }
                this->logger.setLogLevel(log4cplus_level);
            }

            [[nodiscard]] LogLevel getLogLevel() const noexcept
            {
                log4cplus::LogLevel log4cplus_level = this->logger.getLogLevel();
                LogLevel logger_log_level = LogLevel::COUNT;
                if (log4cplus_level == log4cplus::TRACE_LOG_LEVEL)
                {
                    logger_log_level = LogLevel::TRACE;
                }
                else if (log4cplus_level == log4cplus::DEBUG_LOG_LEVEL)
                {
                    logger_log_level = LogLevel::DEBUG;
                }
                else if (log4cplus_level == log4cplus::INFO_LOG_LEVEL)
                {
                    logger_log_level = LogLevel::INFO;
                }
                else if (log4cplus_level == log4cplus::WARN_LOG_LEVEL)
                {
                    logger_log_level = LogLevel::WARNING;
                }
                else if (log4cplus_level == log4cplus::ERROR_LOG_LEVEL)
                {
                    logger_log_level = LogLevel::ERROR;
                }
                else if (log4cplus_level == log4cplus::FATAL_LOG_LEVEL)
                {
                    logger_log_level = LogLevel::FATAL;
                }
                else
                {
                    logger_log_level = LogLevel::COUNT;
                }
                return logger_log_level;
            }

            [[nodiscard]] Bool isLogLevelEnabled(LogLevel log_level) const noexcept
            {
                log4cplus::LogLevel log4cplus_level;
                switch (log_level)
                {
                    case LogLevel::TRACE:
                    {
                        log4cplus_level = log4cplus::TRACE_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::DEBUG:
                    {
                        log4cplus_level = log4cplus::DEBUG_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::INFO:
                    {
                        log4cplus_level = log4cplus::INFO_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::WARNING:
                    {
                        log4cplus_level = log4cplus::WARN_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::ERROR:
                    {
                        log4cplus_level = log4cplus::ERROR_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::FATAL:
                    {
                        log4cplus_level = log4cplus::FATAL_LOG_LEVEL;
                        break;
                    }
                    case LogLevel::COUNT:
                        [[fallthrough]];
                    default:
                    {
                        log4cplus_level = log4cplus::INFO_LOG_LEVEL;
                        break;
                    }
                }
                return this->logger.isEnabledFor(log4cplus_level);
            }

            template<typename... ARGS>
            inline void logTrace(StringView message, ARGS&&... args)
            {
                logImpl<LogLevel::TRACE>(message, std::forward<ARGS>(args)...);
            }

            template<typename... ARGS>
            inline void logDebug(StringView message, ARGS&&... args)
            {
                logImpl<LogLevel::DEBUG>(message, std::forward<ARGS>(args)...);
            }

            template<typename... ARGS>
            inline void logInfo(StringView message, ARGS&&... args)
            {
                logImpl<LogLevel::INFO>(message, std::forward<ARGS>(args)...);
            }

            template<typename... ARGS>
            inline void logWarning(StringView message, ARGS&&... args)
            {
                logImpl<LogLevel::WARNING>(message, std::forward<ARGS>(args)...);
            }

            template<typename... ARGS>
            inline void logError(StringView message, ARGS&&... args)
            {
                logImpl<LogLevel::ERROR>(message, std::forward<ARGS>(args)...);
            }

            template<typename... ARGS>
            inline void logFatal(StringView message, ARGS&&... args)
            {
                logImpl<LogLevel::FATAL>(message, std::forward<ARGS>(args)...);
            }
            
        private:
            template<LogLevel level, typename... ARGS>
            inline void logImpl(StringView message, ARGS&&... args)
            {
                if(!this->isLogLevelEnabled(level))
                {
                    return;
                }
                
                String output_message{message};
                
                if constexpr (sizeof...(args) > 0)
                {
                    output_message = std::vformat(message, std::make_format_args(args...));
                }

                switch (level)
                {
                    case LogLevel::TRACE:
                    {
                        LOG4CPLUS_TRACE(this->logger, output_message.data());
                        break;
                    }
                    case LogLevel::DEBUG:
                    {
                        LOG4CPLUS_DEBUG(this->logger, output_message.data());
                        break;
                    }
                    case LogLevel::INFO:
                    {
                        LOG4CPLUS_INFO(this->logger, output_message.data());
                        break;
                    }
                    case LogLevel::WARNING:
                    {
                        LOG4CPLUS_WARN(this->logger, output_message.data());
                        break;
                    }
                    case LogLevel::ERROR:
                    {
                        LOG4CPLUS_ERROR(this->logger, output_message.data());
                        break;
                    }
                    case LogLevel::FATAL:
                    {
                        LOG4CPLUS_FATAL(this->logger, output_message.data());
                        break;
                    }
                    case LogLevel::COUNT:
                        [[fallthrough]];
                    default:
                    {
                        break;
                    }
                }
            }
            
        private:
            log4cplus::Logger logger;
    }; // class Logger;
} // namespace log;   
} // namespace gg;
