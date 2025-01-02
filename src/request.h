#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
  char *method;
  char *path;
  char *query;
  char *body;
  int query_allocated;
  int body_allocated;
} *request;

request init_request();

void free_request(request *req);

/*
 * Get the value of form field from a request
 * @param request The request to parse
 * @param client_socket The client socket to write the response to
 */
char *get_request_body(request req, const char *field);

#endif
