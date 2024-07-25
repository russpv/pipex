/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:11:27 by rpeavey           #+#    #+#             */
/*   Updated: 2024/05/27 20:11:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* This write to the passed file descriptor instead of STDOUT */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
