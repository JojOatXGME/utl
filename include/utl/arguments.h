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

/**
 * @brief A class to parse command line arguments.
 *
 * This class can be used to parse command line arguments. The following example
 * would accept the options `-q`, `-o <param>`, `--quiet`, `--silent` and
 * `--debug` and up to one argument which will be stored in `inputFile`:
 *
 * ```
 * #define ARG_DEBUG 256
 *
 * int main(int argc, char *argv[])
 * {
 *     Arguments args(argc, argv);
 *     args.registerOption("--quiet", 'q');
 *     args.registerOption("--silent", 'q');
 *     args.registerOption("--debug", ARG_DEBUG);
 *
 *     std::string inputFile = "-";
 *     std::string outputFile = "-";
 *     bool debug = false;
 *     bool quiet = false;
 *
 *     while (int opt = args.getNextOption()) {
 *         switch (opt) {
 *         case 'o':
 *             if (!args.getNextArgument(outputFile)) {
 *                 cerr << "Missing argument for " << args.getOptionName()
 *                      << "." << endl;
 *                 return EXIT_FAILURE;
 *             }
 *             break;
 *         case 'q':
 *             quiet = true;
 *             break;
 *         case ARG_DEBUG:
 *             debug = true;
 *             break;
 *         case -2:
 *             cerr << "Ambiguous option: " << args.getOptionName() << endl;
 *             return EXIT_FAILURE;
 *         default:
 *             cerr << "Unknown option: " << args.getOptionName() << endl;
 *             return EXIT_FAILURE;
 *         }
 *     }
 *
 *     if (args.getArgumentsLeft() <= 1) {
 *         cerr << "Invalid amount of arguments." << endl;
 *         return EXIT_FAILURE;
 *     }
 *     args.getNextArgument(inputFile);
 *
 *     // ...
 * }
 * ```
 */
class Arguments
{
public:
	Arguments(int argc, char const * const argv[]);
	Arguments(int argc, char const * const argv[], bool strict);
	virtual ~Arguments() = default;

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
	bool strictRefuse;

	std::size_t idxArg = 1, idxChar = 0;
	std::queue<std::string> params;
	bool noOptions = false;

	std::string currentOption;
	std::map<std::string,int> possibleOptions;
};


/**
 * @brief Creates a new instance of Arguments.
 *
 * @param argc The amount of arguments. You can use the first parameter of your
 *             `main`-function here.
 * @param argv An array of all arguments. This array should contain @em `argc`
 *             c-strings.
 *
 * @see Arguments(int argc, const char *argv[], bool strict)
 */
inline Arguments::Arguments(int argc, char const * const argv[]) :
	Arguments(argc, argv, false)
{
}

/**
 * @brief Creates a new instance of Arguments and specify whether the
 * *strict mode* is enabled.
 *
 * @param argc The amount of arguments. You can use the first parameter of your
 *             ``main``-function here.
 * @param argv An array of all arguments. This array should contain @em `argc`
 *             c-strings.
 * @param strict Whether *strict mode* should be enabled or not.
 *
 * @see Arguments(int argc, const char *argv[])
 * @see getNextOption()
 */
inline Arguments::Arguments(int argc, const char * const argv[], bool strict) :
	argc(argc), argv(argv), strictRefuse(strict)
{
}

/**
 * @brief Registers an option for the parser.
 *
 * The function can be used to register options before using getNextOption().
 * This function have to be used to allow long options like `--debug`, but you
 * can also use it for short options like `-x`.
 *
 * If a long option should be able to accept a parameter, you have to add
 * ``'='`` to the end of the name. If the parameter is optional, you can check the
 * existence with hasParameter(). You can also register two options to handle an
 * optional parameter:
 *
 * ```
 * args.registerOption("--debug",  OPT_DEBUG_ENABLE);
 * args.registerOption("--debug=", OPT_DEBUG_PARAM);
 * ```
 *
 * @param opt The option you want to register. This can be a long option like
 *            `--debug` or a short option like `-x`.
 * @param key The value which should be returned by getNextOption() if the
 *            option is used.
 *
 * @see getNextOption()
 * @see hasParameter()
 */
inline void Arguments::registerOption(const std::string &opt, int key)
{
	assert(key > 0);
	optionMap[opt] = key;
}

/**
 * @brief Returns the amount of remaining arguments.
 * @return The amount of remaining arguments.
 */
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

/**
 * @brief Returns the name of the current option.
 *
 * If the current option could not be determined (getNextOption() returned `-1`
 * or `-2`), the function returns the string which was entered by the user.
 *
 * @return The name of the current option.
 *
 * @see getPossibleOptions()
 */
inline std::string Arguments::getOptionName() const
{
	return currentOption;
}

/**
 * @brief Returns possible options if getNextOption() has returned `-2`.
 *
 * This function can only be used in combination with long options like
 * `--debug`. If there are `--enableA` and `--enableB` and the user enters
 * `--ena`, getNextOption() will return `-2` and you will get `--enableA`
 * and `--enableB` with getPossibleOptions(). The function getOptionName() will
 * return `"--ena"` in this situation.
 *
 * The behavior is undefined if the last call of getNextOption() did not return
 * `-2`.
 *
 * @return Options which could be meant by the user. The first column of the
 *         returned map is the name of the option. The second one is the key
 *         which would be specified with registerOption().
 *
 * @see getNextOption()
 * @see registerOption()
 */
inline const std::map<std::string, int> &Arguments::getPossibleOptions() const
{
	return possibleOptions;
}

/**
 * @brief Checks whether the current option has a parameter.
 *
 * The function can be used to check whether a long option has a parameter
 * appended by ``'='``. For example, it would return `true` for an option like
 * `--mode=fast`. It would return false if the user writes `--mode debug`.
 *
 * @return `true` if the current option has a parameter, `false` otherwise.
 */
inline bool Arguments::hasParameter() const
{
	return idxChar > 0;
}

} // namespace utl

#endif // ARGUMENTS_H
