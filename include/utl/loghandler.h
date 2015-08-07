#ifndef UTL_LOGHANDLER_H
#define UTL_LOGHANDLER_H

#include "utl/loglevel.h"
#include "utl/logrecord.h"


namespace utl {

class LogHandler
{
public:
	LogHandler();

	LogLevel getLevel() const;
	void setLevel(LogLevel level);

	virtual void publish(const LogRecord &record) = 0;

private:
	LogLevel level;
};


inline LogHandler::LogHandler() :
	level(LogLevel::ALL)
{
}

inline LogLevel LogHandler::getLevel() const
{
	return this->level;
}

inline void LogHandler::setLevel(LogLevel level)
{
	this->level = level;
}

} // namespace utl

#endif // UTL_LOGHANDLER_H
