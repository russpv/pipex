#include "pipex.h"

/* searches **env for "PATH" and returns index */
int	get_env_path(char **env)
{
	const char *path = "PATH";
	char *s;
	int i;
	int idx;

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
	return (FAILURE);
}

static void _init_struct(t_args *st)
{
	st->argc = 0;
	st->cmd_count = 0;
	st->path_offset = 0;
	st->cmdpaths = NULL;
	st->execargs = NULL;
	st->fd = 0;
	st->fildes[0] = -1;
	st->fildes[1] = -1;
	st->fildes2[0] = -1;
	st->fildes2[1] = -1;

}

/* Loads a single absolute command path into struct at index idx if accessible */
static int	_load_cmdpath(int idx, char *cmd, const char **paths, t_args *st)
{
	unsigned int i;
	char fullpath[PATHBUF];

	i = 0;
	while (paths[i])
	{
		ft_memset(fullpath, 0, PATHBUF);
		ft_memcpy(fullpath, paths[i], ft_strlen(paths[i]));
		ft_strlcat((char *)fullpath, "/", ft_strlen(fullpath) + 2);
	    ft_strlcat((char *)fullpath, cmd, ft_strlen(fullpath) + ft_strlen(cmd) + 1);
		if (access((const char *)fullpath, X_OK) == 0)
		{
			st->cmdpaths[idx] = ft_strdup((const char*)fullpath);
			if (!st->cmdpaths[idx])
				err("Command not found.", st, NULL);
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

/* splits argv for use in execve() */
static int	_load_cmdargs(int argc, char **argv, t_args *st)
{
	int i;

	i = 0;
	st->execargs = malloc( sizeof(char **) * (st->cmd_count + 1));
	if (!st->execargs)
		err("_load_cmdargs()", st, NULL);
	while (argv[i + 2])
	{
		st->execargs[i] = ft_split(argv[i + 2], ' ');
		//printf("peeld off %d:_%s,%s\n", i, st->execargs[i][0], st->execargs[i][1]); fflush(stdout);
		if (!st->execargs[i])
			err("_load_cmdargs()", st, NULL);
		i++;
	}
	st->execargs[st->cmd_count] = NULL;
	//printf("_load_cmdargs:"); fflush(stdout); 
	//printarrarr(st->execargs);
	return (SUCCESS);
}

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
static int	_prep_execargs(int argc, char **argv, char **env, t_args *st)
{
	const char **paths = (const char **)_get_paths(env, st);
	int status;
	int i;

	if (!paths)
		err("Could not split PATH", st, NULL);
	status =_load_cmdargs(argc, argv, st);
	if (status == FAILURE)
		err("_get_cmdargs()", st, NULL);
	st->cmdpaths = malloc(sizeof(char *) * (st->cmd_count + 1));
	if (!st->cmdpaths)
		err("cmdpaths malloc", st, NULL);
	st->cmdpaths[st->cmd_count] = NULL;
	i = -1;
	while (++i < st->cmd_count) 
	{
		status = _load_cmdpath(i, st->execargs[i][0], paths, st);
		if (status == FAILURE)
			err("_get_cmdpath()", st, NULL);
	}
	free(paths);
	return (FAILURE);
}

static int	_load_cmd_count(int argc, t_args *st)
{
	st->cmd_count = argc - 2;
	return (SUCCESS);
}

/* returns offset for PATH, inits struct, loads execve args  */
int parse_args(int argc, char **argv, char **env, t_args *st)
{
	if (argc < MINARGS)
		err("Insufficient arguments.", st, NULL);
	_init_struct(st);
	_load_cmd_count(argc, st);
	st->path_offset = get_env_path(env);
	if (st->path_offset == FAILURE)
		err("Could not find PATH in env", st, NULL);
	_prep_execargs(argc, argv, env, st);
	return (SUCCESS);
}
