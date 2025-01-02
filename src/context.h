#ifndef CONTEXT_H
#define CONTEXT_H

#include <mysql/mysql.h>

typedef struct {
  MYSQL *con;
} context;

#endif
