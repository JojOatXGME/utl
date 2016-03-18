#include "utl/log/logger.h"


namespace utl {
namespace log {

Logger Logger::root (nullptr);
std::shared_ptr<Logger> Logger::rootSharedPtr (&Logger::root, [](Logger*){});
std::unordered_map<std::string, std::shared_ptr<Logger>> Logger::globalLoggers;
std::mutex Logger::staticMutex;

} // namespace log
} // namespace utl
