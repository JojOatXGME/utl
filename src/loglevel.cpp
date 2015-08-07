#include "utl/loglevel.h"


namespace utl {

const LogLevel LogLevel::ALL     (INT_MIN);
const LogLevel LogLevel::FINEST  (    300);
const LogLevel LogLevel::FINER   (    400);
const LogLevel LogLevel::FINE    (    500);
const LogLevel LogLevel::CONFIG  (    700);
const LogLevel LogLevel::INFO    (    800);
const LogLevel LogLevel::WARNING (    900);
const LogLevel LogLevel::SEVERE  (   1000);
const LogLevel LogLevel::OFF     (INT_MAX);

} // namespace utl
