/* vi:set ts=2 sts=2 sw=2 et:
 *
 * db.h: Database wrapper for relational database.
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef DB_H
#define DB_H

#include <mysql/mysql.h>

void init_db(MYSQL **con);
void close_db(MYSQL *con);

#endif

