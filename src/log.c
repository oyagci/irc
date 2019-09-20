#include "log.h"

enum _e_log_level _loglevel = LL_VERYVERBOSE;

void _log_print_prefix() {
	struct { int lvl; char *s; } levels[] = {
		{ LL_SILENT, "" },
		{ LL_INFO, "INFO" },
		{ LL_WARNING, "WARNING" },
		{ LL_ERROR, "ERROR" },
		{ LL_VERBOSE, "LOG" },
		{ LL_VERYVERBOSE, "LOG" },
	};

	if (_loglevel != LL_SILENT) {
		fprintf(stderr, "[%s] ", levels[_loglevel].s);
	}
}
