#include "pipex.h"


static void _free_arr(char **arr)
{
	while (arr)
	{
		free(*arr);
		arr++;
	}	
}

static void _free_arrarr(char ***arrarr)
{
	while (arrarr)
	{
		_free_arr(*arrarr);
		arrarr++;
	}
}

void	cleanup(t_args *st)
{
	_free_arr(st->cmdpaths);
	_free_arrarr(st->execargs);
	return ;
}
