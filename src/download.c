#include "download.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

typedef struct buffer {
	char* data;
	size_t size;
} buffer;

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t total = size * nmemb;
	buffer* mem = (buffer*)userp;

	char* ptr = realloc(mem->data, mem->size + total + 1);
	if (!ptr) {
		return 0;
	}

	mem->data = ptr;
	memcpy(&(mem->data[mem->size]), contents, total);
	mem->size += total;
	mem->data[mem->size] = '\0';

	return total;
}

int download_gl_xml(char** out) {
	CURL* curl = curl_easy_init();
	CURLcode res;

	if (!curl) {
		return 1;
	}

	buffer chunk = { 
		.data = NULL,
		.size = 0
	};

	curl_easy_setopt(curl, CURLOPT_URL, LXGL_GL_XML_URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		return 2;
	}

	curl_easy_cleanup(curl);

	*out = chunk.data;
	return 0;
}
