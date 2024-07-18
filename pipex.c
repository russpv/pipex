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
int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("Insufficient arguments.");
		return (EXIT_FAILURE);
	}
	// parse input
	const char *cmd = argv[2];
	const char *infile = argv[1];
	char *content;
	const char *outfile = argv[argc - 1];
	int fd; pid_t p; int status; int fildes[2];
	
	/*
	// 
	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file");
		return (FAILURE);
	}
	*/
	// TODO parse args into queue... argv**
	// TODO execve() to use args
	// TODO dup2 for each child command redirect to filename argv[argc]
	// TODO dup2 for first command redirect filename argv[1] to STDIN
	//
	while (argc--) {	
		/* PIPE: child to push output to parent (children spawn parents) */ 
		if (pipe(fildes) < 0) /* read on fildes[0] write on fildes[1] */
		{
			perror("Pipe dain't werkin rite");
			exit(EXIT_FAILURE);
		}
		p = fork();
		if (0 == p) /* This is the prior process in the pipeline */
		{ 
			printf("%d:child:\n", argc); 
			if (close(fildes[0] == -1)) { // close read end
				perror("close");
				exit(EXIT_FAILURE);
			}
			fd = open("file.txt", O_RDONLY);
			if (fd == -1) {
				perror("open");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDIN_FILENO) == -1) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			//close(fd);
			dup2(fildes[1], STDOUT_FILENO);
			close(fildes[1]);	
			char *argv2[] = {"cat", NULL}; // perhaps this passes flags
			if (execve("/bin/cat", argv2, NULL) == -1) {
				perror("execve sucked");
				exit(EXIT_FAILURE);
			} // what follows after execve is not reached
			//printf("we good\n"); fflush(stdout);
			//write(fildes[1], "from child\nnextline", 19); 
			//write(fildes[1], ft_itoa(argc), 1);
			//close(fildes[1]);
			//fflush(stdout);
		}
		else if (p > 0) /* This is the next process */
		{
			waitpid(p, &status, 0);
			close(fildes[1]);
			printf("%d:parent:", argc);
			char *line = "\0";
			while (line) {
				if (*line)
					printf("%s", line); fflush(stdout);
				line = get_next_line(fildes[0]);
			}
			printf("\n"); fflush(stdout);
			close(fildes[0]);

			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("fork dain't worked\n");
			exit(EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
