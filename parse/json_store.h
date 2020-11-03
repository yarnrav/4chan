#ifndef JSON_STORE
#define JSON_STORE

#include <jansson.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helper.h"
#include "string_funcs.h"
#include "../types.h"

#define HAS_STICKY_ATTRIBUTE 1
#define FORMATTED_REPLY_SAFE 10
#define IMAGE_URL_LENGTH 50
#define THUMBNAIL_FILE_LENGTH 25
#define FILE_LENGTH 25
#define THUMBNAIL_PATH_LENGTH 12
#define FILE_PATH_LENGTH 13

int posts;

int json_parse_post(json_t *post_array, post *post, int sticky_attr)
{
	json_t *com, *sub, *time, *id, *name, *sticky;

	// getting json objects
	com = json_object_get(post_array, "com");
	time = json_object_get(post_array, "now");
	id = json_object_get(post_array, "no");
	name = json_object_get(post_array, "name");
	sub = json_object_get(post_array, "sub");
	sticky = json_object_get(post_array, "sticky");
	
	// allocating strings
	//post_init(post);
	post->name = (char *) malloc(sizeof(char));
	post->time = (char *) malloc(sizeof(char));
	post->heading = (char *) malloc(sizeof(char));
	post->message = (char *) malloc(sizeof(char));
	char *formatted_post_message = (char *) malloc(sizeof(char));
	char *formatted_post_heading = (char *) malloc(sizeof(char));

	
	// temp strings for convenience
	const char *post_name = json_string_value(name);
	const char *post_time = json_string_value(time);
	int post_id = json_integer_value(id);
	int post_sticky = json_integer_value(sticky);

	// check if message exists
	if(json_is_string(com))
	{
		const char *post_message = json_string_value(com);
		// replace html cruft
		formatted_post_message = (char *) realloc(formatted_post_message, sizeof(char) * (strlen(post_message) + FORMATTED_REPLY_SAFE));
		strcpy(formatted_post_message, post_message);
		replace_com(formatted_post_message);
		
		// reallocs for malloced string in post_init
		post->message = (char *) realloc(post->message, sizeof(char) * (strlen(formatted_post_message) + 1) );
		strcpy(post->message, formatted_post_message);
		//puts(post->message);
	}
	else
	{
		post->message[0] = '\0';
	}

	// check if heading exists
	if(json_is_string(sub))
	{
		const char *post_heading = json_string_value(sub);
		// replace html cruft
		formatted_post_heading = (char *) realloc(formatted_post_heading, sizeof(char) * (strlen(post_heading) + FORMATTED_REPLY_SAFE));
		strcpy(formatted_post_heading, post_heading);
		replace_com(formatted_post_heading);

		// reallocs for malloced string in post_init
		post->heading = (char *) realloc(post->heading, sizeof(char) * (strlen(formatted_post_heading) + 1) );
		strcpy(post->heading, formatted_post_heading);
	}
	else
	{
		post->heading[0] = '\0';
	}
	
	// time
	post->time= (char *) realloc(post->time, sizeof(char) * (strlen(post_time) + 1) );
	strcpy(post->time, post_time);
	
	// name
	post->name = (char *) realloc(post->name, sizeof(char) * (strlen(post_name) + 1) );
	strcpy(post->name, post_name);

	// id
	post->id = post_id;
	
	if(sticky_attr == HAS_STICKY_ATTRIBUTE)
	{
		post->stuck = post_sticky; 
	}
	
	// free allocated temporary formatting strings
	free(formatted_post_heading);
	free(formatted_post_message);
	return 0;
}

