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

bool Arguments::getNextArgument(string& param)
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
		noOptions = true;
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
