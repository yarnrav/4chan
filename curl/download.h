#ifndef PIC_DOWNLOAD
#define PIC_DOWNLOAD

/* This code for some reason feels pretty retarded and esoteric to me, but I dont know why */

#include <pthread.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>
#include <stdlib.h>
#include "../types.h"

// Used for passing arguments to the pthread callback functions
struct args{
	char *path;
	char *url;
};

// Pthread Callback Function

void *download_url(void *arg)
{
	CURL *curl;
	FILE *fp;
	struct args *temp = (struct args *) arg;
	fp = fopen(temp->path, "wb");
	curl = curl_easy_init();
	if(curl && fp != 0)
	{
		curl_easy_setopt(curl, CURLOPT_URL, temp->url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
		if(curl_easy_perform(curl) == CURLE_OK)
		{
			puts("success");
		}
		else
		{
			puts("failed or no file");	
		}
	}
	fclose(fp);
	curl_easy_cleanup(curl);
	free(arg);
	return NULL;
}

// The main function

int thumbnail_files_download(char *board, catalog catalog, int start_page_no, int page_no, int start_thread_no, int thread_no, int start_reply_no,  int reply_no)
{
	CURL *curl; 
	CURLcode res;
	
	// Increment variables, i for posts, j for threads, k for replies
	size_t i, j, k;

	// Number of image thumbnails
	int images = 0;
	
	// pthread array with arbitrary number of elements. Should make it dynamic later
	pthread_t tid[4096];
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	// Post loop 
	for(i = start_page_no; i<page_no; i++)
	{
		// Thread Loop
		for(j = start_thread_no; j < catalog.pages.pages[i].threads.size && j < thread_no ; j++)
		{
			// If there is a file struct
			if(catalog.pages.pages[i].threads.threads[j].thread_file_data.file[0] != '\0')
			{
				// Arguments to pass
				struct args *args = (struct args *)malloc(sizeof(struct args));
				
				args->url = catalog.pages.pages[i].threads.threads[j].thread_file_data.thumbnail_url;
				args->path = catalog.pages.pages[i].threads.threads[j].thread_file_data.path_thumbnail;
				
				int error = pthread_create(&tid[images], NULL, download_url, (void *)args);

				images++;
				
	
				if(0 != error)
					fprintf(stderr, "Couldn't run thread number %d, errno %d\n", images, error);
				else
					fprintf(stderr, "Thread %d, gets %s\n", images, args->url);
			}

			for(k = start_reply_no; k < reply_no; k++)
			{
				if(catalog.pages.pages[i].threads.threads[j].thread_file_data.file[0] != '\0')
				{
					struct args *args = (struct args *)malloc(sizeof(struct args));
					args->url = catalog.pages.pages[i].threads.threads[j].thread_file_data.thumbnail_url;
					args->path = catalog.pages.pages[i].threads.threads[j].thread_file_data.path_thumbnail;
					
					int error = pthread_create(&tid[images], NULL, download_url, (void *)args);
					
					images++;
					

					if(0 != error)
						fprintf(stderr, "Couldn't run thread number %d, errno %d\n", images, error);
					else
						fprintf(stderr, "Thread %d, gets %s\n", images, args->url);
				}
			}

		}
	}
	
	images = 0;
	for(i = start_page_no; i<page_no; i++)
	{
		for(j = start_thread_no; j < catalog.pages.pages[i].threads.size && j < thread_no; j++)
		{
			if(catalog.pages.pages[i].threads.threads[j].thread_file_data.file[0] != '\0')
			{
				pthread_join(tid[images], NULL);
				images++;
			}
			for(k = start_reply_no; k < reply_no; k++)
			{
				if(catalog.pages.pages[i].threads.threads[j].thread_file_data.file[0] != '\0')
				{
					pthread_join(tid[images], NULL);
					images++;
				}
			}

		}
	}
	curl_global_cleanup();
	return 0;	
}
#endif
