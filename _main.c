/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _main.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42singapore.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:11:58 by rpeavey           #+#    #+#             */
/*   Updated: 2024/11/16 21:47:03 by rpeavey          ###   ########.fr       */
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
 *
 * Creates all pipes and launches all commands w/o
 * waiting (i.e. in parallel).
 *
 * Waits for report from kernel for all children before returning
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
			{ 	debug_print("error with ops\n");
				cleanup_and_exit(&st, 127);}
		}
		else if (p < 0)
			err("Fork", &st, NULL, 0);
	}
	debug_print("waiting for kids\n");
	close_pipes(&st);
	waitchild(&status, &st);
	cleanup(&st);
	return (get_exit_status(status));
}
