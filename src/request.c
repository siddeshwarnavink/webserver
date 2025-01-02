#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "request.h"
#include "mem.h"

request init_request() {
	request req = (request)malloc(sizeof(*req));
	if(req == NULL) {
		return NULL;
	}

	req->method = (char *)malloc(16 * sizeof(char));
	req->path = (char *)malloc(256 * sizeof(char));
	req->query = NULL;
	req->query_allocated = 0;
	req->body = NULL;
	req->body_allocated = 0;
	if (req->method == NULL || req->path == NULL) {
		free(req->method);
		free(req->path);
		free(req);
		return NULL;
	}

	mem_add(&req, free_request);

	return req;
}

void free_request(request *req) {
	if (req == NULL || *req == NULL) {
		return;
	}

	free((*req)->method);
	free((*req)->path);
	if((*req)->query_allocated) {
		free((*req)->query);
	}
  if((*req)->body_allocated) {
		free((*req)->body);
	}

	free(*req);
	*req = NULL;
}

char *get_request_body(request req, const char *field) {
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
	mem_add(value, NULL);

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
	value = NULL;
	return decoded_value;
}
