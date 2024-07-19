#ifndef PIPEX
# define PIPEX

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

#define SUCCESS 0
#define FAILURE -1

typedef struct my_s {
	char *infile;
	char *outfile;
	char *content;
	int	status;
	int cmds;
}	t_args;



#endif
