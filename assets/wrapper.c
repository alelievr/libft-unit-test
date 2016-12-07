/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2015/11/21 19:48:59 by alelievr          #+#    #+#             */
/*   Updated  2016/10/17 16:07:03 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "libft_test.h"

typedef struct	s_sig {
	int		num;
	char	*name;
	char	*str;
}				t_sig;

t_sig	sigs[] = {
	{1, "SIGHUP", "terminal line hangup"},
	{2, "SIGINT", "interrupt program"},
	{3, "SIGQUIT", "quit program"},
	{4, "SIGILL", "illegal instruction"},
	{5, "SIGTRAP", "trace trap"},
	{6, "SIGABRT", "abort program (formerly SIGIOT)"},
	{7, "SIGEMT", "emulate instruction executed"},
	{8, "SIGFPE", "floating-point exception"},
	{9, "SIGKILL", "kill program"},
	{10, "SIGBUS", "bus error"},
	{11, "SIGSEGV", "segmentation violation"},
	{12, "SIGSYS", "non-existent system call invoked"},
	{13, "SIGPIPE", "write on a pipe with no reader"},
	{14, "SIGALRM", "real-time timer expired"},
	{15, "SIGTERM", "software termination signal"},
	{16, "SIGURG", "urgent condition present on socket"},
	{17, "SIGSTOP", "stop (cannot be caught or ignored)"},
	{18, "SIGTSTP", "stop signal generated from keyboard"},
	{19, "SIGCONT", "continue after stop"},
	{20, "SIGCHLD", "child status has changed"},
	{21, "SIGTTIN", "background read attempted from control terminal"},
	{22, "SIGTTOU", "background write attempted to control terminal"},
	{23, "SIGIO", "I/O is possible on a descriptor (see fcntl(2))"},
	{24, "SIGXCPU", "cpu time limit exceeded (see setrlimit(2))"},
	{25, "SIGXFSZ", "file size limit exceeded (see setrlimit(2))"},
	{26, "SIGVTALRM", "virtual time alarm (see setitimer(2))"},
	{27, "SIGPROF", "profiling timer alarm (see setitimer(2))"},
	{28, "SIGWINCH", "Window size change"},
	{29, "SIGINFO", "status request from keyboard"},
	{30, "SIGUSR1", "User defined signal 1"},
	{31, "SIGUSR2", "User defined signal 2"}
};

int		main(unused int ac, char **av) {
	char	*env[] = {
		"DYLD_INSERT_LIBRARIES=./assets/malloc.dylib",
		"DYLD_FORCE_FLAT_NAMESPACE=1",
		"TERM=xterm-256color",
		"LD_PRELOAD=./assets/malloc.dylib",
	   	NULL
	};
	pid_t	pid;
	int		ret;

	if ((pid = fork()) == 0)
		exit(execve("./assets/libtests", av, env));
	else if (pid != -1)
	{
		signal(SIGSTOP, SIG_IGN);
		signal(SIGKILL, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		signal(SIGKILL, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTRAP, SIG_IGN);
		signal(SIGTERM, SIG_IGN);
		signal(SIGUSR1, SIG_IGN);
		signal(SIGUSR2, SIG_IGN);
		wait(&ret);
		if (WIFSIGNALED(ret))
		{
			int sig = WTERMSIG(ret);
			if (sig != SIGINT && sig != SIGSTOP && sig != SIGKILL && sig != SIGQUIT && sig != SIGTERM && sig)
			{
				printf("[%s]: %s\n", sigs[sig - 1].name, sigs[sig - 1].str);
				printf("\033[38;5;201mOMG you crashed my program, please report this bug to @alelievr !\nthanks\n\033[0m");
			}
		}
		unlink(SHARED_MEM_FILE);
	}
	else
		perror("fork");
	return (0);
}
