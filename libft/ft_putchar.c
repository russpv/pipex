/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:11:15 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/28 16:58:04 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* PUTCHAR 
** Displays c on stdout.
*/
void	ft_putchar(unsigned int c)
{
	write(1, &c, 1);
}
