#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "request.h"

void ping_controller(int client_socket, const Request request);
void home_controller(int client_socket, const Request request);
void register_controller(int client_socket, const Request request);
void post_register_controller(int client_socket, const Request request);
void login_controller(int client_socket, const Request request);
void post_login_controller(int client_socket, const Request request);
void profile_controller(int client_socket, const Request request);

#endif
