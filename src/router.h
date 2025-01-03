/* vi:set ts=2 sts=2 sw=2 et:
 *
 * router.h: Handle routing of requests.
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request.h"
#include "context.h"

typedef void (*controller_func)(context, int, const request);

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
 * @param ctx App context
 * @param method The HTTP method
 * @param path The path to match
 * @param controller The controller function to call
 */
void handle_request(context ctx, const request request, int client_socket);

/*
 * Free memory used by routes
 */

#endif
