/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:32 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/20 18:24:56 by rpeavey          ###   ########.fr       */
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

void	init_struct(int argc, char **argv, char **env, t_args *st)
{
	st->outfile = "";
	st->argc = argc;
	st->cmdpaths = NULL;
	st->execargs = NULL;
	st->fd = 0;
	st->fildes = NULL;
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		st->heredoc = TRUE;
	else
		st->heredoc = FALSE;
	st->cmd_count = argc - (3 + st->heredoc);
	if (argc < (MINARGS + (int)st->heredoc))
		err("Insufficient arguments.", st, NULL, EINVAL);
	st->outfile = argv[argc - 1];
	st->path_offset = get_env_path(env);
	if (st->path_offset == FAILURE)
		err("Could not find PATH in env", st, NULL, 0);
}

void	cleanup_and_exit(t_args *st, int code)
{
	cleanup(st);
	exit(code);
}
