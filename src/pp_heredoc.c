/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pp_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:19:32 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/18 15:19:33 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Simple unbuffered 1-char read until newline
 * Retains newline in passed buf
 * Caller must free passed buffer */
static inline int	_get_line(int fd, char **buf)
{
	int	i;
	int	b;
	int	bytes;
	int	done;

	*buf = malloc(sizeof(char) * BUFSZ);
	if (!*buf)
		return (ERROR);
	i = 0;
	bytes = 0;
	done = 0;
	while (i < BUFSZ - 1 && !done)
	{
		b = read(fd, (*buf) + i, 1);
		if (-1 == b)
			return (ERROR);
		bytes += b;
		if (0 == b || (*buf)[i] == '\n')
			done = 1;
		i++;
	}
	(*buf)[i] = '\0';
	return (bytes);
}

/* Sends one line of cl input to fildes */
static int	_do_heredoc_write(t_args *st, char *eof, int fildes)
{
	char	*userinput;
	int		bytes_read;

	userinput = NULL;
	bytes_read = 0;
	ft_printf("> ");
	bytes_read = _get_line(STDIN_FILENO, &userinput);
	if (ERROR == bytes_read)
		err("get_line read()", st, userinput, 0);
	if (ft_strlen(userinput) - 1 == ft_strlen(eof) && ft_strncmp(userinput, eof,
			bytes_read - 2) == 0)
	{
		free(userinput);
		return (SUCCESS);
	}
	if (bytes_read > 0)
		write(fildes, userinput, bytes_read);
	free(userinput);
	return (FAILURE);
}

/* Redirects tty input to stdin
 * The upstream child process calls this
 * so all pipe ends must be closed except
 * for the first pipe's write end, which we
 * immediately close after the fork.
 */
int	get_heredoc(char *eof, t_args *st)
{
	int		fildes[2];
	pid_t	p;

	if (pipe(fildes) < 0)
		err("heredoc pipe()", NULL, NULL, 0);
	p = fork();
	if (-1 == p)
		err("heredoc fork()", NULL, NULL, 0);
	else if (0 == p)
	{
		close(st->fildes[0][1]);
		close(fildes[0]);
		while (1)
			if (_do_heredoc_write(st, eof, fildes[1]) == SUCCESS)
				break ;
		close(fildes[1]);
		cleanup_and_exit(st, 0);
	}
	close(fildes[1]);
	wait(NULL);
	redirect(&fildes[0], NULL, STDIN_FILENO, false);
	return (1);
}
