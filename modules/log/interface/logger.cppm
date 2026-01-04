module;

#include <utility>

#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"

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
                this->logger.setLogLevel(static_cast<log4cplus::LogLevel>(static_cast<int>(log_level)));
            }

            [[nodiscard]] LogLevel getLogLevel() const noexcept
            {
                return static_cast<LogLevel>(this->logger.getLogLevel());
            }

            [[nodiscard]] Bool isLogLevelEnabled(LogLevel log_level) const noexcept
            {
                return this->logger.isEnabledFor(static_cast<log4cplus::LogLevel>(static_cast<int>(log_level)));
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
                    output_message = std::format(message, std::forward<ARGS>(args)...);
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
