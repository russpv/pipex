#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

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
static void    _writeout(char *outname, int infd)
{
    int bytes_read;
    int bytes_written;
    int buffer[BUFSZ];

    ft_memset(buffer, 0, BUFSZ);
    int outfd = open(outname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfd == -1)
	{
        err("open", NULL, NULL);
	}
	while ((bytes_read = read(infd, buffer, BUFSZ)) > 0)
    {
        bytes_written = write(outfd, buffer, bytes_read);
        if (bytes_written != bytes_read)
            err("writeout", NULL, NULL);
    }
}

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
		printf("|redirect:%s_from_%d|", topath, from); fflush(stdout);
		if (from == STDIN_FILENO)
        	fd = open(topath, O_RDONLY | O_CREAT, 0644);
		else if (from == STDOUT_FILENO)
			fd = open(topath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = -1 ;
		if (fd < 0)
            err("redirect() open", NULL, NULL);
	}
    else
	{
        fd = *to;
		printf("|redirect:%d_to_%d|", fd, from); fflush(stdout);
	}
    if (dup2(fd, from) == -1)
        err("dup2 stdin", NULL, NULL);
    close(fd);
    return (from);
}

static void	heredoc(char *lim)
{
	int fildes[2];
	pid_t p;
	char *userinput;
	
	printf("We in heredoc|"); fflush(stdout);
	userinput = NULL;
	if (pipe(fildes) < 0)
		err("heredoc pipe()", NULL, NULL);
	p = fork();
	if (p == -1)
		err("heredoc fork()", NULL, NULL);
	if (0 == p)
	{
		close(fildes[0]); //read end
		while (1)
		{
			printf("> ");
			userinput = get_next_line(STDIN_FILENO);
			printf(">%s", userinput); fflush(stdout);
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

int main(int argc, char *argv[], char *env[])
{
	t_args st;
	pid_t p;
	int i; 
	int status; 

	parse_args(argc, argv, env, &st);
    i = -1;
	while (++i < st.cmd_count) {
		if (pipe(st.fildes) < 0)
			err("pipe", &st, NULL);
		p = fork();
		if (0 == p)
		{ 
			printf("%d:child:", i + 1); fflush(stdout); 
			close(st.fildes[0]);
            if (i == 0 && !st.heredoc) // read from input file on first command
                _redirect(NULL, argv[1], STDIN_FILENO);
			else if (i == 0 && st.heredoc)
				heredoc(argv[2]);
            else 
                _redirect(NULL, "temp", STDIN_FILENO); 
            printf("|redirected stdin. loading %s|", argv[i + 2 + (int)st.heredoc]); fflush(stdout);
            if (i != st.cmd_count - 1) // write to pipe if not last command
                _redirect(&st.fildes[1], NULL, STDOUT_FILENO);
			else
				_redirect(NULL, st.outfile, STDOUT_FILENO);
			
			if (execve(st.cmdpaths[i], (char *const *)st.execargs[i], NULL) == -1)
				err("execve sucked", &st, NULL);
		}
		else if (p > 0) //fildes[0]=3; fildes[1]=4; "temp"=5; 
		{
			waitpid(p, &status, 0);
			printf("%d:parent:", i + 1); fflush(stdout);
			close(st.fildes[1]);
            if (i != st.cmd_count - 1) // write to tempfile if not last command
				_writeout("temp", st.fildes[0]);
            close(st.fildes[0]);
			printf("\n"); fflush(stdout);
		}
		else
            err("fork", &st, NULL);
	}
    unlink("temp"); 
	return (EXIT_SUCCESS);
}

