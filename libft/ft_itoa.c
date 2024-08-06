/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:16:24 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/05 19:00:00 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define DIGITS 20

/* ITOA
** Returns new C-string representing n value
*/

/* Moves backwards along pre-terminated fixed buffer */
static inline char	*load_str(char *s, long long n)
{
	int	sign;

	sign = 0;
	if (n < 0)
	{
		n *= -1;
		sign = 1;
	}
	if (n == 0)
		*s-- = '0';
	while (n > 0)
	{
		*s-- = n % 10 + '0';
		n = n / 10;
	}
	if (sign)
		*s-- = '-';
	return (++s);
}

char	*ft_itoa(long long num)
{
	char	c[DIGITS];
	char	*s;
	char	*dest;

	ft_memset(&c, '\0', DIGITS);
	s = load_str(c + (DIGITS - 2), num);
	dest = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!dest)
		return (NULL);
	dest[ft_strlen(s)] = 0;
	dest = ft_memcpy(dest, s, ft_strlen(s));
	return (dest);
}
