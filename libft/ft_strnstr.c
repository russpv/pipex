/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:05:08 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:05:09 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* STRNSTR
** Returns first occurrence of substring within n
** characters of s
** or s if sub is empty
** or NULL if not found
** UNPROTECTED: if NULL passed as s raises segfault
*/

char	*ft_strnstr(const char *s, const char *sub, size_t n)
{
	const char	*find;
	const char	*current;
	size_t		len;

	if (!*sub)
		return ((char *)s);
	if (n == (size_t) - 1)
		n = ~0;
	len = ft_strlen(sub);
	find = sub;
	while (*s && n-- >= len)
	{
		current = s;
		while (*current && *find && (*current == *find))
		{
			current++;
			find++;
		}
		if (!*find)
			return ((char *)current - len);
		find = sub;
		current = s;
		s++;
	}
	return (NULL);
}
