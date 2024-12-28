#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
  char *method;
  char *path;
  char *query;
  char *body;
} *Request;

Request init_request();

void free_request(Request req);

#endif
