/* vi:set ts=2 sts=2 sw=2 et:
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef USER_H
#define USER_H

#include <mysql/mysql.h>

typedef struct sUser {
  char name[30];
  char email[30];
  char password[30];
} *user;

int insert_user(MYSQL *con, user u);
user get_user_by_email(MYSQL *con, const char *email);

#endif

