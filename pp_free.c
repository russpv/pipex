/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:22 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/20 17:43:16 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_arr(void **arr, int size)
{
	void	**ptr;
	int		i;

	i = -1;
	ptr = arr;
	if (size)
	{
		while (++i < size)
			if (ptr[i])
				free(ptr[i]);
	}
	else
	{
		if (*ptr)
		{
			while (*ptr)
			{
				free(*ptr);
				ptr++;
			}
		}
	}
	free(arr);
}

static void	_free_arrarr(void ***arrarr)
{
	void	***ptr;

	ptr = arrarr;
	while (*ptr)
	{
		free_arr(*ptr, 0);
		ptr++;
	}
	free(arrarr);
}

void	cleanup(t_args *st)
{
	if (st->cmdpaths && st->cmd_count)
		free_arr((void **)st->cmdpaths, st->cmd_count);
	if (st->execargs)
		_free_arrarr((void ***)st->execargs);
	if (st->fildes)
		free_arr((void **)st->fildes, 0);
	return ;
}

/* Line-saving two-fer for child process exception safe exit */
void	cleanup_and_exit(t_args *st, int code)
{
	cleanup(st);
	exit(code);
}