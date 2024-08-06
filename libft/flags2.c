/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:49:59 by rpeavey           #+#    #+#             */
/*   Updated: 2024/06/27 15:50:00 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Returns NULL if heap string args are NULL, else \
** returns ptr to new string
** s1: must be heap string
** s2: must be heap string
*/
char	*safe_join(const char *s1, const char *s2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
	{
		free((char *)s2);
		return (NULL);
	}
	else if (s1 && !s2)
	{
		free((char *)s1);
		return (NULL);
	}
	res = ft_strjoin(s1, s2);
	free((char *)s2);
	free((char *)s1);
	s2 = NULL;
	s1 = NULL;
	if (!res)
		return (NULL);
	return (res);
}

/* Adds the sign char or lead char for HIPU
** ch: string literal of one char
** r: must be heap string
*/
char	*append_char(const char *ch, const char *r)
{
	char	*res2;

	res2 = ft_strjoin(ch, r);
	free((char *)r);
	if (!res2)
		return (NULL);
	return (res2);
}

/* Applies minimum precision to numerics
 * Truncates 0 to (blank) (except pointers' "(nil)")
 * Else adds prepadding with '0'
 * Returns heap string
 * r: must be heap string (0x..., )
 */
char	*apply_minprecision_num(char *r, t_spec *s)
{
	char	*res2;
	char	*tmp;

	if (!r)
		r = ft_strjoin("", "");
	if (s->minprec == 0 && (ft_strncmp(r, "0", 2) == 0))
	{
		free(r);
		r = ft_strjoin("", "");
	}
	if (ft_strlen(r) < s->minprec)
	{
		tmp = repeat(s->minprec - ft_strlen(r), '0');
		res2 = safe_join(tmp, r);
		if (!res2)
			return (NULL);
		return (res2);
	}
	return (r);
}

/* Applies minimum precision to strings by truncation
** Returns heap string
** r: must be heap string
*/
char	*apply_minprecision_char(char *r, t_spec *s)
{
	char	*res2;

	res2 = ft_substr(r, 0, s->minprec);
	free(r);
	if (!res2)
		return (NULL);
	s->len = ft_strlen(res2);
	return (res2);
}

/* Applies minimum width pre or post padding
** Returns heap string
** r: must be heap string
** padding on empty string or '\0' is valid
** left-just means padding goes to the right
*/
char	*apply_minwidth(char *r, t_spec *s)
{
	char	*tmp;

	s->padlen = s->minwidth - s->len;
	tmp = repeat(s->padlen, s->pch);
	if (s->nullchflg == TRUE && s->ljstflg == TRUE)
		r = safe_mem_join(r, tmp, s->len, s->padlen);
	else if (s->nullchflg == TRUE)
		r = safe_mem_join(tmp, r, s->padlen, s->len);
	else if (s->ljstflg == TRUE)
		r = safe_join(r, tmp);
	else
		r = safe_join(tmp, r);
	if (!r)
		return (NULL);
	return (r);
}
