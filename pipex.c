/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:11:58 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/13 16:54:34 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
				break ;
		close(fildes[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(fildes[1]);
		waitpid(p, NULL, 0);
		redirect(&fildes[0], NULL, STDIN_FILENO, FALSE);
	}
}

/* Close all write ends but current i, close all read ends but i - 1 */

void	_close_pipes(int i, t_args *st)
{
	int j;

	j = 0;
	while (j < st->cmd_count - 1)
	{
		if (j != i)
			close(st->fildes[j][1]);
		if (j != i - 1)
			close(st->fildes[j][0]);
		j++;
	}
}

/* Each child reads from the prior pipe and writes to next pipe */

static int	_do_child_ops(int i, char *argv[], char *env[], t_args *st)
{
//1	close(st->fildes[i][0]);
	if (i == 0 && !st->heredoc)
		redirect(NULL, argv[1], STDIN_FILENO, FALSE);
	else if (i == 0 && st->heredoc)
		_heredoc(argv[2]);
	else
		redirect(&st->fildes[i - 1][0], NULL, STDIN_FILENO, FALSE);
	//ft_printf("read end duped \n"); fflush(stdout);
	if (i + 1 < st->cmd_count) {
		//dprintf(2, "write fd:%d\n", st->fildes[i][1]);
		redirect(&st->fildes[i][1], NULL, STDOUT_FILENO, FALSE);
		//dprintf(2, "write end duped\n");
	} 
	else if (st->heredoc)
		redirect(NULL, st->outfile, STDOUT_FILENO, TRUE);
	else
		redirect(NULL, st->outfile, STDOUT_FILENO, FALSE);
	//dprintf(2, "(TBD)Child closing fd already closed by parent:%d", st->fildes[i - 1][1]);
	//close(st->fildes[i - 1][1]);
	//list_open_fds();
	//dprintf(2, "Child executing...\n");
	if (st->cmdpaths[i] == NULL)
		return (FAILURE);
	else if (execve(st->cmdpaths[i], (char *const *)st->execargs[i],
			env) == -1)
		err("execve()", st, NULL, 0);
//	cleanup(st);
//	(void)env;
	return (SUCCESS);
}

/* Each parent needs to close pipe write ends for childs to read */
static void	_do_parent_ops(int i, int p, int *status, t_args *st)
{
	if (i + 1 < st->cmd_count)
	{ 
		//close(st->fildes[i][0]);
		close(st->fildes[i][1]);
		waitpid(p, status, 0);
	}
	else
	{
		waitpid(p, status, 0);
		//ft_printf("parent closing FD%d\n", st->fildes[i][1]);
		//close(st->fildes[i][1]);
	}
}

/* This scheme achieves child to child communication */
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
			_do_parent_ops(i, p, &status, &st);
		else
			err("fork", &st, NULL, 0);
	}
	cleanup(&st);
	return (get_exit_status(status));
}
