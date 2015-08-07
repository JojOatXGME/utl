#include "utl/arguments.h"

#include <climits>
#include <string>

using std::string;

static bool isOption(const char *arg);
static string strPlus1(const string& str);


namespace utl {

int Arguments::getNextOption()
{
	if (noOptions)
		return 0;

	if (idxChar != 0 && argv[idxArg][idxChar] == '\0') {
		++idxArg;
		idxChar = 0;
	}

	if (idxChar == 0) {
		for (; idxArg < argc && !isOption(argv[idxArg]); ++idxArg) {
			params.push(argv[idxArg]);
		}

		if (idxArg < argc) {
			if (argv[idxArg][1] == '-') {
				if (argv[idxArg][2] == '\0') {
					// It is a '--' -> there are no options anymore
					++idxArg;
					noOptions = true;
					return 0;
				} else {
					// It is a long option (--abc)
					char *opt = argv[idxArg];
					// get name of option and set '=' to '\0' if necessary
					while (opt[idxChar] != '=' && opt[idxChar] != '\0')
						++idxChar;
					if (opt[idxChar] == '=') {
						currentOption.assign(opt, idxChar+1);
						opt[idxChar++] = '\0';
					} else {
						currentOption = opt;
					}
					// check if there is an option with this name
					auto it = optionMap.find(currentOption);
					if (it != optionMap.end()) {
						return it->second;
					}
					it = optionMap.find(currentOption + "=");
					if (it != optionMap.end()) {
						currentOption += "=";
						return it->second;
					}
					// fill possibleOptions
					possibleOptions.clear();
					for (it = optionMap.lower_bound(currentOption);
							it != optionMap.lower_bound(strPlus1(currentOption));
							++it) {
						if (it->first.back() != '=' && opt[idxChar] == '\0') {
							possibleOptions.insert(*it);
						} else if (it->first.back() == '=') {
							string without = it->first.substr(0, it->first.size());
							if (optionMap.find(without) == optionMap.end())
								possibleOptions.insert(*it);
						}
					}
					// check possible options
					switch (possibleOptions.size()) {
					case 0: // no matching option found
						return -1;
						break;
					case 1: // there is only one option, return it
						currentOption = possibleOptions.begin()->first;
						return possibleOptions.begin()->second;
						break;
					default:
						return -2; // multiple possibilities
						break;
					}
				}
			} else {
				// Its is a normal argument with one or more options
				idxChar = 1;
			}
		} else { // idxArg >= argc
			// There is no argument anymore
			noOptions = true;
			return 0;
		}
	} // end idxChar == 0

	// We are already in a set of options (-xyz...)
	currentOption = {'-', argv[idxArg][idxChar], '\0'};
	auto it = optionMap.find(currentOption);
	++idxChar;
	if (it == optionMap.end()) {
		return strictRefuse ? -1 : currentOption[1];
	} else {
		return it->second;
	}
}

bool Arguments::getNextArgument(string& param)
{
	if (noOptions && !params.empty()) {
		param = params.front();
		params.pop();
		return true;
	}

	if (idxChar != 0 && argv[idxArg][idxChar] == '\0'
			&& argv[idxArg][idxChar - 1] != '\0') {
		++idxArg;
		idxChar = 0;
	}

	if (idxArg < argc) {
		param = &argv[idxArg++][idxChar];
		idxChar = 0;
		return true;
	} else {
		noOptions = true;
		return false;
	}
}

} // namespace utl


bool isOption(const char *arg)
{
	return arg[0] == '-' && arg[1] != '\0';
}

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
