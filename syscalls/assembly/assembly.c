#define __attribute__(x)

/*
 *  This file contains the assembly code for system calls.
 *
 *  The system calls are defined in the following format:
 *
 *  int syscall_name(arg1, arg2, ...);
 *
 *  where syscall_name is the name of the system call,
 *  and arg1, arg2, ... are the arguments to the system call.
 */

#define SYSCALL_NR(name) \
	asm volatile ("int 0x80" : : "a" (name))

/*
 *  exit
 *
 *  This system call terminates the current process.
 *
 *  int exit(int status);
 *
 *  status is the exit status of the process.
 */
int exit(int status)
{
	SYSCALL_NR(1);
	return status;
}

/*
 *  fork
 *
 *  This system call creates a new process, which is a copy of the current process.
 *
 *  int fork(void);
 *
 *  Returns the process ID of the new process, or -1 on error.
 */
int fork(void)
{
	SYSCALL_NR(2);
	return 0;
}

/*
 *  read
 *
 *  This system call reads data from a file descriptor.
 *
 *  ssize_t read(int fd, void *buf, size_t count);
 *
 *  fd is the file descriptor to read from.
 *  buf is a pointer to the buffer to store the data read.
 *  count is the number of bytes to read.
 *
 *  Returns the number of bytes read, or -1 on error.
 */
ssize_t read(int fd, void *buf, size_t count)
{
	SYSCALL_NR(3);
	return 0;
}

/*
 *  write
 *
 *  This system call writes data to a file descriptor.
 *
 *  ssize_t write(int fd, const void *buf, size_t count);
 *
 *  fd is the file descriptor to write to.
 *  buf is a pointer to the buffer containing the data to write.
 *  count is the number of bytes to write.
 *
 *  Returns the number of bytes written, or -1 on error.
 */
ssize_t write(int fd, const void *buf, size_t count)
{
	SYSCALL_NR(4);
	return 0;
}

/*
 *  open
 *
 *  This system call opens a file.
 *
 *  int open(const char *pathname, int flags, mode_t mode);
 *
 *  pathname is the path name of the file to open.
 *  flags is a bit mask of flags that specify how the file should be opened.
 *  mode is the permissions to use when creating the file.
 *
 *  Returns the file descriptor of the opened file, or -1 on error.
 */
int open(const char *pathname, int flags, mode_t mode)
{
	SYSCALL_NR(5);
	return 0;
}

/*
 *  close
 *
 *  This system call closes a file descriptor.
 *
 *  int close(int fd);
 *
 *  fd is the file descriptor to close.
 *
 *  Returns 0 on success, or -1 on error.
 */
int close(int fd)
{
	SYSCALL_NR(6);
	return 0;
}

/*
 *  waitpid
 *
 *  This system call waits for a child process to terminate.
 *
 *  pid_t waitpid(pid_t pid, int *status, int options);
 *
 *  pid is the process ID of the child process to wait for.
 *  status is a pointer to a location where the exit status of the child process will be stored.
 *  options is a bit mask of flags that specify how the wait should be performed.
 *
 *  Returns the process ID of the child process that terminated, or -1 on error.
 */
pid_t waitpid(pid_t pid, int *status, int options)
{
	SYSCALL_NR(7);
	return 0;
}

/*
 *  getpid
 *
 *  This system call returns the process ID of the calling process.
 *
 *  pid_t getpid(void);
 *
 *  Returns the process ID of the calling process.
 */
pid_t getpid(void)
{
	SYSCALL_NR(8);
	return 0;
}

/*
 *  getppid
 *
 *  This system call returns the process ID of the parent process.
 *
 *  pid_t getppid(void);
 *
 *  Returns the process ID of the parent process.
 */
pid_t getppid(void)
{
	SYSCALL_NR(9);
	return 0;
}

/*
 *  execve
 *
 *  This system call executes a new program file.
 *
 *  int execve(const char *pathname, char *const argv[], char *const envp[]);
 *
 *  pathname is the path name of the program file to execute.
 *  argv is a pointer to an array of argument strings for the new program.
 *  envp is a pointer to an array of environment variable strings for the new program.
 *
 *  Returns -1 on error. The return value for success isn't defined here since it doesn't return normally upon success. 
 */
int execve(const char* pathname, char* const argv[], char* const envp[])
{
	SYSCALL_NR(11);
	return -1; // Indicate an error as it doesn't return normally on success
}