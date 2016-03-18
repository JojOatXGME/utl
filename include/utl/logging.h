#ifndef UTL_LOGGING_H
#define UTL_LOGGING_H

#include "utl/log/logger.h"
#include "utl/log/loglevel.h"


#ifndef UTL_LOGGER
#define UTL_LOGGER
#endif

// TODO logger cache per file?

#define utl_log(level, ...) utl::log::Logger::get(UTL_STR_VALUE(UTL_LOGGER)).log(level, __VA_ARGS__)
#define utl_finest(...)  utl_log(utl::log::LogLevel::FINEST,  __VA_ARGS__)
#define utl_finer(...)   utl_log(utl::log::LogLevel::FINER,   __VA_ARGS__)
#define utl_fine(...)    utl_log(utl::log::LogLevel::FINE,    __VA_ARGS__)
#define utl_info(...)    utl_log(utl::log::LogLevel::INFO,    __VA_ARGS__)
#define utl_warning(...) utl_log(utl::log::LogLevel::WARNING, __VA_ARGS__)
#define utl_severe(...)  utl_log(utl::log::LogLevel::SEVERE,  __VA_ARGS__)

namespace utl {

template <typename... A>
static inline void logl(log::LogLevel level , A... a) {
	log::Logger &logger = log::Logger::get(UTL_STR_VALUE(UTL_LOGGER));
	logger.log(level, a...);
}

template <typename... A>
static inline void finest(A... a) {
	logl(log::LogLevel::FINEST, a...);
}

template <typename... A>
static inline void finer(A... a) {
	logl(log::LogLevel::FINER, a...);
}

template <typename... A>
static inline void fine(A... a) {
	logl(log::LogLevel::FINE, a...);
}

template <typename... A>
static inline void info(A... a) {
	logl(log::LogLevel::INFO, a...);
}

template <typename... A>
static inline void warning(A... a) {
	logl(log::LogLevel::WARNING, a...);
}

template <typename... A>
static inline void severe(A... a) {
	logl(log::LogLevel::SEVERE, a...);
}

} // namespace utl

#endif // UTL_LOGGING_H
