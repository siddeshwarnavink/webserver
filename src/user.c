/* vi:set ts=2 sts=2 sw=2 et:
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#include "user.h"
#include "mem.h"
#include "log.h"

int insert_user(MYSQL *con, user u) {
  char query[1024];
  snprintf(query, sizeof(query), "INSERT INTO users(name, email, password) VALUES('%s', '%s', '%s')", u->name, u->email, u->password);

  if(mysql_query(con, query)) {
    fprintf(stderr, "%s\n", mysql_error(con));
    LOG("%s\n", mysql_error(con));
    return 0;
  }

  LOG("Created user\n");

  return 1;
}

user get_user_by_email(MYSQL *con, const char *email) {
  char query[1024];
  snprintf(query, sizeof(query), "SELECT id, name, email, password FROM users WHERE email='%s'", email);

  if(mysql_query(con, query)) {
    LOG("%s\n", mysql_error(con));
    return NULL;
  }


  MYSQL_RES *res = mysql_store_result(con);
  if(res == NULL) {
    LOG("Failed to retrieve results\n");
    return NULL;
  }

  MYSQL_ROW row = mysql_fetch_row(res);
  if(row == NULL) {
    mysql_free_result(res);
    LOG("No user found with email %s\n", email);
    return NULL;
  }

  user u = mem_alloc(sizeof(struct sUser));
  u->id = atoi(row[0]);
  strncpy(u->name, row[1], 30);
  strncpy(u->email, row[2], 30);
  strncpy(u->password, row[3], 30);

  mysql_free_result(res);

  return u;
}
