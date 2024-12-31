#ifndef DB_H
#define DB_H

#include <mysql/mysql.h>

void init_db(MYSQL **con);
void close_db(MYSQL *con);

#endif

