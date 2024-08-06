/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 21:10:09 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 21:10:12 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* STRRCHR
** Returns last occurrence of char (incl '\0')
** or NULL if not found
*/

char	*ft_strrchr(const char *s, int c_in)
{
	const unsigned char	c = (const unsigned char)c_in;
	size_t				n;
	const char			*e;

	n = ft_strlen(s) + 1;
	e = s + n - 1;
	while (n >= 4)
	{
		if (*e-- == c)
			return ((char *)e + 1);
		if (*e-- == c)
			return ((char *)e + 1);
		if (*e-- == c)
			return ((char *)e + 1);
		if (*e-- == c)
			return ((char *)e + 1);
		n -= 4;
	}
	while (n--)
		if (*e-- == c)
			return ((char *)e + 1);
	return (NULL);
}
