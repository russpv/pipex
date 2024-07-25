#include "pipex.h"


void    err(const char *msg, t_args *st, char *str)
{
    perror(msg);
	if (st)
		cleanup(st);
	if (str)
		free (str);
    exit(EXIT_FAILURE);
}

static void _free_arr(char **arr)
{
	while (*arr)
	{
		free(*arr);
		arr++;
	}	
}

static void _free_arrarr(char ***arrarr)
{
	while (*arrarr)
	{
		_free_arr(*arrarr);
		arrarr++;
	}
}

void	cleanup(t_args *st)
{
	if (st->cmdpaths)
		_free_arr(st->cmdpaths);
	if (st->execargs)
		_free_arrarr(st->execargs);
	return ;
}

void	arg_error(void)
{
	ft_putstr_fd("\033[31mError: Bad argument\n\e[0m", 2);
	ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <...> <file2>\n", 1);
	ft_putstr_fd("    ./pipex \"here_doc\" <LIMITER> <cmd> <cmd1> <...> <file>\n", 1);
	exit(EXIT_SUCCESS);
}

void    printarr(char **arr)
{
    while (arr && *arr) {
        printf("%s\n", *arr);
        arr++;
    } fflush(stdout);
}

void	printarrarr(char ***arr)
{
	while (arr && *arr)
	{
		printarr(*arr);
		arr++;
	} 
}

/* simple fixed buffer version */
char	*get_next_line(int fd)
{
	char *buf;
	char *p;
	int i;
	int bytes;

	buf = malloc(sizeof(char) * BUFSZ);
	if (!buf)
		return (NULL);
	p = buf;
	i = 0;
	p[0] = '0';
	while (i < BUFSZ)
	{
		bytes = read(fd, buf + i, 1);
		if (bytes == -1)
			err("get next line read()", NULL, NULL);
		if (bytes == 0 || p[i] == '\0' || p[i] == '\n')
			break ;
		i++;
	}
	return (buf);
}

/* searches **env for "PATH" and returns index */
int	get_env_path(char **env)
{
	const char *path = "PATH"; 
	char *s;
	int i;
	int idx;

	s = (char *)path;
	i = 0;
	idx = 0;
	while (env[idx])
	{
		while (env[idx][i] == s[i] && s[i] && env[idx][i])
			i++;
		if (s[i] == '\0')
			return (idx);
		s = (char *)path;
		idx++;
	}
	return (FAILURE);
}