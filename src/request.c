/* vi:set ts=2 sts=2 sw=2 et:
 *
 * request.c - Handle HTTP request
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "request.h"
#include "mem.h"
#include "log.h"

request init_request() {
	request req = (request)mem_alloc(sizeof(struct sRequest));
	if(req == NULL) {
		return NULL;
	}

	req->method = (char *)mem_alloc(16 * sizeof(char));
	req->path = (char *)mem_alloc(256 * sizeof(char));
	req->query = NULL;
	req->query_allocated = 0;
	req->body = NULL;
	req->body_allocated = 0;
	req->cookies = NULL;
	if (req->method == NULL || req->path == NULL) {
		mem_free(req->method);
		mem_free(req->path);
		mem_free(req);
		return NULL;
	}

	return req;
}

void free_request(request *req) {
	if (*req == NULL) {
		LOG("Request already freed\n");
		return;
	}

	mem_free((*req)->method);
	mem_free((*req)->path);
	if((*req)->query_allocated) {
		mem_free((*req)->query);
	}
  if((*req)->body_allocated) {
		mem_free((*req)->body);
	}
	mem_free((*req)->cookies);

	mem_free(*req);
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

	char *value = (char *)mem_alloc(value_len + 1);

	if (!value) {
		perror("Failed to allocate memory for value");
		exit(EXIT_FAILURE);
	}
	strncpy(value, key_start, value_len);
	value[value_len] = '\0';

	char *decoded_value = (char *)mem_alloc(value_len + 1);
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

	mem_free(value);
	return decoded_value;
}
