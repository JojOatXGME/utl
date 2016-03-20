#ifndef UTL_UTILS_H
#define UTL_UTILS_H

/**
 * @file  utils.h
 * @brief This file provides some common functions and macros.
 */

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>


//! Gets arg as sting literal.
#define UTL_STR(arg) #arg
//! Like #UTL_STR, but replaces every macro with it's value first.
#define UTL_STR_VALUE(arg) UTL_STR(arg)

namespace utl {

template <typename... A>
std::string format(const std::string format, A... args);


/**
 * @brief Returns a formatted string based on the given format and arguments.
 *
 * This function uses `snprintf()`, but it may use a typesafe implementation in
 * the future.
 *
 * @param format A format string which specifies the format.
 * @param args   The arguments to be used in the string.
 * @return The formated string.
 */
template <typename... A>
inline std::string format(const std::string format, A... args)
{
	std::size_t final_n = 2 * format.size(), n = 0;
	std::unique_ptr<char[]> formatted;
	do {
		n += std::llabs(final_n - n + 1);
		formatted.reset(new char[n]);
		final_n = std::snprintf(formatted.get(), n, format.c_str(), args...);
		// TODO catch negative final_n and throw exception?
	} while (final_n < 0 || final_n >= n);
	return std::string(formatted.get());
}

} // namespace utl

#endif // UTL_UTILS_H
