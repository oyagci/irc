#ifndef LOG_H
# define LOG_H

# include <stdio.h>

# define LOGLEVEL	LL_VERYVERBOSE
# define LOG(level, ...) \
	do { \
		if (level <= _loglevel) { \
			_log_print_prefix(); \
			fprintf(stderr, __VA_ARGS__); \
			fprintf(stderr, "\n"); \
		} \
	} \
	while (0)

# define INFO(...) LOG(LL_INFO, __VA_ARGS__)
# define WARNING(...) LOG(LL_WARNING, __VA_ARGS__)
# define ERROR(...) LOG(LL_ERROR, __VA_ARGS__)
# define VERBOSE(...) LOG(LL_VERBOSE, __VA_ARGS__)
# define VVERBOSE(...) LOG(LL_VERYVERBOSE, __VA_ARGS__)

enum _e_log_level {
	LL_SILENT = 0,
	LL_WARNING,
	LL_ERROR,
	LL_INFO,
	LL_VERBOSE,
	LL_VERYVERBOSE
};
extern enum _e_log_level _loglevel;

void _log_print_prefix();

#endif
