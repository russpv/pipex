#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "get_next_line.h"
#include "libft.h"
#include <fcntl.h>

/* PIPEX
 * Expects "[infile] [command] | ... | [command] > [outfile]" args
 *
*/
int printfd() {
    // Get the maximum number of file descriptors allowed
    long max_fd = sysconf(_SC_OPEN_MAX);
    if (max_fd == -1) {
        perror("sysconf");
        return 1;
    }
    pid_t pid = getpid();
    printf("PID: %d\n", pid); fflush(stdout);
    // Iterate through all possible file descriptors
    for (int fd = 0; fd < max_fd; fd++) {
        if (fcntl(fd, F_GETFD) != -1 || errno != EBADF) {
            // fcntl(fd, F_GETFD) returns -1 and sets errno to EBADF if fd is not open
            printf("File descriptor %d is open\n", fd); fflush(stdout);
        }
    }

    return 0;
}

static void	wraperr(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

static void	_initstruct(t_args *s, char *argv[], int argc)
{
	s->infile = argv[1];
	s->outfile = argv[argc - 1];
	s->cmds = argc - 1 - 2;
}

int main(int argc, char *argv[])
{
	int fd;
	pid_t p;
	int fildes[2];
	t_args my_s;

	if (argc < 2) // TODO set this properly
		wraperr("Insufficient args.");
	_initstruct(&my_s, argv, argc);
	
	while (--argc) {	
		if (pipe(fildes) < 0) /* read on fildes[0] write on fildes[1] */
			wraperr("Pipe");
		p = fork();
		if (0 == p) /* This is the prior process in the pipeline */
		{
			printf("%d:child:\n", argc);
			if (close(fildes[0]) == -1)
				wraperr("close");
			fd = open("file.txt", O_RDONLY);
			if (fd == -1)
				wraperr("open");
			if (dup2(fd, STDIN_FILENO) == -1)
				wraperr("dup2");
			close(fd); 
			dup2(fildes[1], STDOUT_FILENO); 
			close(fildes[1]);
			char *argv2[] = {argv[2], NULL}; // perhaps this passes flags
			if (execve(ft_strjoin("/bin/", argv[2]), argv2, NULL) == -1)
				wraperr("execve sucked");
		}
		else if (p > 0) /* This is the next process */
		{
			waitpid(p, &my_s.status, 0);
			close(fildes[1]);
			printf("%d:parent:", argc);
			char *line = "\0";
			while (line) {
				if (*line)
					printf("%s", line); fflush(stdout);
				line = get_next_line(fildes[0]);
			}
			printf("\n"); fflush(stdout);
			close(fildes[0]); // 0, 1, 2 fildes
		}
		else
			wraperr("fork");
	}
	return (EXIT_SUCCESS);
}
