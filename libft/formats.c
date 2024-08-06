/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formats.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:50:14 by rpeavey           #+#    #+#             */
/*   Updated: 2024/07/05 21:57:36 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* NOTE: va_args() can only perform the "default argument promotions" of integer
 * and unsigned integer for unprototyped variadic args */

/* Defeat any invalid input based on type rules */
static void	_reset_specs(t_spec *specs, t_types type)
{
	if (type == INT)
		specs->sch = "+";
	if (type == HEX || type == PTR)
		specs->base = 16;
	if (type == CHR || type == HEX || type == PTR || type == UINT || \
		type == STR)
		specs->signflg = FALSE;
	if (type == CHR)
		specs->mpflg = FALSE;
	if (specs->mpflg == TRUE)
		specs->pch = ' ';
}

void	do_idu_formats(const char *s, va_list *args, t_spec *specs, \
		unsigned int *bytes)
{
	long long			value;
	unsigned long long	uvalue;
	unsigned long long	ptr;

	if (*s == 'i' || *s == 'd')
	{
		value = va_arg(*args, int);
		_reset_specs(specs, INT);
		*bytes += print_output(type_switch(&value, INT, specs), specs);
	}
	else if (*s == 'u')
	{
		uvalue = va_arg(*args, unsigned int);
		_reset_specs(specs, UINT);
		*bytes += print_output(type_switch(&uvalue, UINT, specs), specs);
	}
	else if (*s == 'p')
	{
		ptr = (unsigned long long)va_arg(*args, void *);
		_reset_specs(specs, PTR);
		*bytes += print_output(type_switch(&ptr, PTR, specs), specs);
	}
	else
		return ;
}

void	do_cs_formats(const char *s, va_list args, t_spec *specs, \
		unsigned int *bytes)
{
	char	*string;
	char	*res;
	char	c;

	res = NULL;
	if (*s == 'c')
	{
		c = (char)va_arg(args, int);
		_reset_specs(specs, CHR);
		*bytes += print_output(type_switch(&c, CHR, specs), specs);
	}
	else if (*s == 's')
	{
		string = va_arg(args, char *);
		_reset_specs(specs, STR);
		*bytes += print_output(type_switch(string, STR, specs), specs);
	}
	else
		return ;
}

/* '%#x' when x is 0 is 0 */
void	do_xx_formats(const char *s, va_list args, t_spec *specs, \
		unsigned int *bytes)
{
	unsigned long long	num;
	char				*res;

	num = 0;
	res = NULL;
	if (*s != 'X' && *s != 'x')
		return ;
	num = va_arg(args, unsigned int);
	_reset_specs(specs, HEX);
	res = type_switch(&num, HEX, specs);
	if (res == NULL)
	{
		res = "(null)";
		*bytes += print_output(res, specs);
		return ;
	}
	if (ft_strncmp(res, "0", 1) == 0)
		specs->altflg = 0;
	if (specs->altflg == TRUE && ft_strncmp(res, "0", 1) != 0)
		res = safe_join(ft_strjoin("0x", ""), res);
	if (*s == 'X')
		res = to_upper(res);
	*bytes += print_output(res, specs);
}

void	do_pc(const char *s, unsigned int *bytes)
{
	if (*s == '%')
		*bytes += putchar_with_return('%');
	return ;
}
