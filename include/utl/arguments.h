#ifndef ARGUMENTS_H
#define ARGUMENTS_H

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
	Arguments(int argc, char *argv[]);

	int getNextOption();
	bool getNextArgument(std::string& param);
	void registerOption(const std::string& opt, int key);

	std::string getOptionName() const;
	const std::map<std::string,int>& getPossibleOptions() const;
	int getArgumentsLeft() const;

	template<typename T>
	int getNextArgument(T &param);

private:
	const int argc;
	char * const * const argv;
	std::size_t idxArg = 1, idxChar = 0;
	std::string currentOption;
	std::map<std::string,int> possibleOptions;
	std::map<std::string,int> optionMap;
	std::queue<std::string> params;
	bool strictRefuse = false;
	bool noOptions = false;
};


inline Arguments::Arguments(int argc, char *argv[]) :
	argc{argc}, argv{argv}
{
}

inline void Arguments::registerOption(const std::string &opt, int key)
{
	optionMap[opt] = key;
}

inline std::string Arguments::getOptionName() const
{
	return currentOption;
}

inline const std::map<std::string, int> &Arguments::getPossibleOptions() const
{
	return possibleOptions;
}

inline int Arguments::getArgumentsLeft() const
{
	return (argc - idxArg) + params.size();
}

template<typename T>
inline int Arguments::getNextArgument(T &param)
{
	// TODO throw exception for invalid format?
	std::string str;
	if (!getNextArgument(str))
		return 0;
	std::istringstream stream{str};

	stream >> param;

	if (stream.fail())
		return -1;
	if (stream.peek() != EOF)
		return -1;

	return 1;
}

} // namespace utl

#endif // ARGUMENTS_H
