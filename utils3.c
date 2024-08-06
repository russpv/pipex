/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:32 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/05 17:12:32 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
