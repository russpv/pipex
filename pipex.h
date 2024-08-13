/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:13:01 by rpeavey           #+#    #+#             */
/*   Updated: 2024/08/13 16:51:59 by rpeavey          ###   ########.fr       */
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

# define SUCCESS 0
# define FAILURE -1
# define MINARGS 5
# define PATHBUF 1024
# define BUFSZ 1024
# define LIMIT 10000

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

int			parse_args(int argc, char **argv, char **env, t_args *st);

/* Utils.c */
int			redirect(int *to, char *topath, int from, t_bool append);
void		create_pipes(t_args *st);
int			get_exit_status(int status);
int			get_env_path(char **env);
void		err(const char *msg, t_args *st, char *str, int ern);
int			permission_err(char *path, t_args *st, int idx);
void		cleanup(t_args *st);
void		init_struct(int argc, char **argv, char **env, t_args *st);
int			check_access(char *path, int idx, t_args *st);
void		free_arr(void **arr);

/* Lib */
char		**ft_splitsub(const char *s, const char *sub);

/* String.c */
int			process_string(char **arr);
void		remove_outer_quotes(char ***arr);
char		*get_line(int fd);
void		get_split_delim(const char *arg, char *sub);

void	list_open_fds();
#endif
