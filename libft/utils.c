/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:50:21 by rpeavey           #+#    #+#             */
/*   Updated: 2024/07/05 22:31:37 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* init all but lengths */
static void	_init_specs(t_spec *specs, const char *s)
{
	specs->startpos = (char *)s;
	specs->nullchflg = FALSE;
	specs->initflg = TRUE;
	specs->ljstflg = FALSE;
	specs->signflg = FALSE;
	specs->leadflg = FALSE;
	specs->mpflg = FALSE;
	specs->mwflg = FALSE;
	specs->altflg = FALSE;
	specs->minwidth = 0;
	specs->minprec = 0;
	specs->base = 10;
	specs->type = '0';
	specs->sch = "";
	specs->pch = ' ';
	specs->padlen = 0;
	specs->len = 0;
}

/* To get around line limit, combines assignments and incrementation */
void	mark_init_and_advance(const char **s, t_spec *specs)
{
	(*s)++;
	_init_specs(specs, *s);
}

/* Reset s to startpos (one after '%') */
int	return_false_and_resetpos(const char **s, t_spec *specs, \
								unsigned int *bytes)
{
	*s = specs->startpos;
	specs->initflg = FALSE;
	bytes += putchar_with_return('%');
	return (FALSE);
}

/* Prints heap string to screen */
/* The only effect of altflg is "0x"*/
unsigned int	print_output(void *res, t_spec *s)
{
	size_t	len;

	if (!res)
	{
		ft_putstr("(null)");
		return (ft_strlen("(null)"));
	}
	else
	{
		len = s->len + s->padlen + (s->altflg * 2);
		ft_putmem(res, len);
		free(res);
	}
	s->initflg = FALSE;
	return (len);
}
