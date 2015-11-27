/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/21 19:48:59 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/27 02:39:12 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "libft_test.h"

typedef struct	s_sig {
	int		num;
	char	*name;
	char	*str;
}				t_sig;

t_sig	sigs[] = {{1, "SIGHUP", "terminal line hangup"},
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
	{31, "SIGUSR2", "User defined signal 2"}};

int		main(void) {
	char	*argv[] = {"run_test", NULL};
	char	*env[] = {"DYLD_INSERT_LIBRARIES=./assets/malloc.dylib", "DYLD_FORCE_FLAT_NAMESPACE=1", NULL};
	pid_t	pid;
	char	ret[2];

	if ((pid = fork()) == 0)
		exit(execve("./assets/libtests", argv, env));
	else {
		wait((int *)ret);
		if (ret[0] != 0) {
			printf("[%s]: %s\n", sigs[ret[0] - 1].name, sigs[ret[0] - 1].str);
			printf("\033[38;5;201mOMG you crashed my program, please report this bug to @alelievr !\nthanks\n\033[0m");
		}
		unlink(TMP_FILE);
		unlink(MALLOC_FILE);
	}
	return (0);
}
