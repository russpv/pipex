#ifndef PIPEX
# define PIPEX

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdbool.h>

#define SUCCESS 0
#define FAILURE -1
#define MINARGS 5
#define PATHBUF 1024
#define BUFSZ 1024

typedef struct s_args {
	int argc;
	int cmd_count;
	int path_offset;
	char **cmdpaths;
	char ***execargs;
	char **envp;

	int fd;
	int fildes[2];
	int fildes2[2];

	char *outfile;

	t_bool	heredoc;
} t_args;

int	get_env_path(char **env);
int	parse_args(int argc, char **argv, char **env, t_args *st);
void	cleanup(t_args *st);
void	err(const char *msg, t_args *st, char *str);
void	printarr(char **arr);
void	printarrarr(char ***arr);

/* Utils.c */
char	*get_next_line(int fd);
#endif
