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

/*
 * Handle an incoming request
 * @param request The request to handle
 * @param client_socket The client socket to write the response to
 */
void add_route(const char *method, const char *path,
               controller_func controller);

/*
 * Register a route with the router
 * @param method The HTTP method
 * @param path The path to match
 * @param controller The controller function to call
 */

void handle_request(const Request request, int client_socket);

/*
 * Match a route to a request
 * @param route_path The path of the route
 * @param request_path The path of the request
 * @param param_value The value of the parameter
 * @return 1 if the route matches, 0 otherwise
 */
static int match_route(const char *route_path, const char *request_path, char *param_value);

#endif
