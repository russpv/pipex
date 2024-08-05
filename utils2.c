#include "pipex.h"

/* zsh error message */
int	permission_err(char *path, t_args *st, int idx)
{
	if (st)
		st->cmdpaths[idx] = NULL;
	ft_printf("permission denied: %s", path);
	return (EXIT_SUCCESS);
}

void	err(const char *msg, t_args *st, char *str, int ern)
{
	if (ern != 0)
		errno = ern;
	perror(msg);
	if (st)
		cleanup(st);
	if (str)
		free(str);
	exit(EXIT_FAILURE);
}

static void	_free_arr(char **arr)
{
	while (*arr)
	{
		free(*arr);
		arr++;
	}
}

static void	_free_arrarr(char ***arrarr)
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
