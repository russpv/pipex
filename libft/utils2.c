/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 00:04:47 by rpeavey           #+#    #+#             */
/*   Updated: 2024/07/13 00:08:03 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	putchar_with_return(unsigned int ch)
{
	ft_putchar(ch);
	return (1);
}

/* Returns NULL if heap string args are NULL, else
** returns ptr to malloc
** s1: must be heap string
** s2: must be heap string
*/
char	*safe_mem_join(const char *s1, const char *s2, size_t len1, \
		size_t len2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
	{
		free((char *)s2);
		return (NULL);
	}
	else if (s1 && !s2)
	{
		free((char *)s1);
		return (NULL);
	}
	res = ft_memjoin(s1, s2, len1, len2);
	free((char *)s2);
	free((char *)s1);
	s2 = NULL;
	s1 = NULL;
	if (!res)
		return (NULL);
	return (res);
}

char	*to_upper(char *s)
{
	char	*tmp;

	tmp = s;
	while (*tmp)
	{
		*tmp = ft_toupper(*tmp);
		tmp++;
	}
	return (s);
}

/* Checks for specifier 
 * If char matches in any of set.
 */
int	in_set(const char *s, const char *set)
{
	if (!*s)
		return (FALSE);
	while (*set)
		if (*s == *set++)
			return (TRUE);
	return (FALSE);
}
