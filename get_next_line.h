/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:53:45 by rpeavey           #+#    #+#             */
/*   Updated: 2024/06/15 22:55:39 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define BLUE "\033[0;34m"
# define RESET "\033[0m"
# define YELLOW "\033[0;33m"

# define MAX_BUFFERS 1024
# define PAGESZ 65536

typedef enum e_exit_status
{
	PRELOOP,
	EOLPRELOOP,
	EOL,
	EOFILE,
	ERROR,
}					t_stat;

typedef struct s_llist
{
	char			*content;
	size_t			size;
	struct s_llist	*next;
}					t_lst;

struct s_vars
{
	char	*buf;
	char	(*stackbuf)[PAGESZ];
	size_t	bytes_read;
	t_stat	code;
	int		fd;
};

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

char		*get_next_line(int fd);

/*
 * UTILS
 */

void		*gnl_memchr(const void *ptr, int ch, size_t n);
void		gnl_memset(void *str, int c, size_t n);
void		*gnl_memcpy(void *dest, const void *src, size_t n);
void		remove_n_bytes(char *buf, size_t n);

#endif
