#ifndef UTL_LOGLEVEL_H
#define UTL_LOGLEVEL_H

#include <climits>
#include <ostream>
#include <memory>
#include <string>


namespace utl {

class LogLevel final
{
public:
	static const LogLevel ALL;
	static const LogLevel FINEST;
	static const LogLevel FINER;
	static const LogLevel FINE;
	static const LogLevel CONFIG;
	static const LogLevel INFO;
	static const LogLevel WARNING;
	static const LogLevel SEVERE;
	static const LogLevel OFF;

	explicit LogLevel(int value) noexcept;
	LogLevel(int value, std::string name);
	explicit operator int() const noexcept;

	bool operator==(const LogLevel &other) const noexcept;
	bool operator!=(const LogLevel &other) const noexcept;
	bool operator>=(const LogLevel &other) const noexcept;
	bool operator<=(const LogLevel &other) const noexcept;
	bool operator>(const LogLevel &other) const noexcept;
	bool operator<(const LogLevel &other) const noexcept;

	friend std::ostream &operator<< (std::ostream &stream, LogLevel level);

private:
	struct Data {
		std::string mName;
	};

	int mValue;
	std::shared_ptr<Data> mData;

};


inline LogLevel::LogLevel(int value) noexcept :
	mValue(value)
{
}

inline LogLevel::LogLevel(int value, std::string name) :
	mValue(value),
	mData(std::make_shared<Data>())
{
	mData->mName = name;
}

inline LogLevel::operator int() const noexcept
{
	return mValue;
}

inline bool LogLevel::operator==(const LogLevel &other) const noexcept
{
	return (this->mValue == other.mValue);
}

inline bool LogLevel::operator!=(const LogLevel &other) const noexcept
{
	return (this->mValue != other.mValue);
}

inline bool LogLevel::operator>=(const LogLevel &other) const noexcept
{
	return (this->mValue >= other.mValue);
}

inline bool LogLevel::operator<=(const LogLevel &other) const noexcept
{
	return (this->mValue <= other.mValue);
}

inline bool LogLevel::operator>(const LogLevel &other) const noexcept
{
	return (this->mValue > other.mValue);
}

inline bool LogLevel::operator<(const LogLevel &other) const noexcept
{
	return (this->mValue < other.mValue);
}

inline std::ostream &operator<<(std::ostream &stream, LogLevel level)
{
	if (level.mData && !level.mData->mName.empty())
		return stream << level.mData->mName;
	else
		return stream << level.mValue;
}

} // namespace utl

#endif // UTL_LOGLEVEL_H
