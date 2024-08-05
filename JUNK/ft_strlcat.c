/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:09:22 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/28 15:35:22 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* STRLCAT
** Appends src (c-string) to dest of size size (not length)
** null-terminates dest **as long as there is room**
** null-termination allowance must be included in size
** both args must be nul-terminated
** At most size - 1 chars are appended.
** Returns:
** LENGTH of string (not incl null) that would have been created
** so src + dest (always)
** It is not idiomatic in MY code to use -1 for size; however, it is
** PROTECTED here.
*/

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	destlen;
	size_t	srclen;
	size_t	space;

	if (size == (size_t)-1)
		size = ~0;
	srclen = ft_strlen(src);
	destlen = ft_strnlen(dest, size);
	if (destlen >= size || size == 0)
		return (destlen + srclen);
	if (destlen + srclen < size)
	{
		ft_memcpy(dest + destlen, src, srclen);
		dest[destlen + srclen] = '\0';
	}
	else
	{
		space = (size - 1) - destlen;
		ft_memcpy(dest + destlen, src, space);
		dest[destlen + space] = '\0';
	}
	return (destlen + srclen);
}
