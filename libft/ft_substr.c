/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:03:09 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:03:11 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* SUBSTR
** Returns heap C-string of part of s beginning at start
** for maximum of len (or len(s - start))
** start: index of the substring beginning in s
** len: length of substring
**
** PROTECTED: follows glib philosophy: caller responsible for null checking
** returns NULL for start > strlen(s) where others return empty string that
** prevents programmer from distinguishing valid from invalid input
** Copies up to and including the null terminator
*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	maxlen;

	if (s == NULL)
		return (NULL);
	if (start > ft_strlen(s))
		return (NULL);
	maxlen = ft_strlen(s) - start;
	if (maxlen < len)
		len = maxlen;
	sub = malloc(sizeof(char) * len + 1);
	if (!sub)
		return (NULL);
	sub[len] = 0;
	return (ft_memcpy(sub, s + start, len));
}
