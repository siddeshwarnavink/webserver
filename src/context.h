/* vi:set ts=2 sts=2 sw=2 et:
 *
 * context.h - Context structure for webserver
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <mysql/mysql.h>

typedef struct {
  MYSQL *con;
} context;

#endif
