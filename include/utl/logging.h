#ifndef UTL_LOGGING_H
#define UTL_LOGGING_H

#include "utl/log/logger.h"
#include "utl/log/loglevel.h"


#ifndef STR
#define STR(arg) #arg
#endif

#ifndef STR_VALUE
#define STR_VALUE(name) STR(name)
#endif

#ifndef UTL_LOGGER
#define UTL_LOGGER
#endif

// TODO logger cache per file?

#define utl_log(level, ...) utl::Logger::get(STR_VALUE(UTL_LOGGER)).log(level, __VA_ARGS__)
#define utl_finest(...)  utl_log(utl::LogLevel::FINEST,  __VA_ARGS__)
#define utl_finer(...)   utl_log(utl::LogLevel::FINER,   __VA_ARGS__)
#define utl_fine(...)    utl_log(utl::LogLevel::FINE,    __VA_ARGS__)
#define utl_info(...)    utl_log(utl::LogLevel::INFO,    __VA_ARGS__)
#define utl_warning(...) utl_log(utl::LogLevel::WARNING, __VA_ARGS__)
#define utl_severe(...)  utl_log(utl::LogLevel::SEVERE,  __VA_ARGS__)

namespace utl {

template <typename... A>
static inline void log(LogLevel level , A... a) {
	Logger &logger = Logger::get(STR_VALUE(UTL_LOGGER));
	logger.log(level, a...);
}

template <typename... A>
static inline void finest(A... a) {
	log(LogLevel::FINEST, a...);
}

template <typename... A>
static inline void finer(A... a) {
	log(LogLevel::FINER, a...);
}

template <typename... A>
static inline void fine(A... a) {
	log(LogLevel::FINE, a...);
}

template <typename... A>
static inline void info(A... a) {
	log(LogLevel::INFO, a...);
}

template <typename... A>
static inline void warning(A... a) {
	log(LogLevel::WARNING, a...);
}

template <typename... A>
static inline void severe(A... a) {
	log(LogLevel::SEVERE, a...);
}

} // namespace utl

#endif // UTL_LOGGING_H
