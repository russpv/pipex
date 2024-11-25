/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _main.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:11:58 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/25 12:57:26 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* PIPEX
** Expects args "[infile] 	[command]  ... 	[command] 	[outfile]"
** Like 		"[infile] < [command] |...| [command] > [outfile]"
**
** OR
** 		"./pipex here_doc LIMITER cmd ... cmd file"
** 	like	"cmd << LIMITER | ... | cmd >> file"
**
** Designed to mimic ZSH pipe behavior with FISH messages (more descriptive)
**
** Uses only: open, close, read, write, malloc, free, perror,
** 		access, dup, dup2, execve, exit, fork, pipe, wait, waitpid
**
** This scheme achieves CHILD-TO-CHILD communication
** Creates all pipes and launches all commands w/o
** 	waiting (i.e. in parallel)
**
** As a consequence, output redirection issues do not trigger exit
** if a heredoc is involved; and this doesn't check access in light 
** of TOCTTOU.
** Also, any command including a '.' is treated as local dir.
*/

int	main(int argc, char *argv[], char *env[])
{
	t_args	st;
	pid_t	p;
	int		i;
	int		status;

	parse_args(argc, argv, env, &st);
	create_pipes(&st);
	i = -1;
	while (++i < st.cmd_count)
	{
		p = fork();
		if (0 == p)
		{
			if (do_child_ops(i, argv, env, &st) == FAILURE)
				cleanup_and_exit(&st, 127);
		}
		else if (p < 0)
			err("Fork", &st, NULL, 0);
	}
	close_pipes(&st);
	waitchild(&status, &st);
	cleanup(&st);
	return (get_exit_status(status));
}
