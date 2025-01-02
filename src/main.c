#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <mysql/mysql.h>

#include "context.h"
#include "mem.h"
#include "controller.h"
#include "log.h"
#include "request.h"
#include "router.h"
#include "db.h"

#define PORT 3300
#define BUFFER_SIZE 1024

context ctx;

void _clenup() {
  mem_free_all();
  close_db(ctx.con);
}

void handle_sigint(int sig) {
  printf("Caught signal %d, cleaning up...\n", sig);
  _clenup();
  exit(0);
}

void print_banner() {
  printf("                __                                  \n");
  printf(" _      _____  / /_  ________  ______   _____  _____\n");
  printf("| | /| / / _ \\/ __ \\/ ___/ _ \\/ ___/ | / / _ \\/ ___/\n");
  printf("| |/ |/ /  __/ /_/ (__  )  __/ /   | |/ /  __/ /    \n");
  printf("|__/|__/\\___/_.___/____/\\___/_/    |___/\\___/_/     \n");
  printf("                                                    \n");
}

/*
 * Parse the request and populate the request struct
 */
void parse_request(const char *buf, request req) {
  sscanf(buf, "%s %s", req->method, req->path);

  char *query_start = strchr(req->path, '?');
  if (query_start) {
    LOG("Parsing query string\n");

    req->query = query_start + 1;
    *query_start = '\0';
  } else {
    req->query = strdup("");
    req->query_allocated = 1;
  }

  if (strcmp(req->method, "POST") == 0) {
    LOG("Parsing POST request\n");

    char *body_start = strstr(buf, "\r\n\r\n");
    if (body_start) {
      req->body = body_start + 4;
    } else {
      req->body = strdup("");
      req->body_allocated = 1;
    }
  } else {
    req->body = strdup("");
    req->body_allocated = 1;
  }
}

/*
 * Handle the client request
 */
void handle_client(int client_socket) {
  char buffer[BUFFER_SIZE] = {0};

  int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
  if (bytes_read < 0) {
    perror("Failed to read from socket");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  buffer[bytes_read] = '\0';

  request req = init_request();

  parse_request(buffer, req);
  LOG("parse_request done\n");

  handle_request(ctx, req, client_socket);
  LOG("handle_request done\n");

	free_request(&req);
  LOG("free_request done\n");
}

int main() {
  int server_fd, client_socket;
  struct sockaddr_in server_address, client_address;
  int opt = 1;
  int addrlen = sizeof(server_address);

  signal(SIGINT, handle_sigint);

  init_db(&ctx.con);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    _clenup();
    exit(EXIT_FAILURE);
  }

  LOG("Socket created\n");

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
        sizeof(opt))) {
    perror("Failed to set socket options");
    _clenup();
    exit(EXIT_FAILURE);
  }

  LOG("Socket options set\n");

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&server_address,
        sizeof(server_address)) < 0) {
    perror("Failed to bind");
    _clenup();
    exit(EXIT_FAILURE);
  }

  LOG("Socket bound\n");

  if (listen(server_fd, 3) < 0) {
    perror("Failed to listen");
    _clenup();
    exit(EXIT_FAILURE);
  }

  print_banner();

  printf("Server listening on port %d\n", PORT);

  // Register routes
  add_route("POST", "/ping", ping_controller);
  add_route("GET", "/", home_controller);
  add_route("GET", "/register", register_controller);
  add_route("POST", "/register", post_register_controller);
  add_route("GET", "/login", login_controller);
  add_route("POST", "/login", post_login_controller);
  add_route("GET", "/profile/:name", profile_controller);

  LOG("Routes registered\n");

  while (1) {
    if ((client_socket = accept(server_fd, (struct sockaddr *)&client_address,
            (socklen_t *)&addrlen)) < 0) {
      perror("Failed to accept");
      _clenup();
      exit(EXIT_FAILURE);
    }
    handle_client(client_socket);
  }
}
