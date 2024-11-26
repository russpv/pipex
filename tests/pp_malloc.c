#define _GNU_SOURCE
#include "pipex.h"  //TODO change as needed
#include <stdlib.h>
#include <dlfcn.h>  // For dlsym()

/* Override malloc.
 * Define a macro that replaces malloc() calls with this.
   #define malloc(X) my_malloc(X)
   void* my_malloc(size_t size);
 */
void* my_malloc(size_t size) {
    static void* (*real_malloc)(size_t) = NULL;

    if (!real_malloc) {
        // Initialize the real malloc function using dlsym
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        if (!real_malloc) {
            // Handle error if dlsym fails
            fprintf(stderr, "Error in dlsym: %s\n", dlerror());
            exit(1);
        }
    }
    // Simulate malloc failure on specific condition
    static int call_count = 0;
    int failure_call = 0;
  // Check if the environment variable CALL_COUNT is set
    char *env_call_count = getenv("CALL_COUNT");
    if (env_call_count) {
        failure_call = atoi(env_call_count);  // Set call_count from the environment variable
    }
    // CALL_COUNT is incremented in the test script.

	call_count++;
	fprintf(stderr, "Malloc! #%d vs test #%d\n", call_count, failure_call);
    if (call_count == failure_call) {
       return NULL;  // Force malloc failure on the second call
    }

    return real_malloc(size);  // Use the original malloc
}