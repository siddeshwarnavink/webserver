#include <stdlib.h>
#include <string.h>

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
