#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "request.h"

Request init_request() {
	Request req = (Request)malloc(sizeof(*req));
	if(req == NULL) {
		return NULL;
	}

	req->method = (char *)malloc(16 * sizeof(char));
	req->path = (char *)malloc(256 * sizeof(char));

	return req;
}

void free_request(Request req) {
	if(req != NULL) {
		free(req->method);
		free(req->path);
		free(req);
	}
}

char *get_request_body(Request req, const char *field) {
	char *body = req->body;
	size_t field_len = strlen(field);
	char *key_start = strstr(body, field);

	if (!key_start || (key_start[field_len] != '=' && key_start[field_len] != '\0')) {
		return NULL;
	}

	key_start += field_len + 1;
	char *value_end = strchr(key_start, '&');
	size_t value_len = value_end ? (size_t)(value_end - key_start) : strlen(key_start);

	char *value = (char *)malloc(value_len + 1);
	if (!value) {
		perror("Failed to allocate memory for value");
		exit(EXIT_FAILURE);
	}
	strncpy(value, key_start, value_len);
	value[value_len] = '\0';

	char *decoded_value = (char *)malloc(value_len + 1);
	if (!decoded_value) {
		perror("Failed to allocate memory for decoded value");
		exit(EXIT_FAILURE);
	}
	char *src = value;
	char *dest = decoded_value;

	while (*src) {
		if (*src == '%') {
			int code;
			if (sscanf(src + 1, "%2x", &code) != 1) code = '?';
			*dest++ = code;
			src += 3;
		} else if (*src == '+') {
			*dest++ = ' ';
			src++;
		} else {
			*dest++ = *src++;
		}
	}
	*dest = '\0';

	free(value);
	return decoded_value;
}
