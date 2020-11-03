#ifndef TYPES
#define TYPES

typedef struct _file{
	char *thumbnail, *file, *file_name, *path_thumbnail, *path_file, *file_url, *thumbnail_url;
	int file_display_width, file_display_height;
}file;

typedef struct _post{
	char *message, *heading, *name, *time;
	int id, stuck;
}post;

typedef struct _reply{
	post post;
	file reply_file_data;
}reply;

typedef struct _replies{
	reply *replies;
	int size;
}replies;

typedef struct _thread{
	post post;
	file thread_file_data;
	replies replies;
}thread;

typedef struct _threads{
	thread *threads;
	int size;
}threads;

typedef struct _page{
	threads threads;
	int no;
}page;

typedef struct _pages{
	page *pages;
	int size;
}pages;

typedef struct _catalog{
	pages pages;
	char *board;
}catalog;

#endif 
