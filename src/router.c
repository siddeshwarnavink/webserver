/* vi:set ts=2 sts=2 sw=2 et:
 *
 * router.c: Handle routing of requests.
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#include <unistd.h>

#include "router.h"
#include "log.h"
#include "mem.h"

#define MAX_ROUTES 100

static Route routes[MAX_ROUTES];
static int routes_count = 0;

char* _strdup(const char* s) {
  size_t len = strlen(s) + 1;
  char* copy = (char*)mem_alloc(len);
  if (copy != NULL) {
    memcpy(copy, s, len);
  }
  return copy;
}

/*
 * Match a route to a request
 * @param route_path The path of the route
 * @param request_path The path of the request
 * @param param_value The value of the parameter
 * @return 1 if the route matches, 0 otherwise
 */
static int _match_route(const char *route_path, const char *request_path, char *param_value) {
  const char *route_ptr = route_path;
  const char *request_ptr = request_path;

  while (*route_ptr && *request_ptr) {
    if (*route_ptr == ':') {
      route_ptr++;
      while (*request_ptr && *request_ptr != '/') {
        *param_value++ = *request_ptr++;
      }
      *param_value = '\0';
      return 1;
    } else if (*route_ptr != *request_ptr) {
      return 0;
    }

    route_ptr++;
    request_ptr++;
  }

  return *route_ptr == *request_ptr;
}

void add_route(const char *method, const char *path,
               controller_func controller) {
  if (routes_count >= MAX_ROUTES) {
    fprintf(stderr, "Max routes exceeded\n");
    exit(EXIT_FAILURE);
  }

  routes[routes_count].method = _strdup(method);
  routes[routes_count].path = _strdup(path);
  routes[routes_count].controller = controller;

  routes_count++;
}

void handle_request(context ctx, const request request, int client_socket) {
  char param_value[256] = {0};

  LOG("request: %s %s %s %s\n", request->method, request->path, request->query, request->body);

  for (int i = 0; i < routes_count; i++) {
    if (strcmp(routes[i].method, request->method) == 0 && _match_route(routes[i].path, request->path, param_value)) {
      routes[i].controller(ctx, client_socket, request);
      return;
    }
  }

  const char *response = "HTTP/1.1 404 Not Found\nContent-Type: "
    "text/plain\nContent-Length: 9\n\nNot found";
  write(client_socket, response, strlen(response));
  close(client_socket);
}
