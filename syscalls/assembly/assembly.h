#ifndef SYSCALLS_ASSEMBLY_ASSEMBLY_H
#define SYSCALLS_ASSEMBLY_ASSEMBLY_H

#define __attribute__(x)
#define SYSCALL_NR(name) \
asm volatile ("int 0x80" : : "a" (name))

typedef int pid_t;  // Assuming pid_t is an integer on your system
typedef long ssize_t; // Assuming ssize_t is a long integer

typedef unsigned long size_t; // Assuming size_t is an unsigned long integer
typedef unsigned int mode_t; // Assuming mode_t is an unsigned int integer

int exit(int status);
int fork(void);
ssize_t read(int fd, void *buf, size_t count);
int open(const char *pathname, int flags, mode_t mode);
int close(int fd);
pid_t waitpid(pid_t pid, int *status, int options);
pid_t getpid(void);
pid_t getppid(void);
int execve(const char* pathname, char* const argv[], char* const envp[]);

#endif