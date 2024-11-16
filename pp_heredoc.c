#include "pipex.h"

/* Sends one line of cl input to fildes */
static int	_do_heredoc_write(char *eof, int fildes)
{
	char			*userinput;
	int	bytes_read;

	userinput = NULL;
	//debug_print("%d:Got EOF:%s_\n", getpid(), eof);
	ft_printf("> ");
	bytes_read = 0;
	bytes_read = get_line(STDIN_FILENO, &userinput);
	if (ERROR == bytes_read)
		return (ERROR);
	if (ft_strlen(userinput) == ft_strlen(eof))
		if (ft_strncmp(userinput, eof, bytes_read) == 0)
			{
				debug_print("Got EOF!\n");
				free(userinput);
				return (SUCCESS);
			}
	if (bytes_read > 0) {
		debug_print("Wrote line:%s bytes:%d\n", userinput, bytes_read);
		write(fildes, userinput, bytes_read);
	}
	free(userinput);
	return (FAILURE);
}

/* Redirects tty input to stdin 
 * The upstream child process calls this 
 * so all pipe ends must be closed except 
 * for the first pipe's write end, which we 
 * immediately close after the fork. 
 */
void	get_heredoc(char *eof, t_args *st)
{
	int		fildes[2];
	pid_t	p;
	int		r;

	if (pipe(fildes) < 0)
		err("heredoc pipe()", NULL, NULL, 0);
	p = fork();
	if (p == -1)
		err("heredoc fork()", NULL, NULL, 0);
	if (0 == p)
	{
		close(st->fildes[0][1]);
		close(fildes[0]);
		while (1) {
			r = _do_heredoc_write(eof, fildes[1]);
			if (SUCCESS == r)
				break ;
			if (ERROR == r)
				err("get_line read()", st, NULL, 0);
		}
		debug_print("%d:heredoc child outtie\n",getpid());
		close(fildes[1]);
		cleanup_and_exit(st, 0);
	}
	else
	{
		close(fildes[1]);
		/*int *status = NULL;
		waitpid(p, status, 0);
		if (p > 0 && WIFEXITED(*status))
			debug_print("Child %d exited w/ stat:%d\n", p,
				WEXITSTATUS(*status));
		else if (p > 0 && WIFSIGNALED(*status))
			debug_print("Child %d exited by sig:%d\n", p,
				WTERMSIG(*status));*/
		redirect(&fildes[0], NULL, STDIN_FILENO, false);
	}
}
