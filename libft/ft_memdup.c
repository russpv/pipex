/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:10:02 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:10:03 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* MEMDUP
** This returns pointer to new malloc
** malloc handles errno
*/

char	*ft_memdup(const char *s, size_t size)
{
	char	*newmem;

	newmem = (char *)malloc(sizeof(char) * size);
	if (!newmem)
		return (NULL);
	return (ft_memcpy(newmem, s, size));
}
