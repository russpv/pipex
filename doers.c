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
	st->cmd_count = argc - (3 + st->heredoc); // assume t_bool counts as 1
	if (argc < (MINARGS + (int)st->heredoc))
		err("Insufficient arguments.", st, NULL, EINVAL);
	st->outfile = argv[argc - 1];
	st->path_offset = get_env_path(env);
	if (st->path_offset == FAILURE)
		err("Could not find PATH in env", st, NULL, 0);
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
			printf("Loaded path:%s", st->cmdpaths[idx]); fflush(stdout);
			if (!st->cmdpaths[idx])
				err("_load_cmdpath: strdup() failure", st, NULL, 0);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	perror("command not found.");
	//printf("loading NULL at idx:%d",idx); fflush(stdout);
	st->cmdpaths[idx] = NULL;
	return (EXIT_SUCCESS);
}

static void	_remove_outer_quotes(char ***arr)
{
	int i;
	int j;
	size_t len;
	char ch;

	printf("removing quotes|"); fflush(stdout);
	ch = 0;
	i = 0;
	while (arr && arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			len = ft_strlen(arr[i][j]);
			if (arr[i][j][0] == '\'')
				ch = '\'';
			else
				ch = '\"';
			if (len > 0 && (arr[i][j][0] == ch))
			{
				ft_memmove((void *)arr[i][j], arr[i][j] + 1, len);
				--len;
			}
			if (len > 0 && (arr[i][j][len - 1] == ch))
				arr[i][j][len - 1] = 0;
			j++;
		}
		i++;
	}
	printf("removed quotes|"); fflush(stdout);
}

/* Finds internal quotation mark in argv */
static void	_get_split_delim(const char *arg, char *sub)
{
	if (ft_strchr(arg, '\"'))
		sub[1] = '\"';
	else if (ft_strchr(arg, '\''))
		sub[1] = '\'';
	else
		sub[1] = 0;
	printf("delim_%s_ in %s|", sub, arg); fflush(stdout);
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
	//printf("load_cmdargs: sees %d cmds", st->cmd_count);
	st->execargs = malloc( sizeof(char **) * (st->cmd_count + 1));
	if (!st->execargs )
		err("_load_cmdargs()", st, NULL, 0);
	while (i < st->cmd_count)
	{
		//printf("splitting %s", argv[i + 2 + (int)st->heredoc]); fflush(stdout);
		_get_split_delim(argv[i + 2 + (int)st->heredoc], sub);
		printf("delim:_%s_|", sub); fflush(stdout);
		arr = ft_splitsub(argv[i + 2 + (int)st->heredoc], sub);
		st->execargs[i] = arr;
		if (!st->execargs[i])
			err("_load_cmdargs()", st, NULL, 0);
		//printf("loaded%d:", i); fflush(stdout); printarr(st->execargs[i]);
		i++;
	}
	st->execargs[st->cmd_count] = NULL;
	//printf("howdy"); fflush(stdout);
	_remove_outer_quotes(st->execargs);
	printf("_load_cmdargs:"); fflush(stdout); 
	printarrarr(st->execargs);
	printf("_"); fflush(stdout);
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
	//printf(":prep_execargs:loaded cmds"); fflush(stdout);
	if (status == FAILURE)
		err("_get_cmdargs()", st, NULL, 0);
	st->cmdpaths = malloc(sizeof(char *) * (st->cmd_count + 1));
	if (!st->cmdpaths)
		err("cmdpaths malloc", st, NULL, 0);
	st->cmdpaths[st->cmd_count] = NULL;
	i = -1;
	//printf(":prep_execargs: preloop"); fflush(stdout);
	while (++i < st->cmd_count) 
	{
		printf("Loading: %s_", st->execargs[i][0]); fflush(stdout);
		status = _load_cmdpath(i, st->execargs[i][0], paths, st);
		if (status == EXIT_FAILURE) //TODO not needed now
			err("_get_cmdpath()", st, NULL, 0);
	}
	free(paths);
	return (EXIT_SUCCESS);
}

/* returns offset for PATH, inits struct, loads execve args  */
int parse_args(int argc, char **argv, char **env, t_args *st)
{
	if (argc < MINARGS)
		err("Insufficient arguments.", NULL, NULL, EINVAL);
	_init_struct(argc, argv, env, st);
	//_load_cmd_count(argc, argv, st);
	_prep_execargs(argv, env, st);
	return (EXIT_SUCCESS);
}

