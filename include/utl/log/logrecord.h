#ifndef UTL_LOGRECORD_H
#define UTL_LOGRECORD_H

#include <string>

#include "utl/log/loglevel.h"


namespace utl {
namespace log {

struct LogRecord
{
	std::string loggerName;
	LogLevel level;
	std::string message;
	// infos about exception
	// millis (time)
	// thread id

	LogRecord();
};


inline LogRecord::LogRecord() :
	level(LogLevel::ALL)
{
}

} // namespace log
} // namespace utl

#endif // UTL_LOGRECORD_H
