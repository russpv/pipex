/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:17:06 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:17:07 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* ISASCII
** Returns non-zero if 0-0177 inclusive 
*/

t_bool	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (TRUE);
	else
		return (FALSE);
}
