/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pp_err.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:18:46 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/18 15:18:47 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* zsh error message 
 * This probably doesn't make it to tty
 * But I don't have a stderr func atm 
 */
int	permission_err(char *path, t_args *st, int idx)
{
	if (st)
		st->cmdpaths[idx] = NULL;
	ft_printf("Permission denied: %s", path);
	return (SUCCESS);
}

/* Prints error message and exits process safely.
 * Frees struct and any passed new string. 
 * Sets error number.
 */
void	err(const char *msg, t_args *st, void *str, int ern)
{
	if (ern != 0)
		errno = ern;
	perror(msg);
	if (st)
		cleanup(st);
	if (str)
		free_arr(str, 0);
	exit(EXIT_FAILURE);
}
