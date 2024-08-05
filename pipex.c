#include "pipex.h"

/* PIPEX
** Expects "[infile] [command] | ... | [command] > [outfile]" args
*/

static int	_do_heredoc_write(char *eof, int fildes)
{
	char	*userinput;

	userinput = NULL;
	ft_printf("> ");
	userinput = get_line(STDIN_FILENO);
	if (ft_strncmp(userinput, eof, ft_strlen(eof)) == 0)
	{
		free(userinput);
		return (SUCCESS);
	}
	write(fildes, userinput, ft_strlen(userinput));
	free(userinput);
	return (FAILURE);
}

static void	_heredoc(char *eof)
{
	int		fildes[2];
	pid_t	p;

	if (pipe(fildes) < 0)
		err("heredoc pipe()", NULL, NULL, 0);
	p = fork();
	if (p == -1)
		err("heredoc fork()", NULL, NULL, 0);
	if (0 == p)
	{
		close(fildes[0]);
		while (1)
			if (_do_heredoc_write(eof, fildes[1]) == SUCCESS)
				exit(EXIT_SUCCESS);
	}
	else
	{
		close(fildes[1]);
		waitpid(p, NULL, 0);
		redirect(&fildes[0], NULL, STDIN_FILENO);
	}
}

static int	_do_child_ops(int i, char *argv[], char *env[], t_args *st)
{
	close(st->fildes[i][0]);
	if (i == 0 && !st->heredoc)
		redirect(NULL, argv[1], STDIN_FILENO);
	else if (i == 0 && st->heredoc)
		_heredoc(argv[2]);
	else
		redirect(&st->fildes[i - 1][0], NULL, STDIN_FILENO);
	if (i + 1 < st->cmd_count)
		redirect(&st->fildes[i][1], NULL, STDOUT_FILENO);
	else
		redirect(NULL, st->outfile, STDOUT_FILENO);
	if (st->cmdpaths[i] == NULL)
		return (FAILURE);
	else if (execve(st->cmdpaths[i], (char *const *)st->execargs[i],
			env) == -1)
		err("execve()", st, NULL, 0);
	return (SUCCESS);
}

static void	_do_parent_ops(int i, int p, int *status, t_args *st)
{
	if (i + 1 < st->cmd_count)
		waitpid(p, status, WNOHANG);
	else
		waitpid(p, status, 0);
	close(st->fildes[i][1]);
	if (i != 0)
		close(st->fildes[i - 1][0]);
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
			if (_do_child_ops(i, argv, env, &st) == FAILURE)
				exit(127);
		}
		else if (p > 0)
		{
			_do_parent_ops(i, p, &status, &st);
		}
		else
			err("fork", &st, NULL, 0);
	}
	return (get_exit_status(status));
}
