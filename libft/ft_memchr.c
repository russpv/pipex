/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:14:16 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:14:17 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* MEMCHR
** Returns ptr to c if found, else NULL
** Tests 8-byte blocks with bitwise ops
** NOT PORTABLE to 32-bit systems
*/

#define MSK01 0x0101010101010101ULL
#define MSK80 0x8080808080808080ULL

static inline size_t	getmask(t_uint8_t c, size_t a)
{
	a = c;
	a |= a << 8;
	a |= a << 16;
	a |= a << 16;
	if (PTRSZ != 4)
		a |= a << 24;
	return (a);
}
/* check_block()
** Results in nonzero if any byte is zero
**
**   00001000 00000000
** m 00000001 00000001
** - 00000111 11111111
**   11110111 11111111
** & 00000111 11111111
** m 10000000 10000000
** & 00000000 10000000
**
** Unpacks first byte of mask a to get c
*/

static inline t_uint8_t	*check_block(t_uintptr_t const *b,
		size_t a)
{
	t_uint8_t	i;
	t_uint8_t	*byte;
	t_uint8_t	c;

	c = a & 0xFF;
	if ((((*b ^ a) - MSK01) & ~(*b ^ a) & MSK80) != 0)
	{
		i = 0;
		byte = (t_uint8_t *)b;
		while (i++ < PTRSZ)
			if (*byte++ == c)
				return (byte - 1);
	}
	return (NULL);
}

/* Checks eight 8-byte blocks */
static inline void	*unrolled_check(void const *p, size_t a)
{
	t_uintptr_t	*b;

	b = (t_uintptr_t *)p;
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	if (check_block(b++, a) != NULL)
		return (check_block(b - 1, a));
	return (NULL);
}

void	*helper(t_uint8_t **byte, size_t *n, size_t a)
{
	t_uintptr_t	*p;
	void		*res;
	size_t		chnk;

	p = (t_uintptr_t *)(*byte);
	chnk = *n / (8 * PTRSZ);
	while (chnk-- > 0)
	{
		res = unrolled_check(p, a);
		if (res != NULL)
			return (res);
		p += 8;
	}
	*n = *n % (8 * PTRSZ);
	while (*n / PTRSZ > 0)
	{
		res = check_block(p++, a);
		if (res != NULL)
			return (res);
		*n -= PTRSZ;
	}
	*byte = (t_uint8_t *)p;
	return (NULL);
}

/* Unrolls loops per scale of n 
** Iterates by casting s as 8 byte or 1 byte type
*/
void	*ft_memchr(void const *s, int c, size_t n)
{
	t_uint8_t	*byte;
	void		*res;
	size_t		a;

	byte = (t_uint8_t *)s;
	if (n > 2 * PTRSZ)
	{
		a = 0;
		a = getmask(c, a);
		while ((t_uintptr_t)byte % PTRSZ != 0 && n--)
		{
			if (*byte++ == (a & 0xFF))
				return (byte - 1);
		}
		res = helper(&byte, &n, a);
		if (res != NULL)
			return (res);
	}
	while (n > 0)
	{
		if (*byte++ == (unsigned char)c)
			return (byte - 1);
		n--;
	}
	return (NULL);
}
