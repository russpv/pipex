/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:11:53 by rpeavey           #+#    #+#             */
/*   Updated: 2024/06/18 16:40:00 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* This prints integers.
** GCC has a problem with writeing more than 1 byte
*/
void	ft_putnbr(long num)
{
	if (num == (long)1 << (sizeof(long) * 8 - 1))
	{
		write(1, "-9223372036854775808", 20);
	}
	if (num < 0)
	{
		write(1, "-", 1);
		num *= -1;
	}
	if (num >= 10)
	{
		ft_putnbr(num / 10);
		ft_putnbr(num % 10);
	}
	if (num < 10)
		write(1, &(char){num + '0'}, 1);
}
