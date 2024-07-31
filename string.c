#include "pipex.h"

/* Takes each pipe command argument and does processing */
/* Removes backslash escape chars */
/* Caller must free array */
int	process_string(char **args)
{
	char **arr;
	char *src;
	char *dest;
	char *buf;

	if (!args)
		return (EXIT_FAILURE);
	arr = args;
	while (*arr)
	{
		buf = malloc(ft_strlen(*arr) + 1);
		if (!buf)
			return (EXIT_FAILURE);
		dest = buf;
		src = *arr;
		while (*src)
		{
			while (*src == '\\' && *(src + 1))
				src++;
			*dest++ = *src++;
		}
		*dest = 0;
		free(*arr);
		*arr = ft_strdup(buf);
		if (!*arr)
			return (EXIT_FAILURE);
		free(buf);
		arr++;
	}
	return (EXIT_SUCCESS);
}

void	remove_outer_quotes(char ***arr)
{
	int i;
	int j;
	size_t len;
	char ch;

	//printf("removing quotes|"); fflush(stdout);
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
	//ft_printf("removed quotes|");
}

