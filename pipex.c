#include "pipex.h"

/* PIPEX
 * Expects "[infile] [command] | ... | [command] > [outfile]" args
 *
*/

/* arg 2: filepath; arg 3: command; ... ; argv[argc - 1] filepath */
/* fork() spawns deep copies of vars that point to same virtual mem addresses */
/* pipe(fildes[2]) puts read on fildes[0] */

// make a basic pipe. don't need symbols
// read arg as infile, then outfile
// getargcount() ... parse input
// how to create a pipe with PIPE?

/* Writes infd to fresh outname file */
/*
static void    _writeout(char *outname, int infd)
{
    int bytes_read;
    int bytes_written;
    int buffer[BUFSZ];
	int total_bytes_read;

    ft_memset(buffer, 0, BUFSZ);
    int outfd = open(outname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfd == -1)
		err("open", NULL, NULL, 0);
	bytes_read = 1;
	total_bytes_read = 0;
	while (bytes_read > 0 && total_bytes_read < LIMIT)
    {
		bytes_read = read(infd, buffer, BUFSZ);
        bytes_written = write(outfd, buffer, bytes_read);
        if (bytes_written != bytes_read)
            err("writeout", NULL, NULL, 0);
		total_bytes_read += bytes_read;
    }
}
*/
/* Wraps open() and dup2() */
/* 1. Opens input file ror reading
 * 2. Opens output file for writing
 * fromfile is only ever stdin or stdout
 */
static int _redirect(int *to, char *topath, int from)
{
    int fd;
    
	if (!to && (topath != NULL && *topath))
	{
		//printf("|redirect:%s_from_%d|", topath, from); fflush(stdout);
		if (from == STDIN_FILENO && access(topath, F_OK) == -1) // bad cmd last round
		{
			printf("|devnull|"); fflush(stdout);
			fd = open("/dev/null", O_RDONLY);
		}
		else if (from == STDIN_FILENO)
        	fd = open(topath, O_RDONLY | O_CREAT, 0644);
		else if (from == STDOUT_FILENO)
			fd = open(topath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = -1 ;
		if (fd < 0)
            err("redirect() open", NULL, NULL, 0);
	}
    else
	{
        fd = *to;
		//printf("|redirect:%d_to_%d|", fd, from); fflush(stdout);
	}
    if (dup2(fd, from) == -1)
        err("dup2", NULL, NULL, 0);
    close(fd);
    return (from);
}

static void	_heredoc(char *lim)
{
	int fildes[2];
	pid_t p;
	char *userinput;
	
	//printf("We in heredoc|"); fflush(stdout);
	userinput = NULL;
	if (pipe(fildes) < 0)
		err("heredoc pipe()", NULL, NULL, 0);
	p = fork();
	if (p == -1)
		err("heredoc fork()", NULL, NULL, 0);
	if (0 == p)
	{
		close(fildes[0]);
		while (1)
		{
			printf("> "); fflush(stdout);
			userinput = get_line(STDIN_FILENO);
			if (ft_strncmp(userinput, lim, ft_strlen(lim)) == 0)
			{
				free(userinput);
				exit(EXIT_SUCCESS);
			}
			write(fildes[1], userinput, ft_strlen(userinput));
			free(userinput);
		}
	}
	else
	{
		close(fildes[1]);
		waitpid(p, NULL, 0);
		_redirect(&fildes[0], NULL, STDIN_FILENO);
	}
}

/* Interpret waitpid() exit status (signals ignored here) */
static inline int	_get_exit_status(int status)
{
	int exit_status;
	int signal_number;

	exit_status = 0;
	signal_number = 0;
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
		signal_number = WTERMSIG(status);
    return (exit_status);
}

// Dynamically allocates pipe fds
void	create_pipes(t_args *st)
{
	int i;

	i = 0;
	st->fildes = malloc(st->cmd_count * sizeof(int *));
	if (!st->fildes)
		err("malloc", st, NULL, 0);
	while (i < st->cmd_count)
	{
		st->fildes[i] = malloc(3 * sizeof(int));
		if (!st->fildes[i])
			err("malloc", st, NULL, 0);
		st->fildes[2] = NULL;
		if (pipe(st->fildes[i]) < 0)
			err("pipe", st, NULL, 0);
		i++;
	}
}


// TODO refactor to dynamically create pipes for each proc pair
// remove temp file writeout
// allows procs to run in parallel with single fork loop
// children talk directly; parents just close ends
// INFILE || CMD 1  ||  CMD 2  ||  OUTFILE
// 			parent1		parent2
// 			child1	PIPE1  child2 ...
int main(int argc, char *argv[], char *env[])
{
	t_args st;
	pid_t p;
	int i; 
	int status; 

	parse_args(argc, argv, env, &st);
	create_pipes(&st);
    i = -1;
	while (++i < st.cmd_count) {
		p = fork();
		if (0 == p)
		{ 
			//printf("\n|%d:child:", i + 1); fflush(stdout); 
			close(st.fildes[i][0]);
            if (i == 0 && !st.heredoc) // read from input file on first command
                _redirect(NULL, argv[1], STDIN_FILENO);
			else if (i == 0 && st.heredoc)
				_heredoc(argv[2]);
            else
			{
				//printf("|redirecting stdin to fildes [%d]", i - 1);	
                _redirect(&st.fildes[i - 1][0], NULL, STDIN_FILENO); // read from last child
			}
            //printf("|redirected stdin. loading cmd %s|", argv[i + 2 + (int)st.heredoc]); fflush(stdout);
            if (i + 1 < st.cmd_count) // write to pipe if not last command
			{
				//printf("|redirect stdout to write to %d|", i);
				_redirect(&st.fildes[i][1], NULL, STDOUT_FILENO);
			}	
			else
			{
				//printf("|redirect stdout to write to outfile|");
				_redirect(NULL, st.outfile, STDOUT_FILENO);
			}
			if (st.cmdpaths[i] == NULL)
				exit(EXIT_FAILURE);
			else
				if (execve(st.cmdpaths[i], (char *const *)st.execargs[i], env) == -1)
					err("execve()", &st, NULL, 0);
		}
		else if (p > 0) //fildes[0]=3; fildes[1]=4; "temp"=5; 
		{
			if (i  + 1< st.cmd_count )
				waitpid(p, &status, WNOHANG); //let keep running
			else
				waitpid(p, &status, 0);
			//printf("\n%d:parent:", i + 1); fflush(stdout);
			close(st.fildes[i][1]);
			//printf("|exitstat:%d|",_get_exit_status(status)); fflush(stdout);
            //if (i + 1 < st.cmd_count && _get_exit_status(status) == 0)
			//{
				//_writeout("temp", st.fildes[i][0]);
				//printf("cmds:%d|", st.cmd_count); fflush(stdout);
				//int temp = open("temp", O_RDONLY);
				//printf("wroteout:%s\n", get_line(temp)); fflush(stdout); close(temp); }
			//printf("\n"); fflush(stdout);
			if (i != 0)
				close(st.fildes[i - 1][0]);
		}
		else
            err("fork", &st, NULL, 0);
	}
    unlink("temp"); 
	return (_get_exit_status(status));
}

