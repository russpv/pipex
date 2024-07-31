/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:15:31 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:15:32 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mylib.h"

/* LSTDELONE
** Removes passed node
** 1st: node to free
** del: ptr to func that deletes node.content
** UNPROTECTED Does not check del
*/

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
	lst = NULL;
}
