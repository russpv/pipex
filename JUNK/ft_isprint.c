/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:17:36 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:17:37 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* ISPRINT
** returns non-zero if printable char 
*/

t_bool	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
		return (TRUE);
	else
		return (FALSE);
}
