/* vi:set ts=2 sts=2 sw=2 et:
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "request.h"
#include "context.h"

void ping_controller(context ctx, int client_socket, const request request);
void home_controller(context ctx, int client_socket, const request request);
void register_controller(context ctx, int client_socket, const request request);
void post_register_controller(context ctx, int client_socket, const request request);
void login_controller(context ctx, int client_socket, const request request);
void post_login_controller(context ctx, int client_socket, const request request);
void profile_controller(context ctx, int client_socket, const request request);

#endif
