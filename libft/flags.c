/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:50:06 by rpeavey           #+#    #+#             */
/*   Updated: 2024/07/05 22:33:14 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Applies flags for STR and CHR
 * String:
 * - NULL is "(null)" unless minprec truncates,
	any minwidth prints "(null)" TODO: doublecheck
 * Char:
 * - '\0' is printed, so counts as len 1 for padding flags (and return value)
 * r: stack string
 * s: struct
 * typ: specifier
 * val: va_arg
 */
static inline char	*_do_flags_cs(char *r, t_spec *s, t_types typ, void *val)
{
	if (!r && (s->mpflg == TRUE && s->minprec < 6))
		r = ft_substr("", 0, 0);
	else if (!r && (s->mwflg || (s->mpflg == TRUE && s->minprec >= 6)))
		r = ft_substr("(null)", 0, 6);
	else if (r)
		r = ft_substr((char *)val, 0, ft_strlen((char *)val));
	if (!r)
		return (NULL);
	s->len = ft_strlen(r);
	if (typ == CHR && s->len == 0)
	{
		s->len = 1;
		s->nullchflg = TRUE;
	}
	if (s->mpflg && s->minprec < s->len)
		r = apply_minprecision_char(r, s);
	if (s->mwflg && s->minwidth > s->len)
		r = apply_minwidth(r, s);
	if (!r)
		return (NULL);
	return (r);
}

/* Apply flags for UINT/INT/HEX/PTR */
static inline char	*_do_flags_hipu(char *r, t_spec *s)
{
	if (s->mpflg)
		r = apply_minprecision_num(r, s);
	if (!r)
		return (NULL);
	s->len = ft_strlen(r) + s->leadflg + s->signflg
		* ft_strlen(s->sch);
	if (s->signflg && (s->minprec || s->pch != '0'))
	{
		s->signflg = FALSE;
		r = append_char(s->sch, r);
	}
	if (s->minwidth > s->len)
		r = apply_minwidth(r, s);
	if (s->leadflg)
		r = append_char(" ", r);
	if (s->signflg && !s->minprec)
		r = append_char(s->sch, r);
	if (!r)
		return (NULL);
	return (r);
}

/* Adds default hex prefix or returns heap "(nil)" for PTR
** r: must be heap string
*/
static inline char	*_apply_0x_nil(char *r)
{
	char	*tmp;

	tmp = safe_join(ft_strjoin("0x", ""), r);
	if (tmp)
	{
		if (ft_strncmp(tmp, "0x0", ft_strlen(tmp)) == 0)
		{
			free(tmp);
			r = ft_strjoin("(nil)", "");
		}
		else
			r = tmp;
	}
	if (!r)
		return (NULL);
	return (r);
}

/* Sets negative sign char and flag for printing INT
** 'Long long' container would be excessive; 
** va_arg() expects int only
*/
static void	_handle_neg(unsigned long long *value, void *val, t_spec *s)
{
	long long	num;
	long long	min_int;

	num = *(long long *)val;
	if (num < 0)
	{
		min_int = (int)1 << (sizeof(int) * 8 - 1);
		s->sch = "-";
		s->signflg = TRUE;
		s->leadflg = FALSE;
		if (num == min_int)
			*value = (unsigned long long)(~min_int) + 1;
		else
			*value = (unsigned long long)(-num);
	}
	else
		*value = (unsigned long long)num;
}

/* Switchboard for handling flags by arg specifier
** Returns heap string
** HEX, PTR, UINT are positive-only
** va_arg() expects unsigned long
*/
char	*type_switch(void *val, t_types typ, t_spec *specs)
{
	char				*res;
	unsigned long long	value;

	value = 0;
	if (typ == CHR || typ == STR)
		res = (char *)val;
	if (typ == CHR || typ == STR)
		res = _do_flags_cs(res, specs, typ, val);
	if (typ == INT)
		_handle_neg(&value, val, specs);
	if (typ == HEX || typ == PTR || typ == UINT)
		value = *(unsigned long *)val;
	if (typ == HEX || typ == INT || typ == PTR || typ == UINT)
		res = ft_itoa_base(value, specs->base);
	if (typ == PTR)
		res = _apply_0x_nil(res);
	if (typ == HEX || typ == INT || typ == PTR || typ == UINT)
		res = _do_flags_hipu(res, specs);
	if (!res)
		return (NULL);
	return (res);
}
