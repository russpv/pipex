#include "pipex.h"

static void _init_struct(int argc, char **argv, char **env, t_args *st)
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

/* zsh error message */
static int	permission_err(char *path, t_args *st, int idx)
{
	if (st)
		st->cmdpaths[idx] = NULL;
	ft_printf("permission denied: %s", path);
	return (EXIT_SUCCESS);
}
/* Loads a single absolute command path into struct at index idx if accessible */
static int	_load_cmdpath(int idx, char *cmd, const char **paths, t_args *st)
{
	int i;
	char fullpath[PATHBUF];

	i = -1;
	while (paths[++i] && ft_strncmp(cmd, ".", 1) && !ft_strchr(cmd, '/'))
	{
		ft_memset(fullpath, 0, PATHBUF);
		ft_memcpy(fullpath, paths[i], ft_strlen(paths[i]));
		if (fullpath[ft_strlen(fullpath) - 1] != '/')
			ft_strlcat((char *)fullpath, "/", ft_strlen(fullpath) + 2);
	    ft_strlcat((char *)fullpath, cmd, ft_strlen(fullpath) + ft_strlen(cmd) + 1);
		if (access((const char *)fullpath, F_OK) == 0)
		{
			if (access((const char *)fullpath, X_OK != 0))
				return (permission_err(fullpath, st, idx));
			st->cmdpaths[idx] = ft_strdup((const char*)fullpath);
			if (!st->cmdpaths[idx])
				err("_load_cmdpath: strdup() failure", st, NULL, 0);
			return (EXIT_SUCCESS);
		}
	}
	if (access((const char *)cmd, F_OK) == 0 && access((const char*)cmd, X_OK) != 0)
		return (permission_err(cmd, st, idx));
	if (access((const char *)cmd, X_OK) == 0)
	{
		st->cmdpaths[idx] = ft_strdup(cmd);
		return (EXIT_SUCCESS);
	}
	perror("PATH: command not found.");
	st->cmdpaths[idx] = NULL;
	return (EXIT_SUCCESS);
}

/* Finds internal quotation mark in argv */
static void	_get_split_delim(const char *arg, char *sub)
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
	else if (ft_strchr(arg, '.') && (ft_strchr(arg, '.') != ft_strrchr(arg, '.')))
		sub[1] = '?';
	else
		sub[1] = 0;
}

/* splits argv for use in execve() */
static int	_load_cmdargs(char **argv, t_args *st)
{
	int i;
	char **arr;
	char sub[3];

	i = 0;
	sub[0] = ' ';
	sub[2] = 0;
	st->execargs = malloc( sizeof(char **) * (st->cmd_count + 1));
	if (!st->execargs )
		err("_load_cmdargs()", st, NULL, 0);
	while (i < st->cmd_count)
	{
		_get_split_delim(argv[i + 2 + (int)st->heredoc], sub);
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
static char **_get_paths(char **env, t_args *st)
{
	const char *haystack = "PATH=";
	const unsigned int start = ft_strchr(haystack, '=') - haystack + 1;
	char **res;
	char *s;

	s = ft_substr(env[st->path_offset], start, -1);
	if (!s)
		return (NULL);
	res = ft_split(s, ':');
	free(s);
	if (!res) 
		return (NULL);
	return (res);
}

/* creates execve array argument in struct. Requires PATH. */
static int	_prep_execargs(char **argv, char **env, t_args *st)
{
	const char **paths = (const char **)_get_paths(env, st);
	int status;
	int i;

	if (!paths)
		err("Could not split PATH", st, NULL, 0);
	status =_load_cmdargs(argv, st);
	if (status == FAILURE)
		err("Could not load command arguments", st, NULL, 0);
	st->cmdpaths = malloc(sizeof(char *) * (st->cmd_count + 1));
	if (!st->cmdpaths)
		err("Malloc error", st, NULL, 0);
	st->cmdpaths[st->cmd_count] = NULL;
	i = -1;
	while (++i < st->cmd_count) 
		status = _load_cmdpath(i, st->execargs[i][0], paths, st);
	free(paths);
	return (EXIT_SUCCESS);
}

/* returns offset for PATH, inits struct, loads execve args  */
int parse_args(int argc, char **argv, char **env, t_args *st)
{
	if (argc < MINARGS)
		err("Insufficient arguments.", NULL, NULL, EINVAL);
	_init_struct(argc, argv, env, st);
	_prep_execargs(argv, env, st);
	return (EXIT_SUCCESS);
}

