#ifndef UTL_LOGHANDLER_H
#define UTL_LOGHANDLER_H

#include "utl/log/loglevel.h"
#include "utl/log/logrecord.h"


namespace utl {

class LogHandler
{
public:
	LogHandler();
	virtual ~LogHandler() noexcept = default;

	const LogLevel &getLevel() const;
	void setLevel(const LogLevel &level);

	void handle(const LogRecord &record);

protected:
	virtual void publish(const LogRecord &record) = 0;

private:
	LogLevel level;

};


inline LogHandler::LogHandler() :
	level(LogLevel::ALL)
{
}

inline const LogLevel &LogHandler::getLevel() const
{
	return this->level;
}

inline void LogHandler::setLevel(const LogLevel &level)
{
	this->level = level;
}

inline void LogHandler::handle(const LogRecord &record)
{
	if (record.level >= this->getLevel())
		publish(record);
}

} // namespace utl

#endif // UTL_LOGHANDLER_H
