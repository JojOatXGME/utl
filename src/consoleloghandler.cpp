#include "utl/consoleloghandler.h"

#include <iostream>

#include "utl/logrecord.h"

using std::cerr;
using std::endl;


namespace utl {

ConsoleLogHandler::ConsoleLogHandler()
{
}

void ConsoleLogHandler::publish(const LogRecord &record)
{
	if (record.level < this->getLevel())
		return;

	cerr << "[" << record.loggerName << "][" << record.level << "] " << record.message << endl;
}

} // namespace utl
