#ifndef UTL_ARGUMENTS_H
#define UTL_ARGUMENTS_H

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <locale>
#include <map>
#include <utility>
#include <queue>
#include <regex>
#include <sstream>
#include <string>


namespace utl {

/**
 * @brief Argument readers for Arguments::getNextArgument(T&,R).
 *
 * *Argument readers* are classes (or functions) which can be used as second
 * parameter of Arguments::getNextArgument(T&,R). They decide the way how an
 * argument is parsed. All argument readers which are distributed with the
 * library are inside this namespace.
 *
 * The only function they need to implement is
 *
 * ```
 * bool operator() (const std::string &str, T &param);
 * ```
 *
 * The first parameter is the argument as string. The second parameter is the
 * variable where the result should be stored. The function have to return
 * `true` on success. If the argument is not valid, it has to return `false`.
 *
 * Here is a simple example of an argument reader which accepts `true` and
 * `false` only:
 *
 * ```{.cpp}
 * struct boolean {
 *     template<typename T>
 *     bool operator() (const std::string &str, T &param) {
 *         if (str == "true") {
 *             param = true;
 *             return true;
 *         } else if (str == "false") {
 *             param = false;
 *             return true;
 *         } else {
 *             return false;
 *         }
 *     }
 * }
 * ```
 *
 * @see Arguments::getNextArgument(T&,R)
 *         The function where you can use stream readers.
 * @see argr::fromStream
 *         The default stream reader.
 * @see argr::withUnit
 *         If you want to read a value while the user can use different units.
 */
namespace argr {

/**
 * @brief The default argument reader.
 *
 * It uses the stream operator `>>` to read the parameter. This mean it can read
 * any type which has implemented this operator. It works for basic types like
 * int and double, but you may also use custom types. Here is a simple example
 * how to read a double value with this reader:
 *
 * ```{.cpp}
 * double x;
 * if (!args.getNextArgument(x)) {
 *     // handle missing argument
 * }
 * ```
 */
struct fromStream {
	template<typename T>
	bool operator() (const std::string &str, T &param) {
		std::istringstream stream(str);
		stream >> param;

		if (stream.fail() || stream.peek() != EOF)
			return false;
		else
			return true;
	}
};

/**
 * @brief Allows the use of units.
 *
 * If you want that a user can add `k`, `M` or `G` as suffix to apply a factor,
 * you can use this argument reader. Here is a example how you could achieve it:
 *
 * ```{.cpp}
 * utl::argr::withUnit<long> myUnits{
 *         {"",   1},
 *         {"k",  1000},
 *         {"M",  1000 * 1000},
 *         {"G",  1000 * 1000 * 1000}
 * }
 *
 * long x;
 * if (!args.getNextArgument(x, myUnits)) {
 *     // handle missing argument
 * }
 * ```
 *
 * @note
 *     The user can not eneter any number without a unit. If you want to accept
 *     pure numbers, you have to add an empty unit (``""``). Just like in the
 *     example. The user can always enter `0`, even if there is no empty unit.
 */
template<typename F>
class withUnit {
public:
	withUnit(std::initializer_list<std::pair<const std::string,F>> il) :
		unitMap(il)
	{}

