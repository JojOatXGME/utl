#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <cassert>
#include <cstdint>
#include <map>
#include <utility>
#include <queue>
#include <regex>
#include <sstream>
#include <string>


namespace utl {

class Arguments
{
public:
	Arguments(int argc, char const * const argv[]);

	void registerOption(const std::string& opt, int key);

	int getNextOption();
	bool getNextArgument(std::string& param);
	int getArgumentsLeft() const;

	template<typename T>
	bool getNextArgument(T &param);

	std::string getOptionName() const;
	const std::map<std::string,int>& getPossibleOptions() const;
	bool hasParameter() const;

private:
	const int argc;
	char const * const * const argv;
	std::map<std::string,int> optionMap;
	bool strictRefuse = false;

	std::size_t idxArg = 1, idxChar = 0;
	std::queue<std::string> params;
	bool noOptions = false;

	std::string currentOption;
	std::map<std::string,int> possibleOptions;
};


inline Arguments::Arguments(int argc, char const * const argv[]) :
	argc{argc}, argv{argv}
{
}

inline void Arguments::registerOption(const std::string &opt, int key)
{
	assert(key > 0);
	optionMap[opt] = key;
}

inline int Arguments::getArgumentsLeft() const
{
	return (argc - idxArg) + params.size();
}

template<typename T>
inline bool Arguments::getNextArgument(T &param)
{
	std::string str;
	if (!getNextArgument(str))
		return false;
	std::istringstream stream(str);

	stream >> param;

	if (stream.fail())
		throw std::exception(); // TODO use another exception type
	if (stream.peek() != EOF)
		throw std::exception(); // TODO use another exception type

	return true;
}

inline std::string Arguments::getOptionName() const
{
	return currentOption;
}

inline const std::map<std::string, int> &Arguments::getPossibleOptions() const
{
	return possibleOptions;
}

inline bool Arguments::hasParameter() const
{
	return idxChar > 0;
}

} // namespace utl

#endif // ARGUMENTS_H