int json_parse_file(json_t *post_array, file *post_file, char *board)
{
	json_t *file, *thumb, *ext, *h, *w;
	
	file = json_object_get(post_array, "filename");
	ext = json_object_get(post_array, "ext");
	thumb = json_object_get(post_array, "tim");
	w = json_object_get(post_array, "w");
	h = json_object_get(post_array, "h");
	
//	file_init(post_file);
	post_file->file = (char *) malloc(sizeof(char));
	post_file->file_name = (char *) malloc(sizeof(char));
	post_file->thumbnail = (char *) malloc(sizeof(char));
	post_file->file_url = (char *) malloc(sizeof(char));
	post_file->thumbnail_url = (char *) malloc(sizeof(char));
	post_file->path_file = (char *) malloc(sizeof(char));
	post_file->path_thumbnail = (char *) malloc(sizeof(char));

	const char *file_name = json_string_value(file);
	long long thumbnail = json_integer_value(thumb);
	const char *extension = json_string_value(ext);
	
	post_file->file_display_width = json_integer_value(w);
	post_file->file_display_height = json_integer_value(h);

	if(json_is_string(file))
	{
		post_file->file_name = (char *) realloc(post_file->file_name, sizeof(char) * (strlen(file_name) + strlen(extension) + 1 ) );
		sprintf(post_file->file_name, "%s%s", file_name, extension);
		
		post_file->file_url = (char *) realloc(post_file->file_url, sizeof(char) * (strlen(extension) + IMAGE_URL_LENGTH ) );
		sprintf(post_file->file_url, "https://i.4cdn.org/%s/%lld%s", board, thumbnail, extension);
		
		post_file->thumbnail_url = (char *) realloc(post_file->thumbnail_url, sizeof(char) * (IMAGE_URL_LENGTH + 4) );
		sprintf(post_file->thumbnail_url, "https://i.4cdn.org/%s/%llds.jpg", board, thumbnail);
		
		post_file->file = (char *) realloc(post_file->file, sizeof(char) * FILE_LENGTH );
		sprintf(post_file->file, "%lld%s", thumbnail, extension);
		
		post_file->thumbnail = (char *) realloc(post_file->thumbnail, sizeof(char) * THUMBNAIL_FILE_LENGTH );
		sprintf(post_file->thumbnail, "%llds.jpg", thumbnail);

		post_file->path_file = (char *) realloc(post_file->path_file, sizeof(char) * (strlen(post_file->file) + FILE_PATH_LENGTH + 2));
		sprintf(post_file->path_file, "images/large/%s", post_file->file);
		
		post_file->path_thumbnail = (char *) realloc(post_file->path_thumbnail, sizeof(char) * (strlen(post_file->thumbnail) + THUMBNAIL_PATH_LENGTH + 2));
		sprintf(post_file->path_thumbnail, "images/smol/%s", post_file->thumbnail);
	}
	else
	{
		post_file->file_name[0] = '\0';
		post_file->file[0] = '\0';
		post_file->thumbnail[0] = '\0';
		post_file->file_url[0] = '\0';
		post_file->thumbnail_url[0] = '\0';
		post_file->path_file[0] = '\0';
		post_file->path_thumbnail[0] = '\0';
	}

	return 0;
}

void json_parse_and_store_replies(json_t *replies_list, replies *replies, char *board)
{
	replies_init(replies);
	size_t k;
	for(k = 0; k < json_array_size(replies_list); k++)
	{
		replies_add(replies);
	
		json_t *replies_array = json_array_get(replies_list, k);
		
		posts++;
		
		if(!json_is_object(replies_array))
		{
			fprintf(stderr, "error: replies_array %ld is not an object\n", k + 1);
		}
		
		json_parse_post(replies_array, &replies->replies[k].post, 0);
		json_parse_file(replies_array, &replies->replies[k].reply_file_data, board);
	}
}

void 
json_parse_and_store_threads(json_t *threads_list, threads *threads, char *board)
{
	threads_init(threads);
	
	size_t j;
	
	for(j = 0; j < json_array_size(threads_list) - 1; j++)
	{
		threads_add(threads);
		json_t *threads_array = json_array_get(threads_list, j);
			
		if(!json_is_object(threads_array))
		{
			fprintf(stderr, "error: threads_array %ld is not an object\n", j + 1);
		}
		
		json_parse_post(threads_array, &threads->threads[j].post, 1);
		//puts(threads->threads[j].post.message);
		json_parse_file(threads_array, &threads->threads[j].thread_file_data, board);
		
		json_t *replies = json_object_get(threads_array, "last_replies");	
		
		if(!json_is_array(replies))
		{
			threads->threads[j].replies.size = 0;
		}
      		

		json_parse_and_store_replies(replies, &threads->threads[j].replies, board);
	}
}

int 
json_parse_and_store_catalog(char *json_stuff, catalog *catalog)
{
	json_t *root;
	json_error_t error;
	size_t i, j, k;
	int posts;

	root = json_loads(json_stuff, JSON_ALLOW_NUL, &error);
	free(json_stuff);

	if(!root)
	{
		fprintf(stderr, "error on line %d: %s\n", error.line, error.text);
		json_decref(root);
		return 1;
	}
	
	if(!json_is_array(root))
	{
		fprintf(stderr, "error: root is not an array\n");
		json_decref(root);
		return 1;
	}
	pages_init(&catalog->pages);
	for(i = 0; i <= json_array_size(root) - 1; i++)
	{
		printf("Page: %ld\n", i);
		pages_add(&catalog->pages);
		catalog->pages.pages[i].no = i + 1;
		json_t *data, *threads;
		data = json_array_get(root, i);

		if(!json_is_object(data))
		{
			fprintf(stderr, "error: data %ld is not an object\n", i + 1);
			json_decref(root);
			return 1;
		}
		
		threads = json_object_get(data, "threads");
		
		if(!json_is_array(threads))
		{
			fprintf(stderr, "error: %ld: threads is not an array\n", i + 1);
			json_decref(root);
			return 1;
	    	}
		catalog->board = (char *)malloc(sizeof(char) * 2);
		strcpy(catalog->board, "g");
		json_parse_and_store_threads(threads, &catalog->pages.pages[i].threads, catalog->board);
		free(catalog->board);
	}
	json_decref(root);
	return 0;
}
#endif
