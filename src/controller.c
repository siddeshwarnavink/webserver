#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "controller.h"
#include "template.h"
#include "log.h"

void _internal_error(int client_socket) {
  char error_response[] = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
  write(client_socket, error_response, strlen(error_response));
  close(client_socket);
}

void _render_html(int client_socket, const char *html) {
  char *response = malloc(strlen(html) + 256);
  if(!response) {
    LOG("Failed to allocate memory for response\n");
    _internal_error(client_socket);
    return;
  }

  snprintf(response, strlen(html) + 256,
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "Content-Length: %zu\r\n"
      "\r\n"
      "%s",
      strlen(html),
      html);
  write(client_socket, response, strlen(response));
  close(client_socket);
  free(response);
}

void ping_controller(int client_socket, const Request request) {
  char response[2048];

  snprintf(response, sizeof(response),
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: %zu\r\n"
      "\r\n"
      "Pong\n"
      "Method: %s\n"
      "Path: %s\n"
      "Query: %s\n"
      "Body: %s\n",
      strlen("Pong\n") + strlen("Method: ") + strlen(request->method) + strlen("\n") +
      strlen("Path: ") + strlen(request->path) + strlen("\n") +
      strlen("Query: ") + strlen(request->query) + strlen("\n") +
      strlen("Body: ") + strlen(request->body) + strlen("\n"),
      request->method,
      request->path,
      request->query,
      request->body);

  write(client_socket, response, strlen(response));
  close(client_socket);
}

void home_controller(int client_socket, const Request request) {
  char *response_html = render_template("home.html", NULL, NULL, 0);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }
  _render_html(client_socket, response_html);
  free(response_html);
}

void profile_controller(int client_socket, const Request request) {
  char name[100];
  sscanf(request->path, "/profile/%s", name);

  const char *placeholders[] = {"name"};
  const char *values[] = {name};

  char *response_html = render_template("profile.html", placeholders, values, 1);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }

  _render_html(client_socket, response_html);
  free(response_html);
}
