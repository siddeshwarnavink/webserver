/* vi:set ts=2 sts=2 sw=2 et:
 *
 * log.h - Log debug messages
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef LOG_H
#define LOG_H

#ifdef DEBUG
#define LOG(fmt, ...) log_message(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

void log_message(const char *fmt, ...);

#endif
