/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:13:01 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/08 17:57:36 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define MINARGS 5
# define PATHBUF 1024
# define BUFSZ 1024
# define LIMIT 10000
# define NO_APND false
# define APPEND true

typedef struct s_args
{
	int		argc;
	char	**envp;

	int		cmd_count;
	int		path_offset;
	char	**cmdpaths;
	char	***execargs;

	int		fd;
	int		**fildes;

	char	*outfile;

	t_bool	heredoc;
}			t_args;

typedef struct s_cnxn
{
	int		from_fd;
	int		to_fd;
	char	*topath;
	int		ifappend;
}			t_cnxn;

/* parser */
int				parse_args(int argc, char **argv, char **env, t_args *st);

/* pipes */
int				redirect(int *to, char *topath, int from, t_bool append);
void			create_pipes(t_args *st);
int				get_exit_status(int status);

/* string - used by parser */
int				process_string(char **arr);
void			remove_outer_quotes(char ***arr);
unsigned int	get_line(int fd, char **buf);
void			get_split_delim(const char *arg, char *sub);

/* utils */
int				check_access(char *path, int idx, t_args *st);

/* init */
void			init_struct(int argc, char **argv, char **env, t_args *st);

/* frees */
void			free_arr(void **arr, int size);
void			cleanup(t_args *st);
void			cleanup_and_exit(t_args *st, int code);

/* err */
void			err(const char *msg, t_args *st, char *str, int ern);
int				permission_err(char *path, t_args *st, int idx);

/* Lib */
char			**ft_splitsub(const char *s, const char *sub);

#endif
