/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:16:24 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/05 18:53:20 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

#define DIGITS 65

/* ITOA BASE (UNSIGNED)
** Returns new C-string representing POSITIVE n value in bases 2-20
** 64 digits (+null) should represent a long long signed binary number
*/

/* Moves backwards along pre-terminated fixed buffer 
** Adapted to handle unsigned only. 
*/
static inline char	*load_str(char *s, unsigned long long n, unsigned int base)
{
	const char	*radix = "0123456789abcdefghijk";

	if (n == 0)
		*s-- = '0';
	while (n > 0)
	{
		*s-- = radix[n % base];
		n /= base;
	}
	return (++s);
}

/* Goes to the end of the char buffer and translates the char to digit 
 */
char	*ft_itoa_base(unsigned long long num, unsigned int base)
{
	char	buf[DIGITS];
	char	*s;
	char	*dest;

	ft_memset(&buf, '\0', DIGITS);
	s = load_str(buf + (DIGITS - 2), num, base);
	dest = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s, ft_strlen(s));
	dest[ft_strlen(s)] = 0;
	return (dest);
}
