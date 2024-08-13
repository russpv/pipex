/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:22 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/13 15:28:39 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* zsh error message */
int	permission_err(char *path, t_args *st, int idx)
{
	if (st)
		st->cmdpaths[idx] = NULL;
	ft_printf("permission denied: %s", path);
	return (EXIT_SUCCESS);
}

void	err(const char *msg, t_args *st, char *str, int ern)
{
	if (ern != 0)
		errno = ern;
	perror(msg);
	if (st)
		cleanup(st);
	if (str)
		free(str);
	exit(EXIT_FAILURE);
}

void	free_arr(void **arr)
{
	void ** ptr = arr;
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(arr);
}

static void	_free_arrarr(void ***arrarr)
{
	void ***ptr = arrarr;
	while (*ptr)
	{
		free_arr(*ptr);
		ptr++;
	}
	free(arrarr);
}

void	cleanup(t_args *st)
{
	if (st->cmdpaths)
		free_arr((void**)st->cmdpaths);
	if (st->execargs)
		_free_arrarr((void***)st->execargs);
	if (st->fildes)
		free_arr((void **)st->fildes);
	return ;
}
