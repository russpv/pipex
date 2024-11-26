/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pp_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:18:58 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/18 15:19:03 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* If cmd path is accessible, loads it into struct
 * else exits safely.
 * If it is a built-in, it must have '-' '--' or quoted
 * options/args
 */
int	check_access(char *path, int idx, t_args *st)
{
	if (access((const char *)path, F_OK) == 0)
	{
		if (access((const char *)path, X_OK) != 0)
			return (permission_err(path, st, idx));
		st->cmdpaths[idx] = ft_strdup(path);
		if (!st->cmdpaths[idx])
			err("_load_cmdpath: strdup() failure", st, NULL, 0);
		return (SUCCESS);
	}
	return (FAILURE);
}

void	debug_print(const char *format, ...)
{
	va_list	args;

	if (DEBUG)
	{
		va_start(args, format);
		fprintf(stderr, "DEBUG: %d ", getpid());
		vfprintf(stderr, format, args);
		va_end(args);
	}
}

void	printerr(char *s)
{
	int			i;
	const int	len = ft_strlen(s);

	i = -1;
	while (++i < len)
		ft_putchar_fd(s[i], 2);
}
