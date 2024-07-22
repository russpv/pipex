#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

/* PIPEX
 * Expects "[infile] [command] | ... | [command] > [outfile]" args
 *
*/

// make a basic pipe. don't need symbols
// read arg as infile, then outfile
// getargcount() ... parse input
// how to create a pipe with PIPE?

void    err(const char *msg, t_args *st, char *str)
{
    perror(msg);
	if (st)
		cleanup(st);
	if (str)
		free (str);
    exit(EXIT_FAILURE);
}

/* Writes infd to fresh outname file */
static void    _writeout(char *outname, int infd)
{
    int bytes_read;
    int bytes_written;
    int buffer[BUFSZ];

    ft_memset(buffer, 0, BUFSZ);
    int outfd = open(outname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfd == -1)
        err("open", NULL, NULL);
    while ((bytes_read = read(infd, buffer, BUFSZ)) != 0)
    {
        bytes_written = write(outfd, buffer, bytes_read);
        if (bytes_written != bytes_read)
            err("writeout", NULL, NULL);
    }
}

/* Wraps open() and dup2() */
static int _redirect(int tofile, char *topath, int fromfile)
{
    int fd;
    
    if (topath)
    {
        fd = open(topath, O_RDONLY);
        if (fd == -1)
            err("open", NULL, NULL);
    }
    else 
        fd = tofile;
    if (dup2(fd, fromfile) == -1)
        err("dup2 stdin", NULL, NULL);
    close(fd);
    return (fromfile);
}

/* don't think you actually need this */
static char *_getline(const char *path)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        err("open", NULL, NULL);
    return (get_next_line(fd));
}

/* don't need this */
void    printarr(char **arr)
{
    while (*arr) {
        printf("%s\n", *arr);
        arr++;
    } fflush(stdout);
}

void	printarrarr(char ***arr)
{
	while (*arr)
	{
		printarr(*arr);
		arr++;
	} 
}

/* Prepares the arg array for execve */
static char **_load_args(const char *arg)
{
    char **arr;
    char **res;
    size_t len;

    printf("loadargs"); fflush(stdout);
    arr = ft_split(arg, ' ');
    len = 0;
    while (arr[len])
        len++;
    printf("malloc"); fflush(stdout);
    res = malloc(sizeof(char *) * (len + 1));
    if (!res)
    {
        free(arr);
        return (NULL);
    }
    len = 0;
    while (arr[len])
        res[len] = arr[len];
    free(arr);
    res[len] = NULL;
    printarr(res);
    return (res);
}

/* arg 2: filepath; arg 3: command ... */
int main(int argc, char *argv[], char *env[])
{
	t_args st;
	pid_t p;
	int i; //argv commands
	int status; //waitpid

	parse_args(argc, argv, env, &st);
    i = 1;
	if (pipe(st.fildes) < 0) /* init pipe; read on fildes[0] write on fildes[1] */
	    err("pipe", &st, NULL);
	while (++i < argc) {	// tempfd, fd, pipes are avail to parent/child
		p = fork(); // spawns independent deep copies of variables; points to same virtual mem
		if (0 == p)
		{ 
			printf("%d:child:", i - 1); fflush(stdout); 
			close(st.fildes[0]);
            if (i == 2) // read from input file on first command
                _redirect(-1, argv[1], STDIN_FILENO);
            else 
                _redirect(-1, "temp", STDIN_FILENO); 
            printf("redirected. loading %s", argv[i]); fflush(stdout);
            if (i != argc - 1) {// write to pipe if not last command
                _redirect(st.fildes[1], 0, STDOUT_FILENO);}
			if (execve(st.cmdpaths[i - 2], (char *const *)st.execargs[i - 2], NULL) == -1)
				err("execve sucked", &st, NULL);
		}
		else if (p > 0) 
		{
			waitpid(p, &status, 0);
			printf("%d:parent:", i - 1); fflush(stdout);
			close(st.fildes[1]);
            if (i != argc - 1) // write to tempfile if not last command
            { 
                _writeout("temp", st.fildes[0]);
               //printf("writtenout."); fflush(stdout); 
		        if (pipe(st.fildes2) < 0) /*makes another pipe */
		            err("pipe2", &st, NULL);
                if (dup2(st.fildes2[0], st.fildes[0]) < 0 || dup2(st.fildes2[1], st.fildes[1]) < 0)
                    err("dup2", &st, NULL);
                //printf("pipes duped"); fflush(stdout);
                printf("debug: written to temp: %s\n", _getline("temp"));
            }
            close(st.fildes[0]);
			printf("\n"); fflush(stdout);
		}
		else
            err("fork", &st, NULL);
	}
    unlink("temp"); 
	return (EXIT_SUCCESS);
}
