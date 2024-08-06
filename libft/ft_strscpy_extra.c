/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strscpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:07:03 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:07:10 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>

/* STRSCPY
** Copy a C-string into a sized buffer
** Returns # of chars copied (excl NULL) (E2BIG if buffer too small)
** Undefined behavior if buffers overlap
** Dest is always NULL terminated unless zero-sized
** Better than strlcpy since mem beyond count bytes is untouched, and
** sensible return value
** Superior to strncpy since no excessive NULL termination, always
** returns C-string
** Must be able to access src and dest within count limit
*/

size_t	strscopy(char *dest, const char *src, size_t count)
{
	size_t	len;

	if (count <= 0 || !*dest)
		return (-E2BIG);
	len = ft_strnlen(src, count - 1);
	ft_memcpy(dest, src, len);
	dest[len] = '\0';
	return (len);
}
