#include "pipex.h"

/* Closes all the pipe ends for the i-th child
 * Critical to ensure SIGPIPE is sent to cmds with
 * no readers
 *
 * Closes all read ends except the prior pipe's
 * Closes all write ends except the ith pipe's
 *
 * For 1st child with heredoc, closes 1st pipe read end
 */
static int	_close_other_pipe_ends(t_args *st, int i)
{
	int	pipe;
	int	counter;

	pipe = -1;
	counter = 0;
	while (++pipe < st->cmd_count - 1)
	{
		if (pipe != i)
		{
			close(st->fildes[pipe][1]);
			counter++;
		}
		if (pipe != i - 1)
		{
			close(st->fildes[pipe][0]);
			counter++;
		}
	}
	debug_print("Child %d: closed %d pipe ends (%d cmds)\n", getpid(), counter,
		st->cmd_count);
	return (1);
}

static inline void	_do_child_inputs(t_args *st, char *argv[], int i)
{
	int	r;

	r = 0;
	if (i == 0 && st->heredoc)
		get_heredoc(argv[2], st);
	else if (i == 0 && !st->heredoc)
		r = redirect(NULL, argv[1], STDIN_FILENO, NO_APND);
	else
		r = redirect(&st->fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
	if (r == -1)
		err("Open (input)", st, NULL, 0);
}

static inline void	_do_child_outputs(t_args *st, int i)
{
	int	r;

	r = 0;
	if (i < st->cmd_count - 1)
		r = redirect(&st->fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	else if (i == st->cmd_count - 1 && st->heredoc)
		r = redirect(NULL, st->outfile, STDOUT_FILENO, APPEND);
	else
		r = redirect(NULL, st->outfile, STDOUT_FILENO, NO_APND);
	if (r == -1)
		err("Open (output)", st, NULL, 0);
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
int	do_child_ops(int i, char *argv[], char *env[], t_args *st)
{
	_close_other_pipe_ends(st, i);
	_do_child_inputs(st, argv, i);
	_do_child_outputs(st, i);
	debug_print("Child exec'g %s, %s\n", st->cmdpaths[i], *st->execargs[i]);
	if (st->cmdpaths[i] == NULL)
		return (FAILURE);
	else if (execve(st->cmdpaths[i], (char *const *)st->execargs[i], env) == -1)
		err("execve()", st, NULL, 0);
	return (SUCCESS);
}
