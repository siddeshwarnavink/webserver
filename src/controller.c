/* vi:set ts=2 sts=2 sw=2 et:
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "controller.h"
#include "mem.h"
#include "template.h"
#include "log.h"
#include "user.h"

void _internal_error(int client_socket) {
  char error_response[] = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
  write(client_socket, error_response, strlen(error_response));
  close(client_socket);
}

void _render_html(int client_socket, const char *html, const char *cookie) {
  size_t response_size = strlen(html) + 256;
  if (cookie) {
    response_size += strlen(cookie) + 30;
  }

  char *response = (char *)mem_alloc(response_size);

  if(!response) {
    LOG("Failed to allocate memory for response\n");
    _internal_error(client_socket);
    return;
  }

  if (cookie) {
    snprintf(response, strlen(html) + 256,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Set-Cookie: %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        cookie,
        strlen(html),
        html);
  } else {
    snprintf(response, strlen(html) + 256,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        strlen(html),
        html);
  }

  write(client_socket, response, strlen(response));
  close(client_socket);
  mem_free(response);
}

void ping_controller(context ctx, int client_socket, const request request) {
  char response[2048];
  size_t size = strlen("Pong\n") + strlen("Method: ") + strlen(request->method) + strlen("\n") +
      strlen("Path: ") + strlen(request->path) + strlen("\n") +
      strlen("Query: ") + strlen(request->query) + strlen("\n") +
      strlen("Body: ") + strlen(request->body) + strlen("\n") +
      strlen("Cookies: ") + strlen(request->cookies) + strlen("\n");

  snprintf(response, sizeof(response),
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: %zu\r\n"
      "\r\n"
      "Pong\n"
      "Method: %s\n"
      "Path: %s\n"
      "Query: %s\n"
      "Body: %s\n"
      "Cookies: %s\n",
      size,
      request->method,
      request->path,
      request->query,
      request->body,
      request->cookies);

  write(client_socket, response, sizeof(response));
  close(client_socket);
}

void home_controller(context ctx, int client_socket, const request request) {
  char *response_html = render_template("home.html", NULL, NULL, 0);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }
  _render_html(client_socket, response_html, NULL);
  mem_free(response_html);
}

void post_register_controller(context ctx, int client_socket, const request request) {
  char *name = get_request_body(request, "name");
  char *email = get_request_body(request, "email");
  char *password = get_request_body(request, "password");

  const char *placeholders[] = {"message", "name", "email", "password"};
  const char *values[4];

  if(!(name && strcmp(name, "") != 0 && email && strcmp(email, "") != 0 && password && strcmp(password, "") != 0)) {
    values[0] = "Please fill in all fields";
    values[1] = name ? name : "";
    values[2] = email ? email : "";
    values[3] = "";
  } else {
    user email_usr = get_user_by_email(ctx.con, email);

    if(email_usr != NULL) {
      values[0] = "Email used";
      mem_free(email_usr);
    } else {
      user u = (user)mem_alloc(sizeof(struct sUser));
      strncpy(u->name, name, 30);
      strncpy(u->email, email, 30);
      strncpy(u->password, password, 30);

      if(insert_user(ctx.con, u)) {
        values[0] = "Account Created!";
      } else {
        values[0] = "Failed!";
      }

      mem_free(u);
    }

    values[1] = name;
    values[2] = email;
    values[3] = "";
  }

  char *response_html = render_template("register.html", placeholders, values, 4);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }
  _render_html(client_socket, response_html, NULL);
  mem_free(response_html);
  if (name) mem_free(name);
  if (email) mem_free(email);
  if (password) mem_free(password);
}

void register_controller(context ctx, int client_socket, const request request) {
  const char *placeholders[] = {"message", "name", "email", "password"};
  const char *values[] = {"", "", "", ""};

  char *response_html = render_template("register.html", placeholders, values, 4);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }
  _render_html(client_socket, response_html, NULL);
  mem_free(response_html);
}

void login_controller(context ctx, int client_socket, const request request) {
  const char *placeholders[] = {"message", "email", "password"};
  const char *values[] = {"", "", ""};

  char *response_html = render_template("login.html", placeholders, values, 3);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }
  _render_html(client_socket, response_html, NULL);
  mem_free(response_html);
}

void post_login_controller(context ctx, int client_socket, const request request) {
  char *email = get_request_body(request, "email");
  char *password = get_request_body(request, "password");
  char cookie[10];

  const char *placeholders[] = {"message", "email", "password"};
  const char *values[3];

  if(!(email && strcmp(email, "") != 0 && password && strcmp(password, "") != 0)) {
    values[0] = "Please fill in all fields";
    values[1] = email ? email : "";
    values[2] = "";
  } else {
    user email_usr = get_user_by_email(ctx.con, email);

    if(email_usr == NULL) {
      values[0] = "Invalid";
    } else if(strcmp(email_usr->password, password) != 0) {
      values[0] = "Invalid";
      mem_free(email_usr);
    } else {
      values[0] = "Success";
      snprintf(cookie, sizeof(cookie), "uid=%d", email_usr->id);
      mem_free(email_usr);
    }

    values[1] = email;
    values[2] = "";
  }

  char *response_html = render_template("login.html", placeholders, values, 4);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }
  _render_html(client_socket, response_html, cookie);
  mem_free(response_html);
  if (email) mem_free(email);
  if (password) mem_free(password);
}


void profile_controller(context ctx, int client_socket, const request request) {
  char name[100];
  sscanf(request->path, "/profile/%s", name);

  const char *placeholders[] = {"name"};
  const char *values[] = {name};

  char *response_html = render_template("profile.html", placeholders, values, 1);
  if (!response_html) {
    _internal_error(client_socket);
    return;
  }

  _render_html(client_socket, response_html, NULL);
  mem_free(response_html);
}
