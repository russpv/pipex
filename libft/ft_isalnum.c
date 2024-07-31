/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:02:32 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:02:35 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mylib.h"

/* ISALNUM
** Tests if char is alphanumeric 
*/

t_bool	ft_isalnum(int c)
{
	return (ft_isdigit(c) || ft_isalpha(c));
}
