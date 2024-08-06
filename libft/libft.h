/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 20:18:27 by rpeavey           #+#    #+#             */
/*   Updated: 2024/06/19 20:29:49 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdarg.h>

typedef enum e_types
{
	INT,
	UINT,
	PTR,
	STR,
	HEX,
	CHR
}						t_types;

typedef struct s_spec
{
	char				*startpos;
	unsigned int		nullchflg;
	int					initflg;
	int					ljstflg;
	int					signflg;
	int					leadflg;
	int					mpflg;
	int					mwflg;
	int					altflg;
	unsigned int		base;
	char				pch;
	char				*sch;
	char				type;
	size_t				len;
	size_t				minwidth;
	size_t				minprec;
	size_t				padlen;
}						t_spec;
typedef enum e_bool
{
	FALSE,
	TRUE,
}					t_bool;
/* 1 byte */
typedef unsigned char	t_uint8_t;
/* 4 bytes */
typedef unsigned int	t_uint32_t;
/* 16 bytes */
typedef struct s_16
{
	t_uint8_t		data[16];
}	t_block_16;
/* 32 bytes */
typedef struct s_32
{
	t_uint8_t		data[32];
}	t_block_32;
typedef struct s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;
/* Must be at least 8-bytes */
typedef unsigned long long	(t_uint64_t);
/* 64-bit ptr */
typedef unsigned long long	(t_uintptr_t);

# define BIGBLOCKSZ 32
/* 16-bytes */
# define BLOCK 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFULL
# define BLOCKSZ 16
/* 8 bytes */
# define SUBBLOCK 0xFFFFFFFFFFFFFFFFUL
# define SUBBLOCKSZ 8
/* 8-byte ptr */
# define PTRSZ 8
# define ULSZ 4
# define FLAGS "#-+0 "
# define SPECS "cspduixX%"
# define SUCCESS 0
# define FAILURE -1
# define TRUE 1
# define FALSE 0

/*
 * Extras / TODO
 */

//void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
//void				ft_lstadd(t_list **alist, t_list *new);
size_t				ft_strnlen(const char *s, size_t maxlen);
char				*ft_strcpy(char *dest, const char *src);
char				*ft_strncpy(char *dest, const char *src, size_t n);
char				*ft_strcat(char *dest, const char *src);
char				*ft_strncat(char *dest, const char *src, size_t n);
void				*ft_memccpy(void *dest, const void *src, int c, size_t n);
//char				*ft_strstr(const char *str, const char *substr);
//char				*ft_strmap(char const *s, char (*f)(unsigned int, char));
//int					ft_strcmp(const char *s1, const char *s2);

/*
** Print functions
*/

void				ft_putchar(unsigned int c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
void				ft_putnbr(long n);
void				*ft_print_memory(void *addr, unsigned int size);

void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);

/*
** String functions
*/

size_t				ft_strlen(const char *s);
char				*ft_strdup(const char *src);
size_t				ft_strlcpy(char *dest, const char *src, size_t n);
size_t				ft_strlcat(char *dest, const char *src, size_t n);
char				*ft_strchr(const char *str, int ch);
char				*ft_strrchr(const char *str, int ch);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_toupper(int c);
int					ft_tolower(int c);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);

/*
** Memory manipulation
*/

void				*ft_memset(void *str, int c, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *ptr, int ch, size_t n);
int					ft_memcmp(const void *lhs, const void *rhs, size_t n);
void				*ft_calloc(size_t nelem, size_t elsize);
char				*ft_memdup(const char *s, size_t size);
void				ft_putmem(char const *s, size_t size);
char				*ft_memjoin(char const *s1, char const *s2, \
						size_t l1, size_t l2);

/*
** Tests
*/

//t_bool				ft_isupper(int c);
//t_bool				ft_islower(int c);
t_bool				ft_isalpha(int c);
t_bool				ft_isdigit(int c);
t_bool				ft_isalnum(int c);
t_bool				ft_isascii(int c);
t_bool				ft_isprint(int c);
t_bool				ft_isspace(int c);

/*
** Conversions
*/

int					ft_atoi(const char *str);
char				*ft_itoa(long long n);
char				*ft_itoa_base(unsigned long long n, unsigned int base);

/*
** List manipulation
*/

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
void				ft_lstadd_back(t_list **lst, t_list *new);
t_list				*ft_lstlast(t_list *lst);
int					ft_lstsize(t_list *lst);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

int					ft_printf(const char *s, ...);
char				*type_switch(void *val, t_types typ, t_spec *specs);
char				*repeat(size_t len, char ch);

/* Flags */
char				*safe_join(const char *s1, const char *s2);
char				*append_char(const char *ch, const char *r);
char				*apply_minwidth(char *r, t_spec *s);
char				*apply_minprecision_num(char *r, t_spec *s);
char				*apply_minprecision_char(char *r, t_spec *s);

/* Utils */
void				mark_init_and_advance(const char **s, t_spec *specs);
int					in_set(const char *s, const char *set);
unsigned int		print_output(void *res, t_spec *s);
char				*to_upper(char *s);
int					return_false_and_resetpos(const char **s, \
						t_spec *specs, unsigned int *bytes);
unsigned int		putchar_with_return(unsigned int ch);
char				*safe_mem_join(const char *s1, const char *s2, \
						size_t sz1, size_t sz2);

/* Format handlers */
void				do_idu_formats(const char *s, va_list *args,
						t_spec *specs, unsigned int *b);
void				do_cs_formats(const char *s, va_list args,
						t_spec *specs, unsigned int *b);
void				do_xx_formats(const char *s, va_list args,
						t_spec *specs, unsigned int *b);
void				do_pc(const char *s, unsigned int *b);

#endif
