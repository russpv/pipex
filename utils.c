#include "pipex.h"

/* searches **env for "PATH" and returns index */
int	get_env_path(char **env)
{
	const char	*path = "PATH";
	char		*s;
	int			i;
	int			idx;

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

/* Interpret waitpid() exit status (signals ignored here) */
int	get_exit_status(int status)
{
	int	exit_status;
	int	signal_number;

	exit_status = 0;
	signal_number = 0;
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		signal_number = WTERMSIG(status);
	return (exit_status);
}

/* _REDIRECT() Wraps open() and dup2() */
/* 1. Opens input file ror reading
 * 2. Opens output file for writing
 * fromfile is only ever stdin or stdout
 */
int	redirect(int *to, char *topath, int from)
{
	int	fd;

	if (!to && (topath != NULL && *topath))
	{
		if (from == STDIN_FILENO && access(topath, R_OK) == -1)
		{
			ft_printf("warning: An error occurred while redirecting file '%s'", \
						topath);
			fd = open("/dev/null", O_RDONLY);
		}
		else if (from == STDIN_FILENO)
			fd = open(topath, O_RDONLY | O_CREAT, 0644);
		else if (from == STDOUT_FILENO)
			fd = open(topath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = -1;
		if (fd < 0)
			err("redirect() open", NULL, NULL, 0);
	}
	else
		fd = *to;
	if (dup2(fd, from) == -1)
		err("dup2", NULL, NULL, 0);
	close(fd);
	return (from);
}

// Dynamically allocates pipe fds
void	create_pipes(t_args *st)
{
	int	i;

	i = 0;
	st->fildes = malloc(st->cmd_count * sizeof(int *));
	if (!st->fildes)
		err("malloc", st, NULL, 0);
	while (i < st->cmd_count)
	{
		st->fildes[i] = malloc(3 * sizeof(int));
		if (!st->fildes[i])
			err("malloc", st, NULL, 0);
		st->fildes[2] = NULL;
		if (pipe(st->fildes[i]) < 0)
			err("pipe", st, NULL, 0);
		i++;
	}
}

void	init_struct(int argc, char **argv, char **env, t_args *st)
{
	st->outfile = "";
	st->argc = argc;
	st->cmdpaths = NULL;
	st->execargs = NULL;
	st->fd = 0;
	st->fildes = NULL;
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		st->heredoc = TRUE;
	else
		st->heredoc = FALSE;
	st->cmd_count = argc - (3 + st->heredoc);
	if (argc < (MINARGS + (int)st->heredoc))
		err("Insufficient arguments.", st, NULL, EINVAL);
	st->outfile = argv[argc - 1];
	st->path_offset = get_env_path(env);
	if (st->path_offset == FAILURE)
		err("Could not find PATH in env", st, NULL, 0);
}
