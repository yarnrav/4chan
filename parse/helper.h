#ifndef STORE_HELPER
#define STORE_HELPER

#include <stdlib.h>
#include <stdio.h>

#include "string_funcs.h"
#include "../types.h"

void post_init(post post)
{
	post.name = (char *) malloc(sizeof(char));
	post.time = (char *) malloc(sizeof(char));
	post.heading = (char *) malloc(sizeof(char));
	post.message = (char *) malloc(sizeof(char));
}

void file_init(file file)
{
	file.file = (char *) malloc(sizeof(char));
	file.file_name = (char *) malloc(sizeof(char));
	file.thumbnail = (char *) malloc(sizeof(char));
}

void pages_init(pages *pages)
{
	pages->size = 0;
	pages->pages = (page *)malloc(sizeof(page) + 1);
}

void pages_add(pages *pages)
{
	pages->size++;
	pages->pages = (page *)realloc(pages->pages, sizeof(page) * (pages->size + 1));
} 
void pages_free(pages *pages)
{
	free(pages->pages);
}

void threads_init(threads *threads)
{
	threads->size = 0;
	threads->threads = (thread *)malloc(sizeof(thread) * (threads->size + 1));
}

void threads_add(threads *threads)
{
	threads->size++;
	threads->threads = (thread *)realloc(threads->threads, sizeof(thread) * (threads->size + 1));
}

void threads_free(threads *threads)
{
	free(threads->threads);
}

void replies_init(replies *replies)
{
	replies->size = 0;
	replies->replies = (reply *)malloc(sizeof(reply) * (replies->size + 1));
}

void replies_add(replies *replies)
{
	replies->size++;
	replies->replies = (reply *)realloc(replies->replies, sizeof(reply) * (replies->size + 1));
}

void replies_free(replies *replies)
{
	free(replies->replies);
}

void replace_com(char *formatted_thread_message)
{
	stringReplace("<br>", "\n", formatted_thread_message);
	stringReplace("&quot;", "\"", formatted_thread_message);
	stringReplace("<span class=\"quote\">", "", formatted_thread_message);
	stringReplace("&gt;", ">", formatted_thread_message);
	stringReplace("&#039;", "'", formatted_thread_message);
	stringReplace("</span>", "", formatted_thread_message);
	stringReplace("</a>", "", formatted_thread_message);
	string_del("<a href=\"", "\">", formatted_thread_message);
}

void catalog_free(catalog catalog)
{
	int i, j, k;
	for(i = 0; i < catalog.pages.size ; i++)
	{
		for(j = 0; j < catalog.pages.pages[i].threads.size ; j++)
		{
			free(catalog.pages.pages[i].threads.threads[j].post.message);
			free(catalog.pages.pages[i].threads.threads[j].post.heading);
			free(catalog.pages.pages[i].threads.threads[j].post.name);
			free(catalog.pages.pages[i].threads.threads[j].post.time);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.file);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.file_name);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.file_url);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.thumbnail_url);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.thumbnail);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.path_thumbnail);
			free(catalog.pages.pages[i].threads.threads[j].thread_file_data.path_file);
			for(k = 0; k < catalog.pages.pages[i].threads.threads[j].replies.size ; k++)
			{
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].post.heading);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].post.message);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].post.name);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].post.time);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.file_name);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.thumbnail);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.file);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.thumbnail_url);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.file_url);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.path_thumbnail);
				free(catalog.pages.pages[i].threads.threads[j].replies.replies[k].reply_file_data.path_file);
			}
			free(catalog.pages.pages[i].threads.threads[j].replies.replies);
		}
		threads_free(&catalog.pages.pages[i].threads);
	}
	puts("Freeing pages");
	pages_free(&catalog.pages);
}
#endif
