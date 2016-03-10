#ifndef UTL_LOGGER_H
#define UTL_LOGGER_H

#include <memory>
#include <mutex>
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
	virtual ~Logger() = default;

	static Logger &getRoot();
	static Logger &get(const std::string &name);
	static std::shared_ptr<Logger> getRootP();
	static std::shared_ptr<Logger> getP(const std::string &name);

	const LogLevel &getLevel() const;
	void setLevel(const LogLevel &level);
	void addHandler(std::shared_ptr<LogHandler> handler);
	void removeHandler(std::shared_ptr<LogHandler> handler);

	void log(const LogLevel &level, const std::string &msg) const;
	template <typename... Args>
	void log(const LogLevel &level, const std::string &format, const Args&... args) const;

protected:
	void log(const LogRecord &record) const;

private:
	LogLevel mLevel;
	std::string mName;
	std::shared_ptr<Logger> mParent;
	std::unordered_set<std::shared_ptr<LogHandler>> mHandlers;
	mutable std::mutex mMutex;

	static Logger root;
	static std::shared_ptr<Logger> rootSharedPtr;
	static std::unordered_map<std::string, std::shared_ptr<Logger>> globalLoggers;
	static std::mutex staticMutex;
};


inline Logger::Logger() :
	mLevel(LogLevel::CONFIG)
{
}

inline Logger::Logger(const std::shared_ptr<Logger> &parent) :
	mLevel(LogLevel::CONFIG),
	mParent(parent)
{
	if (mParent != nullptr) {
		mLevel = mParent->mLevel;
	}
}

inline Logger &Logger::getRoot()
{
	return Logger::root;
}

inline Logger &Logger::get(const std::string &name)
{
	return *Logger::getP(name);
}

inline std::shared_ptr<Logger> Logger::getRootP()
{
	return Logger::rootSharedPtr;
}

inline std::shared_ptr<Logger> Logger::getP(const std::string &name)
{
	// use global logger if the name is empty
	if (name.empty())
		return Logger::rootSharedPtr;
	// lock to secure the map (globalLoggers)
	// TODO use a lock which supports concurrent reads?
	std::lock_guard<std::mutex> lock(staticMutex);
	// get the locker
	auto ret = globalLoggers.emplace(name, std::make_shared<Logger>(rootSharedPtr));
	std::shared_ptr<Logger> &logger = ret.first->second;
	if (ret.second) {
		// locker is new, setup
		logger->mName = name;
		// TODO setup from configuration, if available
	}
	return logger;
}

inline const LogLevel &Logger::getLevel() const
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mLevel;
}

inline void Logger::setLevel(const LogLevel &level)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mLevel = level;
}

inline void Logger::addHandler(std::shared_ptr<LogHandler> handler)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mHandlers.insert(handler);
}

inline void Logger::removeHandler(std::shared_ptr<LogHandler> handler)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mHandlers.erase(handler);
}

inline void Logger::log(const LogLevel &level, const std::string &msg) const
{
	if (level < mLevel)
		return;

	LogRecord record;
	record.loggerName = mName;
	record.level = level;
	record.message = msg;
	this->log(record);
}

template <typename... Args>
inline void Logger::log(const LogLevel &level, const std::string &format, const Args&... args) const
{
	this->log(level, utl::format(format, args...));
}

inline void Logger::log(const LogRecord &record) const
{
	// TODO use lock which supports concurrent reads?
	std::lock_guard<std::mutex> lock(mMutex);

	for (const auto &handler : mHandlers) {
		handler->handle(record);
	}

	if (this->mParent != nullptr) {
		this->mParent->log(record);
	}
}

} // namespace utl

#endif // UTL_LOGGER_H
