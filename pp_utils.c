#include "pipex.h"

/* If cmd path is accessible, loads it into struct
 * else exits safely.
 */
int	check_access(char *path, int idx, t_args *st)
{
	if (access((const char *)path, F_OK) == 0)
	{
		if (access((const char *)path, X_OK) != 0)
			return (permission_err(path, st, idx));
		st->cmdpaths[idx] = ft_strdup(path);
		if (!st->cmdpaths[idx])
			err("_load_cmdpath: strdup() failure", st, NULL, 0);
		return (SUCCESS);
	}
	return (FAILURE);
}

void	debug_print(const char *format, ...)
{
	va_list	args;

	if (DEBUG)
	{
		va_start(args, format);
		fprintf(stderr, "DEBUG: ");
		vfprintf(stderr, format, args);
		va_end(args);
	}
}
