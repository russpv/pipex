#include "pipex.h"

/* Takes each pipe command argument and does processing
** Removes backslash escape chars
** Caller must free array 
*/
int	process_string(char **args)
{
	char	*ptrs[3];

	if (!args)
		return (EXIT_FAILURE);
	while (*args)
	{
		ptrs[0] = *args;
		ptrs[2] = malloc(ft_strlen(*args) + 1);
		if (!ptrs[2])
			return (EXIT_FAILURE);
		ptrs[1] = ptrs[2];
		while (*ptrs[0])
		{
			while (*ptrs[0] == '\\' && *(ptrs[0] + 1))
				ptrs[0]++;
			*ptrs[1]++ = *ptrs[0]++;
		}
		*ptrs[1] = 0;
		free(*args);
		*args = ft_strdup(ptrs[2]);
		free(ptrs[2]);
		if (!*args++)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	remove_outer_quotes(char ***arr)
{
	int		i;
	int		j;
	size_t	len;
	char	ch;

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
				ft_memmove((void *)arr[i][j], arr[i][j] + 1, len--);
			if (len > 0 && (arr[i][j][len - 1] == ch))
				arr[i][j][len - 1] = 0;
			j++;
		}
		i++;
	}
}

/* simple fixed buffer version */
char	*get_line(int fd)
{
	char	*buf;
	char	*p;
	int		i;
	int		bytes;

	buf = malloc(sizeof(char) * BUFSZ);
	if (!buf)
		return (NULL);
	p = buf;
	i = 0;
	p[0] = '0';
	while (i < BUFSZ - 1)
	{
		bytes = read(fd, buf + i, 1);
		if (bytes == -1)
			err("get line read()", NULL, NULL, 0);
		if (bytes == 0 || buf[i] == '\0' || buf[i] == '\n')
			break ;
		i++;
	}
	buf[i] = '\n';
	return (buf);
}

/* Finds internal quotation mark in argv */
void	get_split_delim(const char *arg, char *sub)
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
	else if (ft_strchr(arg, '.') && (ft_strchr(arg, '.') != ft_strrchr(arg,
				'.')))
		sub[1] = '?';
	else
		sub[1] = 0;
}
