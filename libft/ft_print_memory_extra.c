/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory_extra.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:27:57 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/31 18:55:14 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define WIDTH 16
#define PRINT 1
#define CONVERT 0

/* PRINT MEMORY
** This displays memory metadata for void *addr argument:
** address (hex) of first location
** values (hex) in first and subsequent locations separated by '  '
** values in printable characters, else '.'
** size == 0 means nothing displayed.
** Assume 'size' means number of bytes from addr.
** Assume return 'addr' is not modified. LAME - IT RETURNS addr+size
*/

/* Prints hex couplet for 2 decimal characters at location */
/* Or, converts remainder */
char	print_char_hex(void *ptr, int remainder, int type)
{
	char		c_value;
	const char	hexbase[] = "0123456789ABCDEF";

	if (type == PRINT)
	{
		c_value = *((char *)ptr);
		write(1, &hexbase[c_value / 16], 1);
		write(1, &hexbase[c_value % 16], 1);
		return (0);
	}
	if (type == CONVERT)
		return ((char)hexbase[remainder % 16]);
	return (0);
}

/* Read in hex memory address and print to screen with leading zeros */
void	print_addr(void *addr, unsigned int i, unsigned int size)
{
	char			address[20];
	char			*c;
	int				j;
	unsigned long	addrval;
	int				remainder;

	addrval = *(unsigned long *)&addr;
	j = 19;
	c = &address[19];
	if (i < size)
		write(1, "\n", 1);
	while (j >= 0)
	{
		remainder = addrval % 16;
		addrval = addrval / 16;
		*c = print_char_hex(c, remainder, CONVERT);
		c--;
		j--;
	}
	c = &address[0];
	write(1, c, 20);
	write(1, ":", 1);
}

/* Prints memory content at end of row */
/* Figures out to write full WIDTH or stop */
void	print_rest(unsigned char *endrow, unsigned int i, unsigned int size)
{
	const unsigned char	*startrow = endrow - WIDTH;
	int					j;
	int					rest;

	j = 0;
	if (i >= size)
		rest = WIDTH - (i - size);
	else
		rest = WIDTH;
	write(1, " ", 2);
	while (j < rest)
	{
		if (*(startrow + j) < ' ' || *(startrow + j) > '~')
			write(1, ".", 1);
		else
			write(1, startrow + j, 1);
		j++;
	}
}

void	print_stuff(unsigned int i, unsigned int size, unsigned char *ptr)
{
	if ((i + 1) % 2 != 0)
		write(1, " ", 2);
	if (i >= size)
		write(1, " ", 2);
	if (i < size)
		print_char_hex(ptr + i, 0, PRINT);
}

/* Main loop: declares ptr to move from addr; skips size == 0 */
/* i is for byte; j is for row; flag bypasses mod(0) issue */
void	*ft_print_memory(void *addr, unsigned int size)
{
	unsigned int	i;
	unsigned int	j;
	int				flag;

	j = 0;
	i = 0;
	while (j < size && size != 0)
	{
		print_addr(addr + i, i, size);
		flag = 1;
		while (i % WIDTH != 0 || flag == 1)
		{
			flag = 0;
			print_stuff(i, size, addr);
			i++;
		}
		if (i % WIDTH == 0)
		{
			print_rest(addr + i, i, size);
			j += WIDTH;
		}
	}
	return (addr);
}
