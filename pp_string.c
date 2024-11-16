/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pp_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:40 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/16 20:49:42 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Removes backslash escape chars from a single command string
 * and returns it by reference.
 */
int	process_string(char **args)
{
	char	*ptrs[3];

	if (!args)
		return (FAILURE);
	while (*args)
	{
		ptrs[0] = *args;
		ptrs[2] = malloc(ft_strlen(*args) + 1);
		if (!ptrs[2])
			return (FAILURE);
		ptrs[1] = ptrs[2];
		while (*ptrs[0])
		{
			while (*ptrs[0] == '\\' && *(ptrs[0] + 1))
				ptrs[0]++;
			*ptrs[1]++ = *ptrs[0]++;
		}
		*ptrs[1] = 0;
		free(*args);
		*args = ft_strdup(ptrs[2]);
		free(ptrs[2]);
		if (!*args++)
			return (FAILURE);
	}
	return (SUCCESS);
}

void	remove_outer_quotes(char ***arr)
{
	int		i;
	int		j;
	size_t	len;
	char	ch;

	ch = 0;
	i = 0;
	while (arr && arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			len = ft_strlen(arr[i][j]);
			if (arr[i][j][0] == '\'')
				ch = '\'';
			else
				ch = '\"';
			if (len > 0 && (arr[i][j][0] == ch))
				ft_memmove((void *)arr[i][j], arr[i][j] + 1, len--);
			if (len > 0 && (arr[i][j][len - 1] == ch))
				arr[i][j][len - 1] = 0;
			j++;
		}
		i++;
	}
}

/* Simple unbuffered 1-char read until newline 
 * Caller must free passed buffer */
int	get_line(int fd, char **buf)
{
	int		i;
	int		b;
	int		bytes;

	*buf = malloc(sizeof(char) * BUFSZ);
	if (!*buf)
		return (ERROR);
	i = 0;
	bytes = 0;
	while (i < BUFSZ - 1)
	{
		b = 0;
		b = read(fd, (*buf) + i, 1);
		if (-1 == b)
		{
			free(*buf);
			return (ERROR);
		}
		bytes += b;
		if (0 == b || (*buf)[i] == '\n')
			break ;
		i++;
	}
	(*buf)[i] = '\0';
	return (bytes);
}

/* Finds which internal quotation mark in command
 * 		for separating command from args
 *
 * Some commands use single marks for their args
 */
void	get_split_delim(const char *arg, char *sub)
{
	if (ft_strchr(arg, '"') && ft_strchr(arg, '\''))
	{
		if (ft_strchr(arg, '"') < ft_strchr(arg, '\''))
			sub[1] = '"';
		else
			sub[1] = '\'';
	}
	else if (ft_strchr(arg, '"'))
		sub[1] = '"';
	else if (ft_strchr(arg, '\''))
		sub[1] = '\'';
	else if (ft_strchr(arg, '.') && (ft_strchr(arg, '.') != ft_strrchr(arg,
				'.')))
		sub[1] = '?';
	else
		sub[1] = 0;
}
