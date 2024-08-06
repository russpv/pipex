/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:13:36 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:13:37 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* MEMMOVE
** buffers can overlap
** buffers must be at least n in size
** return dest
** 1. tests if overlap ( 2 tests )
** 2. reverses copy sequence, one by one
** 3. arbitrarily returns NULL if both buffers are NULL
**
** Case we want to avoid:
** src: A | B | C
** dest:    A | B | C
** res:   | A | A | A
*/

static inline t_bool	overlap1(t_uint8_t *d, t_uint8_t *s, size_t n)
{
	if ((s + n) > d && s < d)
		return (1);
	else
		return (0);
}

static inline t_bool	overlap2(t_uint8_t *d, t_uint8_t *s, size_t n)
{
	if ((d + n) > s && d < s)
		return (1);
	else
		return (0);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	t_uint8_t	*d;
	t_uint8_t	*s;
	size_t		i;

	d = (t_uint8_t *)dest;
	s = (t_uint8_t *)src;
	i = 0;
	if (dest == NULL && src == NULL)
		return (NULL);
	if (!overlap1(d, s, n) && !overlap2(d, s, n))
		return (ft_memcpy(dest, src, n));
	if (overlap1(d, s, n))
		while (n--)
			d[n] = s[n];
	else
	{
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}
