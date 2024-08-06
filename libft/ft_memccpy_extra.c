/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:14:28 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:42:19 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* MEMCCPY
** Stops copying if c is copied or until n bytes
** Undefined if dest/src are null, overlap, or access beyond end
** Returns null or ptr to byte after copied c in dest
** e.g. memccpy(bigS, "James", '\0', sizeof bigS - 1);
** Since each byte must be checked, no parallelism
*/

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	const unsigned char	*s = (const unsigned char *)src;
	unsigned char		*d;

	d = (unsigned char *)dest;
	while (n)
	{
		if (*s == c)
			return (++d);
		*d++ = *s++;
		n--;
	}
	return (NULL);
}
