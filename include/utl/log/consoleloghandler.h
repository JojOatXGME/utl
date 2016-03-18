#ifndef UTL_CONSOLELOGHANDLER_H
#define UTL_CONSOLELOGHANDLER_H

#include "utl/log/loghandler.h"
#include "utl/log/logrecord.h"


namespace utl {

class ConsoleLogHandler : public LogHandler
{
public:
	ConsoleLogHandler();
	virtual ~ConsoleLogHandler() noexcept;

	virtual void publish(const LogRecord &record) override;

private:
	bool mIsTTY;

};

} // namespace utl

#endif // UTL_CONSOLELOGHANDLER_H
