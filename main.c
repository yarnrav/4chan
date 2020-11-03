#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "curl/curl_reqs.h"
#include "curl/download.h"
#include "parse/json_store.h"
#include "types.h"

struct memory chunk;

int main(void)
{
	if(init_get_data("https://a.4cdn.org/g/catalog.json", &chunk))
	{
		return 1;
	}
	FILE *fp = fopen("data", "w");
	fprintf(fp, "%s", chunk.memory);
	fclose(fp);
	catalog catalog;
	json_parse_and_store_catalog(chunk.memory, &catalog);
	puts("starting");
	thumbnail_files_download("g", catalog, 0, 2, 0, 14, 0, 0);
	catalog_free(catalog);
	return 0;
}
