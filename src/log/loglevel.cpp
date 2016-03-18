#include "utl/log/loglevel.h"


namespace utl {
namespace log {

const LogLevel LogLevel::ALL     (INT_MIN);
const LogLevel LogLevel::FINEST  (    300, "FINEST" );
const LogLevel LogLevel::FINER   (    400, "FINER"  );
const LogLevel LogLevel::FINE    (    500, "FINE"   );
const LogLevel LogLevel::CONFIG  (    700, "CONFIG" );
const LogLevel LogLevel::INFO    (    800, "INFO"   );
const LogLevel LogLevel::WARNING (    900, "WARNING");
const LogLevel LogLevel::SEVERE  (   1000, "SEVERE" );
const LogLevel LogLevel::OFF     (INT_MAX);

} // namespace log
} // namespace utl
