#ifndef log_h
#define log_h

void _debug_log(const char* file, int line, const char* str);

#define debug_log(_log_str) \
_debug_log(__FILE__, __LINE__, _log_str);

#endif // log_h
