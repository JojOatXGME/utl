#ifndef UTL_LOGLEVEL_H
#define UTL_LOGLEVEL_H

#include <climits>
#include <ostream>


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

	explicit LogLevel(int value);

	const bool operator==(const LogLevel &other) const;
	const bool operator!=(const LogLevel &other) const;
	const bool operator>=(const LogLevel &other) const;
	const bool operator<=(const LogLevel &other) const;
	const bool operator>(const LogLevel &other) const;
	const bool operator<(const LogLevel &other) const;

	friend std::ostream &operator<< (std::ostream &stream, LogLevel level);

private:
	int value;
};


inline LogLevel::LogLevel(int value) :
	value(value)
{
}

inline const bool LogLevel::operator==(const LogLevel &other) const
{
	return (this->value == other.value);
}

inline const bool LogLevel::operator!=(const LogLevel &other) const
{
	return (this->value != other.value);
}

inline const bool LogLevel::operator>=(const LogLevel &other) const
{
	return (this->value >= other.value);
}

inline const bool LogLevel::operator<=(const LogLevel &other) const
{
	return (this->value <= other.value);
}

inline const bool LogLevel::operator>(const LogLevel &other) const
{
	return (this->value > other.value);
}

inline const bool LogLevel::operator<(const LogLevel &other) const
{
	return (this->value < other.value);
}

inline std::ostream &operator<<(std::ostream &stream, LogLevel level)
{
	return stream << level.value;
}

} // namespace utl

#endif // UTL_LOGLEVEL_H
