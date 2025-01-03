/* vi:set ts=2 sts=2 sw=2 et:
 *
 * log.c - Log debug messages
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "log.h"

/*
 * Log debug message to stdout
 */
void log_message(const char *fmt, ...) {
  time_t rawtime;
  struct tm *timeinfo;
  char time_buffer[20];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

  printf("[%s] ", time_buffer);

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
