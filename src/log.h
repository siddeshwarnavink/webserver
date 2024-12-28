#ifndef LOG_H
#define LOG_H

#ifdef DEBUG
#define LOG(fmt, ...) log_message(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

void log_message(const char *fmt, ...);

#endif
