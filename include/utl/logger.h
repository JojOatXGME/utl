#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "utl/loghandler.h"
#include "utl/loglevel.h"
#include "utl/logrecord.h"
#include "utl/utils.h"


namespace utl {

class Logger
{
public:
	Logger();
	Logger(const std::shared_ptr<Logger> &parent);

	static Logger &getRoot();
	static Logger &get(std::string name);

	void addHandler(std::shared_ptr<LogHandler> handler);
	void setLevel(LogLevel level);
	void removeHandler(std::shared_ptr<LogHandler> handler);

	void log(LogLevel level, std::string msg);
	template <typename... A>
	void log(LogLevel level, std::string format, A... args);

protected:
	void log(const LogRecord &record);

private:
	LogLevel level;
	std::string name;
	std::shared_ptr<Logger> parent;
	std::unordered_set<std::shared_ptr<LogHandler>> handlers;

	static Logger root;
	static std::shared_ptr<Logger> rootSharedPtr;
	static std::unordered_map<std::string, Logger> globalLoggers;
};


inline Logger::Logger() :
	level(LogLevel::CONFIG)
{
}

inline Logger::Logger(const std::shared_ptr<Logger> &parent) :
	level(LogLevel::CONFIG),
	parent(parent)
{
	if (parent != nullptr) {
		level = parent->level;
	}
}

inline Logger &Logger::getRoot()
{
	return Logger::root;
}

inline Logger &Logger::get(std::string name)
{
	if (name.empty())
		return Logger::root;

	auto ret = globalLoggers.emplace(name, Logger(rootSharedPtr));
	if (ret.second) {
		Logger &logger = ret.first->second;
		logger.name = name;
		// TODO setup from configuration
		return logger;
	}
	return ret.first->second;
}

inline void Logger::addHandler(std::shared_ptr<LogHandler> handler)
{
	this->handlers.insert(handler);
}

inline void Logger::setLevel(LogLevel level)
{
	this->level = level;
}

inline void Logger::removeHandler(std::shared_ptr<LogHandler> handler)
{
	this->handlers.erase(handler);
}

inline void Logger::log(LogLevel level, std::string msg)
{
	if (level < this->level)
		return;

	LogRecord record;
	record.loggerName = this->name;
	record.level = level;
	record.message = msg;
	this->log(record);
}

inline void Logger::log(const LogRecord &record)
{
	for (const std::shared_ptr<utl::LogHandler> &handler : this->handlers) {
		handler->publish(record);
	}

	if (this->parent != nullptr) {
		this->parent->log(record);
	}
}

template <typename... A>
inline void Logger::log(LogLevel level, std::string format, A... args)
{
	this->log(level, utl::format(format, args...));
}

} // namespace utl

#endif // LOGGER_H
