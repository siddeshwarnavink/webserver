#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "controller.h"

void ping_controller(int client_socket, const Request request) {
  const char *response = "HTTP/1.1 200 OK\nContent-Type: "
                         "text/plain\nContent-Length: 4\n\nPong";
  write(client_socket, response, strlen(response));
  close(client_socket);
}

void home_controller(int client_socket, const Request request) {
  const char *response = "HTTP/1.1 200 OK\nContent-Type: "
                         "text/plain\nContent-Length: 12\n\nWelcome home!";
  write(client_socket, response, strlen(response));
  close(client_socket);
}

void profile_controller(int client_socket, const Request request) {
  char name[100];
  sscanf(request->path, "/profile/%s", name);

  char response[256];
  snprintf(response, sizeof(response),
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: %zu\r\n"
           "\r\n"
           "Profile Name: %s\nQuery: %s",
           strlen(name) + strlen(request->query) + 18, name, request->query);

  write(client_socket, response, strlen(response));
  close(client_socket);
}
