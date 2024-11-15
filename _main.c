/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:11:58 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/20 19:00:31 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* PIPEX
** Expects args "[infile] 	[command]  ... 	[command] 	[outfile]"
** Like 		"[infile] < [command] |...| [command] > [outfile]"
** 
** OR
** 		"./pipex here_doc LIMITER cmd ... cmd file"
** 	like	"cmd << LIMITER | ... | cmd >> file"
**
** Designed to mimic ZSH pipe behavior with FISH messages (more descriptive)
**
** Uses only: open, close, read, write, malloc, free, perror,
** 		access, dup, dup2, execve, exit, fork, pipe, wait, waitpid
*/

/* Sends user prompt input to fildes until EOF entered */
static int	_do_heredoc_write(char *eof, int fildes)
{
	char	*userinput;
	unsigned int	bytes_read;

	userinput = NULL;
	ft_printf("> ");
	bytes_read = get_line(STDIN_FILENO, &userinput); //TODO am I getting each line or the whole shebang?
	if (ft_strncmp(userinput, eof, ft_strlen(eof)) == 0)
	{
		free(userinput);
		return (SUCCESS);
	}
	write(fildes, userinput, bytes_read);
	free(userinput);
	return (FAILURE);
}

/* Loads here-document input to stdin via pipe */
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

int	close_pipes(t_args *st, int i)
{
// TODO: closes all pipe fd's except the ones needed
	int j = 0;
	while (++j < st->cmd_count)
	{
		if (j == i)
			continue ;
		close(st->fildes[i][0]); /* Close the other read ends except mine */
		close(st->fildes[i][1]); /* Close other write ends except mine */
	}
}

/* This executes the piped cmd and
 * Manages the various I/O redirections whether process is first/heredoc, 
 * mid, or last in the pipeline.
 * 
 * Each child reads from the prior child's write and 
 * writes to next pipe (or file for last cmd)
 * Each child writes to pipe first, then next child reads.
 * Each pipe fd must be closed in every fork'd process.
 */
static int	_do_child_ops(int i, char *argv[], char *env[], t_args *st)
{
	int r;

	r = 0;
	if (i == 0 && st->heredoc)
		_heredoc(argv[2]);
	else 
	{ /* here, two pipes are active */
		//if (i >= 1) // close prior write end ??
		//	close(st->fildes[i - 1][1]);
		close_pipes(st, i);
		if (i == 0 && !st->heredoc)
			r = redirect(NULL, argv[1], STDIN_FILENO, NO_APND);
		else
			r = redirect(&st->fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
		if (r == -1)
			err("Open (input)", st, NULL, 0);
		if (i + 1 < st->cmd_count)
			r = redirect(&st->fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
		else if (st->heredoc)
			r = redirect(NULL, st->outfile, STDOUT_FILENO, APPEND);
		else
			r = redirect(NULL, st->outfile, STDOUT_FILENO, NO_APND);
		if (r == -1)
			err("Open (output)", st, NULL, 0);
	}
	if (st->cmdpaths[i] == NULL)
		return (FAILURE);
	else if (execve(st->cmdpaths[i], (char *const *)st->execargs[i], env) == -1)
		err("execve()", st, NULL, 0);
	return (SUCCESS);
}

/* This manages the wait() behavior of each parent.
 * The last wait blocks. 
 
 * TODO: Test if a separate loop wait() for all
 * children is equivalent to this.
 * 
 * Returns exit status by reference. 
 * Parents 
 */
// **** VERSION 1 *****
/*static void	_do_parent_ops(int i, int p, int *status, t_args *st)
{
	int	j;

	if (i + 1 < st->cmd_count)
	{
		close(st->fildes[i][1]);
		waitpid(p, status, WNOHANG);
	}
	else
	{
		waitpid(p, status, 0);
		j = -1;
		while (st->fildes[++j])
			close(st->fildes[j][0]);
	}
}*/
#include <sys/types.h>
#include <sys/wait.h>
static void	_waitchild(int *status, t_args *st)
{
    for (int i = 0; i < 3; i++) {
		if (i >= 1)
			close(st->fildes[i - 1][0]);
        pid_t child_pid = waitpid(-1, status, 0); // Wait for any child process
        if (child_pid > 0) {
            // Process exited normally, or was terminated by a signal
            if (WIFEXITED(*status)) {
                printf("Child %d exited with status %d\n", child_pid, WEXITSTATUS(*status));
            } else if (WIFSIGNALED(*status)) {
                printf("Child %d exited due to signal %d\n", child_pid, WTERMSIG(*status));
            }
        }
    }

    printf("All child processes have terminated\n");
}

/* This scheme achieves CHILD-TO-CHILD communication 
 * 
 * Creates all pipes and launches all commands w/o
 * waiting (i.e. in parallel). 
 * 
 * Waits for report from kernel for all children before returning
 */
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
				cleanup_and_exit(&st, 127);
		}
		else if (p > 0) {
			if (i < st.cmd_count && i > 0) {
				close(st.fildes[i - 1][1]);//_do_parent_ops(i, p, &status, &st);
				
			}
		}else
			err("fork", &st, NULL, 0);
	}
	_waitchild(&status, &st);
	cleanup(&st);
	return (get_exit_status(status));
}

/* Close all write ends but current i, close all read ends but i - 1 */
/*
void	_close_pipes(int i, t_args *st)
{
	int	j;

	j = 0;
	while (j < st->cmd_count - 1)
	{
		if (j != i)
			close(st->fildes[j][1]);
		if (j != i - 1)
			close(st->fildes[j][0]);
		j++;
	}
}*/
