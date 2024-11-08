/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:11 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/13 17:20:26 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (*env)
		env++;
	fflush(stderr);
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

static inline int	_redirect_logic(char *topath, int from, t_bool append)
{
	int	fd;

	if (from == STDIN_FILENO && access(topath, R_OK) == -1)
	{
		ft_printf("warning: An error occurred while redirecting file '%s'", \
				topath);
		fd = open("/dev/null", O_RDONLY);
	}
	else if (from == STDIN_FILENO)
		fd = open(topath, O_RDONLY | O_CREAT, 0644);
	else if (from == STDOUT_FILENO && append)
		fd = open(topath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (from == STDOUT_FILENO)
		fd = open(topath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = -1;
	return (fd);
}

/* _REDIRECT() Wraps open() and dup2() */
/* 1. Opens input file ror reading
 * 2. Opens output file for writing
 * fromfile is only ever stdin or stdout
 * Error message is FISH version
 */

int	redirect(int *to, char *topath, int from, t_bool append)
{
	int	fd;

	if (!to && (topath != NULL && *topath))
	{
		fd = _redirect_logic(topath, from, append);
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

/* Allocates pipe fd's per 'argc - 4 - heredoc' */
/* fildes is an array of int* */
void	create_pipes(t_args *st)
{
	int	i;

	i = 0;
	st->fildes = malloc((st->cmd_count) * (sizeof(int *)));
	if (!st->fildes)
		err("malloc", st, NULL, 0);
	st->fildes[st->cmd_count - 1] = NULL;
	while (i < st->cmd_count - 1)
	{
		st->fildes[i] = malloc(2 * sizeof(int));
		if (!st->fildes[i])
			err("malloc", st, NULL, 0);
		if (pipe(st->fildes[i]) < 0)
			err("pipe", st, NULL, 0);
		i++;
	}
}
