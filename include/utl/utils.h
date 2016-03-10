#ifndef UTL_UTILS_H
#define UTL_UTILS_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>


namespace utl {

template <typename... A>
std::string format(const std::string format, A... args);


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
