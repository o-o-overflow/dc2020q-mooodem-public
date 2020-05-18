#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <signal.h>
#include <poll.h>

void sigchld_handler(int signum)
{
    if (signum != SIGCHLD) return;
    pid_t child;
    int status;
    while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            fprintf(stderr, "Child exited\n");
            exit(WEXITSTATUS(status));
        }
        if (WIFSTOPPED(status)) {
            fprintf(stderr, "Exiting due to %d\n", WSTOPSIG(status));
            exit(1);
        }
        if (WIFSIGNALED(status)) {
            fprintf(stderr, "Exiting due to term: %d\n", WTERMSIG(status));
            exit(1);
        }
        
        perror("Unhandled stop condition. Exiting.");
        exit(255);
    }
}

int main(int argc, char *argv[], char *envp[]) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    signal(SIGCHLD, sigchld_handler);

    // Create pipe to read from original stdin
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Failed to create pipe");
        return 1;
    }

    // Create child
    pid_t p = fork();
    if (p < 0) {
        perror("Fork failed!");
        return 1;
    }

    if (p == 0) {
        // Child process
        close(pipefd[1]); // Close write end of pipe
        dup2(pipefd[0], 0); // Assign read end to stdin fd

        char *new_args[argc];
        for (int i = 0; i < (argc-1); i++) {
            new_args[i] = argv[i+1];
        }
        new_args[argc-1] = NULL;
        execve(argv[1], new_args, envp);

        perror("execve failed!");
        exit(1);
    }

    // Parent process
    close(pipefd[0]); // Close read end of pipe

    while (1) {
        char buf;
        struct pollfd pollfd[1];
        pollfd[0].fd = 0;
        pollfd[0].events = POLLIN | POLLPRI | POLLRDHUP;
        pollfd[0].revents = 0;

        retval = ppoll(pollfd, 1, NULL, NULL);
        if (retval < 0) {
            perror("ppoll()");
            kill(p, SIGINT);                    
            return 1;
        } else if (retval > 0) {
            // fprintf(stderr, "ppoll returned: ");
            // if (pollfd[0].revents & POLLERR)   fprintf(stderr, "POLLERR ");
            // if (pollfd[0].revents & POLLIN)    fprintf(stderr, "POLLIN ");
            // if (pollfd[0].revents & POLLPRI)   fprintf(stderr, "POLLPRI ");
            // if (pollfd[0].revents & POLLOUT)   fprintf(stderr, "POLLOUT ");
            // if (pollfd[0].revents & POLLRDHUP) fprintf(stderr, "POLLRDHUP ");
            // if (pollfd[0].revents & POLLHUP)   fprintf(stderr, "POLLHUP ");
            // if (pollfd[0].revents & POLLNVAL)  fprintf(stderr, "POLLNVAL ");
            // fprintf(stderr, "\n");

            // Fatal
            if (pollfd[0].revents & (POLLERR | POLLRDHUP | POLLHUP | POLLNVAL)) {
                kill(p, SIGINT);
                return 1;
            }

            int n = read(0, &buf, 1);
            if (n <= 0) {
                if (n == 0) {
                    // FIXME: Why am I getting this with xinetd + (something)
                    // + qemu where something can be apparently any program
                    // that reads from stdin and writes to stdout. Works with
                    // socat just fine.
                    fprintf(stderr, "END OF FILE\n");
                    kill(p, SIGINT);
                    return 1;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // fprintf(stderr, "EAGAIN: Continuing\n");
                    continue;
                }
                if (errno == EBADF)  { fprintf(stderr, "EBADF\n");  exit(0);}
                if (errno == EFAULT) { fprintf(stderr, "EFAULT\n"); exit(0);}
                if (errno == EINTR)  { fprintf(stderr, "EINTR\n");  exit(0);}
                if (errno == EINVAL) { fprintf(stderr, "EINVAL\n"); exit(0);}
                if (errno == EINVAL) { fprintf(stderr, "EINVAL\n"); exit(0);}
                if (errno == EIO)    { fprintf(stderr, "EIO\n");    exit(0);}
                if (errno == EISDIR) { fprintf(stderr, "EISDIR\n"); exit(0);}

                fprintf(stderr, "Read returned < 0, exiting. Errno = %d\n", errno);
                kill(p, SIGINT);
                return 1;
            }
            // printf("Writing %c to child\n", buf);
            while (1) {
            n = write(pipefd[1], &buf, 1); // Write to child
            if (n < 1) {
                fprintf(stderr, "Write failed\n");
                kill(p, SIGINT);                    
                return 1;
            }
            break;
            }

        } else {
            perror("Timed out");
            return 1;
        }
    }
    return 0;
}
