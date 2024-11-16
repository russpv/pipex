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

/* searches **env for "PATH" and returns index */
static int	_get_env_path(char **env)
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
	return (FAILURE);
}

/* Creates app struct or exits safely */
void	init_struct(int argc, char **argv, char **env, t_args *st)
{
	st->outfile = "";
	st->argc = argc;
	st->cmdpaths = NULL;
	st->execargs = NULL;
	st->fd = 0;
	st->fildes = NULL;
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		st->heredoc = true;
	else
		st->heredoc = false;
	st->cmd_count = argc - (3 + st->heredoc);
	if (argc < (MINARGS + (int)st->heredoc))
		err("Init: Insufficient arguments.", st, NULL, EINVAL);
	st->outfile = argv[argc - 1];
	st->path_offset = _get_env_path(env);
	if (st->path_offset == FAILURE)
		err("Could not find PATH in env", st, NULL, 0);
}
