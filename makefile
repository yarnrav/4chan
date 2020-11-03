CC = cc

all:main.c
	${CC} -g -o main main.c -lcurl -ljansson -lpthread
