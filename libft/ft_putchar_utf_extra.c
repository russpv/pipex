/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_utf_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:57:27 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/28 17:02:05 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_utf(unsigned int c)
{
	unsigned int	bytes;
	unsigned int	value;
	unsigned char	*ch;
	int				i;

	value = c;
	bytes = 0;
	while (value != 0)
	{
		value = value >> 8;
		bytes++;
	}
	ch = malloc(sizeof (char) * bytes + 1);
	if (!ch)
		return ;
	i = bytes;
	value = c;
	while (i-- > 0)
	{
		ch[i] = value & 0xFF;
		value = value >> 8;
	}
	ch[bytes] = '\0';
	write(1, ch, bytes);
}
