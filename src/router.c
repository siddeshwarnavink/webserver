#include <unistd.h>

#include "router.h"
#include "log.h"

#define MAX_ROUTES 100

static Route routes[MAX_ROUTES];
static int routes_count = 0;

void add_route(const char *method, const char *path,
               controller_func controller) {
  if (routes_count >= MAX_ROUTES) {
    fprintf(stderr, "Max routes exceeded\n");
    exit(EXIT_FAILURE);
  }

  routes[routes_count].method = strdup(method);
  routes[routes_count].path = strdup(path);
  routes[routes_count].controller = controller;

  routes_count++;
}

void handle_request(context ctx, const request request, int client_socket) {
  char param_value[256] = {0};

  LOG("request: %s %s %s %s\n", request->method, request->path, request->query, request->body);

  for (int i = 0; i < routes_count; i++) {
    if (strcmp(routes[i].method, request->method) == 0 && match_route(routes[i].path, request->path, param_value)) {
      routes[i].controller(ctx, client_socket, request);
      return;
    }
  }

  const char *response = "HTTP/1.1 404 Not Found\nContent-Type: "
    "text/plain\nContent-Length: 9\n\nNot found";
  write(client_socket, response, strlen(response));
  close(client_socket);
}

static int match_route(const char *route_path, const char *request_path, char *param_value) {
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
