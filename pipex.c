#include "pipex.h"
#include <errno.h>

/* PIPEX
** Expects "[infile] [command] | ... | [command] > [outfile]" args
*/

/* _REDIRECT() Wraps open() and dup2() */
/* 1. Opens input file ror reading
 * 2. Opens output file for writing
 * fromfile is only ever stdin or stdout
 */
static int	_redirect(int *to, char *topath, int from)
{
	int	fd;

	if (!to && (topath != NULL && *topath))
	{
		if (from == STDIN_FILENO && access(topath, R_OK) == -1)
			// bad cmd last round
		{
			ft_printf("warning: An error occurred while redirecting file '%s'",
				topath);
			fflush(stdout);
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

static void	_heredoc(char *lim)
{
	int		fildes[2];
	pid_t	p;
	char	*userinput;

	userinput = NULL;
	if (pipe(fildes) < 0)
		err("heredoc pipe()", NULL, NULL, 0);
	p = fork();
	if (p == -1)
		err("heredoc fork()", NULL, NULL, 0);
	if (0 == p)
	{
		close(fildes[0]);
		while (1)
		{
			ft_printf("> ");
			fflush(stdout);
			userinput = get_line(STDIN_FILENO);
			if (ft_strncmp(userinput, lim, ft_strlen(lim)) == 0)
			{
				free(userinput);
				exit(EXIT_SUCCESS);
			}
			write(fildes[1], userinput, ft_strlen(userinput));
			free(userinput);
		}
	}
	else
	{
		close(fildes[1]);
		waitpid(p, NULL, 0);
		_redirect(&fildes[0], NULL, STDIN_FILENO);
	}
}

/* Interpret waitpid() exit status (signals ignored here) */
static inline int	_get_exit_status(int status)
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

int	main(int argc, char *argv[], char *env[])
{
	t_args	st;
	pid_t	p;
	int		i;
	int		status;

	parse_args(argc, argv, env, &st);
	create_pipes(&st);
	i = -1;
	while (++i < st.cmd_count)
	{
		p = fork();
		if (0 == p)
		{
			close(st.fildes[i][0]);
			if (i == 0 && !st.heredoc)
				_redirect(NULL, argv[1], STDIN_FILENO);
			else if (i == 0 && st.heredoc)
				_heredoc(argv[2]);
			else
				_redirect(&st.fildes[i - 1][0], NULL, STDIN_FILENO);
			if (i + 1 < st.cmd_count)
				_redirect(&st.fildes[i][1], NULL, STDOUT_FILENO);
			else
				_redirect(NULL, st.outfile, STDOUT_FILENO);
			if (st.cmdpaths[i] == NULL)
				exit(127);
			else if (execve(st.cmdpaths[i], (char *const *)st.execargs[i],
					env) == -1)
			{
				ft_printf("err:%d", errno);
				fflush(stdout);
				sleep(1);
				err("execve()", &st, NULL, 0);
			}
		}
		else if (p > 0)
		{
			if (i + 1 < st.cmd_count)
				waitpid(p, &status, WNOHANG);
			else
				waitpid(p, &status, 0);
			close(st.fildes[i][1]);
			if (i != 0)
				close(st.fildes[i - 1][0]);
		}
		else
			err("fork", &st, NULL, 0);
	}
	return (_get_exit_status(status));
}
