/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:09:35 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:09:38 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* STRJOIN
** Returns a new C-string of s1 and s2
** concatenation.
** UNPROTECTED
*/

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*s3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	*(s3 + len1 + len2) = 0;
	ft_memcpy(s3, s1, len1);
	ft_memcpy(s3 + len1, s2, len2);
	return (s3);
}
