#include "utl/arguments.h"

#include <climits>
#include <string>

using std::string;

static bool isOption(const char *arg);
static string strPlus1(const string& str);


namespace utl {

/**
 * @brief Returns the next option.
 *
 * This function is used to iterate over all options in the argument list. It
 * goes forward in the argument list and returns the key of the next option.
 * This key could be registered by registerOption(). If *strict mode* is
 * disabled, this can also be the value of a character, which was
 * used as an option. You can set *strict mode* in the
 * {@link Arguments(int argc, const char *argv[], bool strict) %constructor}.
 *
 * @return
 *   * The @em key if the option was registered with registerOption().
 *
 *   * The @em value of a character if *strict mode* is disabled and a short
 *     option (like `-x`) is given. The option `-x` would produce `'x'` for
 *     example.
 *
 *   * `0` if there is no option available anymore.
 *
 *   * `-1` if there was no option registered with the given name. For short
 *     options, this can only happens if *strict mode* is enabled.
 *
 *   * If a long option (like `--enable`) is given and there are multiple
 *     possibilities (like `--enableX` and `--enableY`).
 *
 * @see Arguments(int argc, const char *argv[], bool strict)
 * @see registerOption()
 * @see getNextArgument()
 */
int Arguments::getNextOption()
{
	if (noOptions)
		return 0;

	if (idxChar == 0) {
		while (true) {
			if (idxArg == argc) {
				// There are no options anymore
				noOptions = true;
				return 0;
			} else if (argv[idxArg][0] == '-' && argv[idxArg][1] != '\0') {
				// Options found, continue
				break;
			} else {
				// Argument found, save it for later
				params.push(argv[idxArg++]);
			}
		}

		if (argv[idxArg][1] != '-') {
			// The argument is a set of short options (-xyz)
			// We will handle it below
			idxChar = 1;
		} else if (argv[idxArg][2] == '\0') {
			// Argument is '--' -> There are no options anymore
			++idxArg;
			noOptions = true;
			return 0;
		} else {
			// It is a long option (--abc)
			const char *opt = argv[idxArg];
			bool hasParam = false;
			// Get name of the option // TODO and set '=' to '\0' if necessary
			while (opt[idxChar] != '=' && opt[idxChar] != '\0')
				++idxChar;
			if (opt[idxChar] == '=') {
				currentOption.assign(opt, idxChar);
				idxChar++;
				hasParam = true;
				//opt[idxChar++] = '\0'; // TODO
			} else {
				currentOption = opt;
				idxArg++; idxChar = 0;
			}
			// Check whether there is an option with this name
			if (!hasParam) {
				auto it = optionMap.find(currentOption);
				if (it != optionMap.end()) {
					return it->second;
				}
			}
			{
				auto it = optionMap.find(currentOption + "=");
				if (it != optionMap.end()) {
					currentOption.push_back('=');
					return it->second;
				}
			}
			// Fill possibleOptions
			possibleOptions.clear();
			auto it = optionMap.lower_bound(currentOption);
			auto end = optionMap.lower_bound(strPlus1(currentOption));
			for (; it != end; ++it) {
				const string &name = it->first;
				if (!hasParam && name.back() != '=') {
					possibleOptions.insert(*it);
				} else if (name.back() == '=') {
					string base = name.substr(0, name.size() - 1);
					if (optionMap.find(base) == optionMap.end())
						possibleOptions.insert(*it);
				}
			}
			// Check possible options
			switch (possibleOptions.size()) {
			case 0: // No matching option found
				if (hasParam)
					currentOption += '=';
				return -1;
				break;
			case 1: // There is only one option, return it
				it = possibleOptions.begin();
				currentOption = it->first;
				return it->second;
				break;
			default: // Multiple possibilities
				if (hasParam)
					currentOption += '=';
				return -2;
				break;
			}
		}
	}

	// We are in a set of short options (-xyz)
	// Get the next option
	currentOption = {'-', argv[idxArg][idxChar++]};
	if (argv[idxArg][idxChar] == '\0') {
		++idxArg; idxChar = 0;
	}
	// and return it
	auto it = optionMap.find(currentOption);
	if (it == optionMap.end()) {
		return strictRefuse ? -1 : currentOption[1];
	} else {
		return it->second;
	}
}

/**
 * @brief Gets the next argument from the argument list.
 *
 * This function is used to iterate over all arguments in the argument list. It
 * can be used to get a parameter of an option or to iterate over all
 * arguments. The function getNextOption() may skip arguments since they may not
 * be an option. getNextArgument() will not observe this arguments
 * until getNextOption() has returned `0` at least one time.
 *
 * @param param The function will write the argument to this parameter.
 * @return `false` if there is no argument left, `true` otherwise.
 *
 * @see getNextArgument(T&,R)
 *         Can be used with an {@link argr argument reader}.
 * @see getNextOption()
 *         If you want to get the next option instead.
 */
bool Arguments::getNextArgument(std::string& param)
{
	if (noOptions && !params.empty()) {
		param = params.front();
		params.pop();
		return true;
	}

	if (idxArg < argc) {
		param = &argv[idxArg][idxChar];
		idxArg++; idxChar = 0;
		return true;
	} else {
		return false;
	}
}

} // namespace utl


string strPlus1(const string& str)
{
	string tmp = str;
	for (int i = tmp.size() - 1; i >= 0; --i) {
		if (tmp[i] < CHAR_MAX) {
			++tmp[i];
			break;
		}
	}
	return tmp;
}
