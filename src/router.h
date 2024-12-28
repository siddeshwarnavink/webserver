#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request.h"

typedef void (*controller_func)(int, const Request);

typedef struct {
  char *method;
  char *path;
  controller_func controller;
} Route;

void add_route(const char *method, const char *path,
               controller_func controller);
void handle_request(const Request request, int client_socket);

#endif
