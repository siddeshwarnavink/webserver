/* vi:set ts=2 sts=2 sw=2 et:
 *
 * db.c: Database wrapper for relational database.
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */
#include <stdio.h>
#include <mysql/mysql.h>

#include "db.h"
#include "log.h"

void init_db(MYSQL **con) {
  *con = mysql_init(NULL);
  if (*con == NULL) {
    LOG("mysql_init() failed\n");
    return;
  }

  if (mysql_real_connect(*con, "127.0.0.1", "user", "password", "webserver", 0, NULL, 0) == NULL) {
    fprintf(stderr, "%s\n", mysql_error(*con));
    mysql_close(*con);
    *con = NULL;
    return;
  }

  LOG("MySql connection established\n");
}

void close_db(MYSQL *con) {
  LOG("MySql connection closed\n");
  mysql_close(con);
}
