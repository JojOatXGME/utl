#include "utl/consoleloghandler.h"

#include <iostream>
#include <sstream>
#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

#include "utl/logrecord.h"

using std::cerr;
using std::endl;


namespace utl {

ConsoleLogHandler::ConsoleLogHandler() :
	mIsTTY(isatty(fileno(stderr)))
{
}

ConsoleLogHandler::~ConsoleLogHandler()
{
}

void ConsoleLogHandler::publish(const LogRecord &record)
{
	std::string colorLevel, colorLogger, colorEnd;
#if defined(unix) || defined(__unix__) || defined(__unix)
	if (mIsTTY) {
		colorEnd = "\x1b[0m";
		colorLogger = "\x1b[1m";

		if (record.level <= LogLevel::INFO)
			colorLevel = "\x1b[32m";
		else if (record.level <= LogLevel::WARNING)
			colorLevel = "\x1b[33m";
		else
			colorLevel = "\x1b[31m";
	}
#endif

	auto lineEnd = record.message.find_first_of('\n');

	std::stringstream obuf;
	obuf << "[" << colorLevel << record.level << colorEnd << "]"
			"[" << colorLogger << record.loggerName << colorEnd << "] "
		 << record.message.substr(0, lineEnd);

	while (lineEnd != std::string::npos) {
		auto lineStart = lineEnd + 1;
		lineEnd = record.message.find_first_of('\n', lineStart);
		obuf << "\n    " << record.message.substr(lineStart, lineEnd - lineStart);
	}

	cerr << obuf.str() << std::endl;
}

} // namespace utl
