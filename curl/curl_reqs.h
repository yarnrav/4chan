#ifndef CURL_REQS
#define CURL_REQS

#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>
#include <stdlib.h>

struct memory{
	char *memory;
	size_t size;
};

size_t write_callback(char *contents, size_t size, size_t membnum, void *userp){
        size_t realsize = size * membnum;
        struct memory *mem = (struct memory *)userp;
        char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
        if(ptr == NULL)
                return 0;
        mem->memory = ptr;
        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
        return realsize;
}

int init_get_data(char *url, struct memory *chunk)
{
	CURL *curl;
	CURLcode res;
	chunk->memory = (char *)malloc(1);
	chunk->size = 0;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if(res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			return 1;
		}
	
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;
}
#endif
