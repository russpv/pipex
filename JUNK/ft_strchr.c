/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:10:39 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:10:40 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* STRCHR
** Returns ptr to first occurence of char
** or NULL
** UNPROTECTED
** Note: gcc runs it faster w/o longword checking or independent
** loop instrucs
*/

char	*ft_strchr(const char *s, int c_in)
{
	const unsigned char	c = c_in;
	char				*p;

	p = (char *)s;
	while (1)
	{
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
	}
}
