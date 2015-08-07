#ifndef UTL_CONSOLELOGHANDLER_H
#define UTL_CONSOLELOGHANDLER_H

#include "utl/loghandler.h"
#include "utl/logrecord.h"


namespace utl {

class ConsoleLogHandler : public LogHandler
{
public:
	ConsoleLogHandler();

	virtual void publish(const LogRecord &record) override;
};

} // namespace utl

#endif // UTL_CONSOLELOGHANDLER_H
