/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doers.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:12:04 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/20 17:37:38 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Loads a single absolute command path into
** struct at index idx if accessible
** First loop searches PATH
** Second conditionals test local scripts
*/
static int	_load_cmdpath(int idx, char *cmd, const char **paths, t_args *st)
{
	int		i;
	char	fullpath[PATHBUF];

	i = -1;
	while (paths[++i] && ft_strncmp(cmd, ".", 1) && !ft_strchr(cmd, '/'))
	{
		ft_memset(fullpath, 0, PATHBUF);
		ft_memcpy(fullpath, paths[i], ft_strlen(paths[i]));
		if (fullpath[ft_strlen(fullpath) - 1] != '/')
			ft_strlcat((char *)fullpath, "/", ft_strlen(fullpath) + 2);
		ft_strlcat((char *)fullpath, cmd, ft_strlen(fullpath) + ft_strlen(cmd)
			+ 1);
		if (check_access(fullpath, idx, st) != FAILURE)
			return (EXIT_SUCCESS);
	}
	if (check_access(cmd, idx, st) != FAILURE)
		return (EXIT_SUCCESS);
	perror("PATH: command not found.");
	st->cmdpaths[idx] = NULL;
	return (EXIT_SUCCESS);
}

/* splits argv for use in execve() */
static int	_load_cmdargs(char **argv, t_args *st)
{
	int		i;
	char	**arr;
	char	sub[3];

	i = 0;
	sub[0] = ' ';
	sub[2] = 0;
	st->execargs = malloc(sizeof(char **) * (st->cmd_count + 1));
	if (!st->execargs)
		err("_load_cmdargs()", st, NULL, 0);
	while (i < st->cmd_count)
	{
		get_split_delim(argv[i + 2 + (int)st->heredoc], sub);
		arr = ft_splitsub(argv[i + 2 + (int)st->heredoc], sub);
		if (!arr)
			err("_load_cmdargs()", st, NULL, 0);
		process_string(arr);
		st->execargs[i] = arr;
		i++;
	}
	st->execargs[st->cmd_count] = NULL;
	remove_outer_quotes(st->execargs);
	return (EXIT_SUCCESS);
}

/* Parses PATH paths. Called once. */
static char	**_get_paths(char **env, t_args *st)
{
	const char			*path_str = env[st->path_offset];
	const char			*equal_pos = ft_strchr(path_str, '=');
	char				**res;
	char				*s;

	s = NULL;
	if (env == NULL || st == NULL || path_str == NULL)
		return (NULL);
	if (equal_pos != NULL)
		s = ft_substr(path_str, equal_pos - path_str + 1, -1);
	if (!s)
		err("PATH parsing issue.", st, NULL, 0);
	res = ft_split(s, ':');
	free(s);
	if (!res)
		return (NULL);
	return (res);
}

/* creates execve array argument in struct. Requires PATH. */
static int	_prep_execargs(char **argv, char **env, t_args *st)
{
	const char	**paths = (const char **)_get_paths(env, st);
	int			status;
	int			i;

	if (!paths)
		err("Could not split PATH", st, NULL, 0);
	status = _load_cmdargs(argv, st);
	if (status == FAILURE)
		err("Could not load command arguments", st, NULL, 0);
	st->cmdpaths = malloc(sizeof(char *) * (st->cmd_count + 1));
	if (!st->cmdpaths)
		err("Malloc error", st, NULL, 0);
	st->cmdpaths[st->cmd_count] = NULL;
	i = -1;
	while (++i < st->cmd_count)
		status = _load_cmdpath(i, st->execargs[i][0], paths, st);
	free_arr((void **)paths, 0);
	return (EXIT_SUCCESS);
}

/* returns offset for PATH, inits struct, loads execve args  */
int	parse_args(int argc, char **argv, char **env, t_args *st)
{
	if (argc < MINARGS)
		err("Insufficient arguments.", NULL, NULL, EINVAL);
	init_struct(argc, argv, env, st);
	_prep_execargs(argv, env, st);
	return (EXIT_SUCCESS);
}
