#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "get_next_line.h"
#include "libft.h"

/* PIPEX
 * Expects "[infile] [command] | ... | [command] > [outfile]" args
 *
*/

// make a basic pipe. don't need symbols
// read arg as infile, then outfile
// getargcount() ... parse input
// how to create a pipe with PIPE?

static void    _err(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/* writes infd to fresh outname file */
static void    _writeout(char *outname, int infd)
{
    int bytes_read;
    int bytes_written;
    int buffer[BUFSZ];

    ft_memset(buffer, 0, BUFSZ);
    int outfd = open(outname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfd == -1)
        _err("open");
    while ((bytes_read = read(infd, buffer, BUFSZ)) != 0)
    {
        bytes_written = write(outfd, buffer, bytes_read);
        if (bytes_written != bytes_read)
            _err("writeout");
    }
}

static int _redirect(int tofile, char *topath, int fromfile)
{
    int fd;
    
    if (topath)
    {
        fd = open(topath, O_RDONLY);
        if (fd == -1)
            _err("open");
    }
    else 
        fd = tofile;
    if (dup2(fd, fromfile) == -1)
        _err("dup2 stdin");
    close(fd);
    return (fromfile);
}

static char *_getline(const char *path)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        _err("open");
    return (get_next_line(fd));
}
void    printarr(char **arr)
{
    while (arr) {
        printf("%s\n", *arr);
        arr++;
    } fflush(stdout);
}


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
	if (argc < 4) // execname infile cmd1 cmd2
        _err("Insufficient arguments.");
	// parse input
	const char *cmd = argv[2];
	const char *infile = argv[1];
	char *content;
	const char *outfile = argv[argc - 1];
	int fd; pid_t p; int status; int fildes[2]; int fildes2[2];
    int i = 1; // argv COMMAND index
	
    printarr(env);
	if (pipe(fildes) < 0) /* init pipe; read on fildes[0] write on fildes[1] */
	    _err("pipe");
	while (++i < argc) {	// tempfd, fd, pipes are avail to parent/child
		p = fork(); // spawns independent deep copies of variables; points to same virtual mem
		if (0 == p)
		{ 
			printf("%d:child:", i - 1); fflush(stdout); 
			close(fildes[0]);
            if (i == 2) // read from input file on first command
                _redirect(-1, argv[1], STDIN_FILENO);
            else 
                _redirect(-1, "temp", STDIN_FILENO); 
            printf("redirected. loading %s", argv[i]); fflush(stdout);
            if (i != argc - 1) {// write to pipe if not last command
                _redirect(fildes[1], 0, STDOUT_FILENO);}
            const char *argv2[] = {"cat",  NULL}; //_load_args(argv[i]);
			if (execve("cat", (char *const *)argv2, NULL) == -1)
				_err("execve sucked");
		}
		else if (p > 0) 
		{
			waitpid(p, &status, 0);
			printf("%d:parent:", i - 1); fflush(stdout);
			close(fildes[1]);
            if (i != argc - 1) // write to tempfile if not last command
            { 
                _writeout("temp", fildes[0]);
               //printf("writtenout."); fflush(stdout); 
		        if (pipe(fildes2) < 0) /*makes another pipe */
		            _err("pipe2");
                if (dup2(fildes2[0], fildes[0]) < 0 || dup2(fildes2[1], fildes[1]) < 0)
                    _err("dup2");
                //printf("pipes duped"); fflush(stdout);
                printf("debug: written to temp: %s\n", _getline("temp"));
            }
            close(fildes[0]);
			printf("\n"); fflush(stdout);
		}
		else
            _err("fork");
	}
    unlink("temp"); 
	return (EXIT_SUCCESS);
}
