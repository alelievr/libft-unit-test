/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/13 19:59:29 by alelievr          #+#    #+#             */
/*   Updated: 2015/12/04 00:45:18 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <execinfo.h>
#include "libft_test.h"
#include "init.c"
#include <errno.h>

int		fd_pipe[2];
int		_stdout;

void	fd_to_buffer(int fd) {
	_stdout = dup(fd);
	if (pipe(fd_pipe) != 0)
		return ;
	dup2(fd_pipe[1], fd);
	close(fd_pipe[1]);
}

int		get_last_malloc_size(void) {
	int		fd;
	char	buff[0xF0];

	if ((fd = open(MALLOC_FILE, O_RDONLY)) == -1)
		return (0);
	if ((read(fd, buff, sizeof(buff))) == -1)
		return (0);
	return (atoi(buff));
}

char	*get_fd_buffer(int fd, char *buff, size_t size) {
	int		ret;

	if (buff == NULL) {
		char	b[0xF000];
		read(fd_pipe[0], b, sizeof(b));
		dup2(_stdout, fd);
		return (NULL);
	}
	ret = read(fd_pipe[0], buff, size);
	buff[ret] = 0;
	dup2(_stdout, fd);
	return (buff);
}

void	ft_exit(char *str) {
	printf("%s\n", str);
	exit(-1);
}

void	ft_raise(int s) {
	display_test_result(s, current_explication);
}

void	load_test(void *handle, int start) {
	for (int i = start; fun_test_table[i].fun_name; i++) {
		current_test_id++;
		current_fun_name = fun_test_table[i].fun_name;
		current_fun_visibility = fun_test_table[i].visible;
		fun_test_table[i].fun_test_ptr();
	}
	(void)handle;
}

void	run_subtests(void *h, int start) {
	void		*tmpfun;
	static void	*handle = NULL;

	if (!handle)
		handle = h;
	for (int i = start; fun_subtest_table[i].fun_name; i++, current_subtest_id++) {
		current_fun_name = fun_subtest_table[i].fun_name;
		tmpfun = dlsym(handle, fun_subtest_table[i].fun_name);
		MALLOC_RESET;
		RESET_DIFF;
		SET_CURRENT_PROTECTED(INVISIBLE);
		if (tmpfun)
			fun_subtest_table[i].fun_test_ptr(tmpfun);
		else if (fun_subtest_table[i].visible)
			ft_raise(TEST_MISSING);
	}
	current_fun_name = "";
	display_test_result(TEST_FINISHED, "");
	printf("\nSee %s for more informations !\n", LOG_FILE);
	dprintf(g_log_fd, "\n");
	close(g_log_fd);
	unlink(TMP_FILE);
	exit(0);
}

void	*timer(void *t) {
	static int	time = 0;
	static int	last_test_id = 0;

	(void)t;
	while (42) {
		if (last_test_id != current_subtest_id)
			time = 0;
		if (time >= TIMEOUT_MILLIS) {
			time = 0;
			kill(g_pid, SIGKILL);
		}
		time++;
		last_test_id = current_subtest_id;
		usleep(1000);
	}
	return (NULL);
}

void	load_timer(void) {
	pthread_t	p;

	if ((pthread_create(&p, NULL, timer, NULL) == -1))
		ft_exit("thread inits failed !");
}

int		main(void) {
	void	*handle;

	if ((g_log_fd = open(LOG_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1)
		ft_exit("can't open/create logfile !");
	if ((g_malloc_fd = open(TMP_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1)
		ft_exit("can't create tmp file !");
	if ((g_diff_fd = open(DIFF_FILE, O_RDWR | O_TRUNC | O_CREAT, 0600)) == -1)
		ft_exit("can't create/open diff file !");
	MALLOC_RESET;
	RESET_DIFF;
	if (!(handle = dlopen("./libft.so", RTLD_LAZY)))
		ft_exit(dlerror());

	/* Ignore user interupt signals: */
//	signal(SIGINT, SIG_IGN);
//	signal(SIGQUIT, SIG_IGN);

	/* Threads for timeout */
	load_timer();

	/* Running test for evry function: */
	load_test(handle, 0);
	run_subtests(handle, 0);
}
