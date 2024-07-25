/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:08:09 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:08:10 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* STRNCMP
** Returns diff betw values of the first pair of differing bytes
** (as unsigned char) of s1 - s2, comparing no more than n bytes
*/

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	if (n == (size_t) - 1)
	{
		n = ~0;
	}
	if (n == 0 || (*s1 == 0 && *s2 == 0))
		return (0);
	if (*s1 != *s2)
		return (*s1 - *s2);
	while (--n > 0 && *s1 && *s2)
	{
		s1++;
		s2++;
		if (*s1 != *s2)
			return (*s1 - *s2);
	}
	if ((*s1 == 0) ^ (*s2 == 0))
		return (*s1 - *s2);
	return (0);
}
