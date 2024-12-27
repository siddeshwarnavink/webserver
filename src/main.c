#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3300
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
  char buffer[BUFFER_SIZE] = {0};

  int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
  if (bytes_read < 0) {
    perror("Failed to read from socket");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  buffer[bytes_read] = '\0';

  const char *response = "HTTP/1.1 200 OK\nContent-Type: "
                         "text/plain\nContent-Length: 12\n\nHello world!";

  write(client_socket, response, strlen(response));
  close(client_socket);
}

int main() {
  int server_fd, client_socket;
  struct sockaddr_in server_address, client_address;
  int opt = 1;
  int addrlen = sizeof(server_address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("Failed to set socket options");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    perror("Failed to bind");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("Failed to listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  while (1) {
    if ((client_socket = accept(server_fd, (struct sockaddr *)&client_address,
                                (socklen_t *)&addrlen)) < 0) {
      perror("Failed to accept");
      exit(EXIT_FAILURE);
    }
    handle_client(client_socket);
  }
}
