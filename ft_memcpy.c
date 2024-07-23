/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:13:47 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:13:48 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* MEMCPY
** Copies src into dest; tries to do it in blocks
** Undefined if mem regions overlap.
** Things added to satisfy arbitrary test cases:
** - return NULL if passed BOTH NULLs
** NOT PORTABLE. For 64-bit systems only.
*/

/* Test if less than 16 bytes */
static inline t_bool	too_lil(size_t n)
{
	return (n < BLOCKSZ);
}

/* Test if cpy sources are not aligned to 32 bit boundaries */
static inline t_bool	misalign(void *dest, const void *src)
{
	return (((sizeof(long) - 1) & (long)dest) | ((sizeof(long)
				- 1) & (long)src));
}

/* Writes 4 bytes per instruction up to at least  16 bytes */
static inline size_t	do_loops(char **dst, const char **s, size_t n)
{
	long		*aligned_dst;
	const long	*aligned_src;

	aligned_dst = (long *)*dst;
	aligned_src = (long *)*s;
	while (n >= BLOCKSZ)
	{
		*aligned_dst++ = *aligned_src++;
		*aligned_dst++ = *aligned_src++;
		n -= BLOCKSZ;
	}
	while (n >= SUBBLOCKSZ)
	{
		*aligned_dst++ = *aligned_src++;
		n -= SUBBLOCKSZ;
	}
	*dst = (char *)aligned_dst;
	*s = (char *)aligned_src;
	return (n);
}

/* Writes 1 byte at a time if less than 16 bytes, else 4 bytes at a time */
/* Doesn't align addresses */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*dst;
	const char	*s;

	if (dest == NULL && src == NULL)
		return (NULL);
	dst = dest;
	s = src;
	if (!too_lil(n) && !misalign(dest, src))
		n = do_loops(&dst, &s, n);
	while (n--)
		*dst++ = *s++;
	return (dest);
}
