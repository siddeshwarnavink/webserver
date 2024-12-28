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

void handle_request(const Request request, int client_socket) {
  for (int i = 0; i < routes_count; i++) {
    if (strcmp(routes[i].method, request->method) == 0 &&
        strcmp(routes[i].path, request->path) == 0) {
			LOG("Matched %s %s\n", routes[i].method, routes[i].path);
      routes[i].controller(client_socket, request);
      return;
    }
  }

  const char *response = "HTTP/1.1 404 Not Found\nContent-Type: "
                         "text/plain\nContent-Length: 9\n\nNot found";
  write(client_socket, response, strlen(response));
  close(client_socket);
}