	template<typename T>
	bool operator() (const std::string &str, T &param) {
		std::istringstream stream(str);
		stream >> param;

		std::string unit = static_cast<std::stringstream&>(
					std::stringstream() << stream.rdbuf()).str();
		auto it = unitMap.find(unit);

		if (stream.fail()) {
			return false;
		} else if (param == 0 && stream.peek() == EOF) {
			return true;
		} else if (it != unitMap.end()) {
			param *= it->second;
			return true;
		} else {
			return false;
		}
	}
private:
	std::map<std::string,F> unitMap;
};

/**
 * @brief Reads a boolean value.
 *
 * The reader accepts `0`, `1`, `true`, `false`, `on` and `off`. It also accepts
 * upper case versions of this strings.
 *
 * ```
 * bool x;
 * args.getNextArgument(x, utl::argr::boolean());
 * ```
 */
struct boolean {
	template<typename T>
	bool operator() (const std::string &str, T &param) {
		std::string lc = str;
		std::transform(str.begin(), str.end(), lc.begin(), [](char c) {
			return std::tolower(c, std::locale::classic());
		});

		if (lc == "1" | lc == "true" | lc == "on") {
			param = true;
			return true;
		} else if (lc == "0" | lc == "false" | lc == "off") {
			param = false;
			return true;
		} else {
			return false;
		}
	}
};

/**
 * @brief Helper class for utl::argr::list.
 *
 * @see list
 */
template<typename R = fromStream>
class list_helper {
	template<typename C, typename V>
	static void addTo(C &container, const V &value, decltype(container.insert(value))* = nullptr) {
		container.insert(value);
	}
	template<typename C, typename V>
	static void addTo(C &container, const V &value) {
		container.push_back(value);
	}
public:
	list_helper(const R &reader = R(), char delimiter = ',') :
		reader(reader), delimiter(delimiter)
	{}
	template<typename T>
	bool operator() (const std::string &str, T &param) {
		if (str.empty())
			return true;
		std::size_t lastDeli = 0;
		do {
			std::size_t nextDeli = str.find_first_of(delimiter, lastDeli);
			typename T::value_type val;
			if (!reader(str.substr(lastDeli, nextDeli - lastDeli), val))
				return false;
			addTo(param, val);
			lastDeli = nextDeli;
		} while (lastDeli++ != std::string::npos);
		return true;
	}
private:
	R reader;
	char delimiter;
};

/**
 * @brief Reads a list.
 *
 * Here is a simple example which can read a comma seperated list of integers:
 *
 * ```
 * std::vector<int> x;
 * args.getNextArgument(x, utl::argr::list());
 * ```
 *
 * But you can also specify another delimiter and reader:
 *
 * ```
 * std::vector<bool> x;
 * args.getNextArgument(x, utl::argr::list(boolean(), ':'));
 * ```
 */
template<typename R = fromStream>
list_helper<R> list(const R &reader = R(), char delimiter = ',') {
	return list_helper<R>(reader, delimiter);
}

} // namespace argr

/**
 * @brief A class to parse command line arguments.
 *
 * This class can be used to parse command line arguments. The following example
 * would accept the options `-q`, `-o <param>`, `--quiet`, `--silent` and
 * `--debug` and up to one argument which will be stored in `inputFile`:
 *
 * ```
 * #define OPT_DEBUG 256
 *
 * int main(int argc, char *argv[])
 * {
 *     Arguments args(argc, argv);
 *     args.registerOption("--quiet", 'q');
 *     args.registerOption("--silent", 'q');
 *     args.registerOption("--debug", OPT_DEBUG);
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
 *         case OPT_DEBUG:
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
 *     if (args.getArgumentsLeft() > 1) {
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

	template<typename T, typename R = argr::fromStream>
	bool getNextArgument(T &param, R reader = R());

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

/**
 * @brief Gets the next argument while using an argument reader.
 *
 * This function is an extension of getNextArgument(std::string&). It uses an
 * {@link argr argument reader} which can be specified as second parameter.
 * The default argument reader is argr::fromStream. If the argument is not valid
 * (corresponding to the argument reader), the function will throw an exception.
 *
 * @param param The function will write the argument to this parameter.
 * @param reader The argument reader which should be used.
 * @return `false` if there is no argument left, `true` otherwise.
 * @throws std::exception If the argument is not valid.
 *
 * @see getNextArgument(std::string&)
 *         Is used by this function.
 * @see argr
 *         To get a list of argument readers distributed by the library.
 */
template<typename T, typename R>
inline bool Arguments::getNextArgument(T &param, R reader)
{
	std::string str;
	if (!getNextArgument(str))
		return false;

	if (reader(str, param))
		return true;
	else
		throw std::exception(); // TODO use another exception type
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

#endif // UTL_ARGUMENTS_H
