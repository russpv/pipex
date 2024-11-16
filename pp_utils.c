#include "pipex.h"

/* If cmd path is accessible, loads it into struct
 * else exits safely.
 */
int	check_access(char *path, int idx, t_args *st)
{
	if (access((const char *)path, F_OK) == 0)
	{
		if (access((const char *)path, X_OK) != 0)
			return (permission_err(path, st, idx));
		st->cmdpaths[idx] = ft_strdup(path);
		if (!st->cmdpaths[idx])
			err("_load_cmdpath: strdup() failure", st, NULL, 0);
		return (SUCCESS);
	}
	return (FAILURE);
}

void	debug_print(const char *format, ...)
{
	va_list	args;

	if (DEBUG)
	{
		va_start(args, format);
		fprintf(stderr, "DEBUG: %d ", getpid());
		vfprintf(stderr, format, args);
		va_end(args);
	}
}


static int is_numeric(const char *str) {
    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0; // Not a number
        }
        str++;
    }
    return 1; // It's a valid number
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

// Function to print all open file descriptors of a process
void print_open_fds(pid_t pid) {
    DIR *fd_dir;
    struct dirent *fd_entry;
    char fd_path[256];

    snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd", pid);
    if ((fd_dir = opendir(fd_path)) == NULL) {
        perror("opendir /proc/<pid>/fd");
        return;
    }

    printf("  Open file descriptors for process %d:\n", pid);
    
    // Iterate through the file descriptors of the process
    while ((fd_entry = readdir(fd_dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(fd_entry->d_name, ".") == 0 || strcmp(fd_entry->d_name, "..") == 0) {
            continue;
        }

        // Construct the full path to the file descriptor
        char link_path[256];
        snprintf(link_path, sizeof(link_path), "/proc/%d/fd/%s", pid, fd_entry->d_name);

        // Read the symbolic link to get the target (file the descriptor points to)
        char target[256];
        ssize_t len = readlink(link_path, target, sizeof(target) - 1);
        if (len != -1) {
            target[len] = '\0';  // Null-terminate the string
            printf("    FD %s -> %s\n", fd_entry->d_name, target);
        } else {
            perror("readlink");
        }
    }

    closedir(fd_dir);
}

void print_live_processes() {
    pid_t pgid = getpgrp();  // Get the process group ID of the current process
    DIR *dir;
    struct dirent *entry;

    // Print the current PGID for debugging
    printf("Current process PGID: %d\n", pgid);

    // Open /proc directory to scan for processes
    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir /proc");
        return;
    }

    printf("Live processes in the same process group (PGID: %d):\n", pgid);

    // Loop through all processes listed in /proc
    while ((entry = readdir(dir)) != NULL) {
        // Only look at directories that are numeric (representing process IDs)
        if (is_numeric(entry->d_name)) {
            pid_t pid = atoi(entry->d_name);  // Get process ID
            char status_file[256];
            FILE *file;
            char line[256];

            // Construct the path to the status file for the process
            snprintf(status_file, sizeof(status_file), "/proc/%d/status", pid);

            // Open the status file to read process details
            if ((file = fopen(status_file, "r")) != NULL) {
                // Read the process status to find the process group ID
                while (fgets(line, sizeof(line), file) != NULL) {
                    if (strncmp(line, "Pgid:", 5) == 0) {
                        pid_t pgrp = atoi(line + 6);  // Extract the PGID from the file
                        printf("Process %d has PGID: %d\n", pid, pgrp); // Debugging PGID

                        if (pgrp == pgid) {
                            printf("Process %d (PGID: %d) is in the same group.\n", pid, pgrp);
                            // Now print the open file descriptors for this process
                            print_open_fds(pid);
                        }
                        break;
                    }
                }
                fclose(file);
            }
        }
    }

    closedir(dir);
}