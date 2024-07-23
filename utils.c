#include "pipex.h"


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
