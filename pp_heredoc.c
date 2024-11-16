#include "pipex.h"

/* Sends user prompt input to fildes until EOF entered */
static int	_do_heredoc_write(char *eof, int fildes)
{
	char			*userinput;
	unsigned int	bytes_read;

	userinput = NULL;
	ft_printf("> ");
	bytes_read = get_line(STDIN_FILENO, &userinput);
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
void	get_heredoc(char *eof)
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
