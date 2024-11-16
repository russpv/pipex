#include "pipex.h"

/* zsh error message */
int	permission_err(char *path, t_args *st, int idx)
{
	if (st)
		st->cmdpaths[idx] = NULL;
	ft_printf("Permission denied: %s", path);
	return (SUCCESS);
}

/* Prints error message and exits process safely.
 * Frees struct and any passed new string. 
 * Sets error number.
 */
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
