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

/* Finds which internal quotation mark in command
 * for separating command from args for cmds
 * like awk.
 *
 * Some commands use single marks for their args
 * IGNORES splitting entirely if '.' present 
 * marking file extensions for custom scripts
 */
void	get_split_delim(const char *arg, char *sub)
{
	sub[1] = 0;
	if (ft_strchr(arg, '.'))
	{
		sub[0] = 0;
		return ;
	}
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
}
// NOTE: Functions below are deprecated. Complicated 
// arg parsing in cmd args (e.g. awk, "echo 'hello'") 
// will not be handled.
/*
static int	_has_single_quotes(char *s)
{
	
	if (!s)
		return (FAILURE);
	if (ft_strlen(s) < 3)
		return (FAILURE);
	if ('\'' == s[0] && '\'' == s[ft_strlen(s) - 1])
		return (SUCCESS);
	if (ft_strrchr(s, '\'') != ft_strchr(s,'\''))
		return (SUCCESS); 
	(void)s;
	return (FAILURE);
}
*/
/* Removes quotation marks around internal
 * arguments. Ex: echo 'hello' to echo hello */
/*
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
} */
/* 
 * Removes backslash escape chars from a single command string
 * and returns it by reference.
 * UNLESS the string is wrapped in single quotes
 * 
 * Note: Commands with escaped chars are already interpreted by 
 * the shell (bash/zsh) at this point. And that depends on 
 * single quote enclosure. No need to remove blackslashes.
 */
/*
int	process_string(char **args)
{
	char	*ptrs[3];

	if (!args)
		return (FAILURE);
	while (*args && FAILURE == _has_single_quotes(*args))
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
*/