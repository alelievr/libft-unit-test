/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/13 19:59:29 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/22 00:20:16 by bciss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include "libft_test.h"
#include "init.c"

void	ft_exit(char *str) {
	printf("%s\n", str);
	exit(-1);
}

void	ft_raise(int s) {
	display_test_result(s, current_explication);
}

void	load_test(void *handle, int start) {
	void	*tmpfun;

	for (int i = start; fun_test_table[i].fun_name; i++) {
		current_test_id++;
		current_fun_name = fun_test_table[i].fun_name;
		tmpfun = dlsym(handle, fun_test_table[i].fun_name);
		if (!tmpfun) {
			ft_raise(TEST_MISSING);
			continue ;
		}
		fun_test_table[i].fun_test_ptr();
	}
}

void	run_subtests(void *h, int start) {
	void	*tmpfun;
	static void	*handle = NULL;

	if (!handle)
		handle = h;
	if (fun_subtest_table[start].fun_name) {
		current_fun_name = fun_subtest_table[start].fun_name;
		tmpfun = dlsym(handle, fun_subtest_table[start].fun_name);
		current_subtest_id++;
		MALLOC_RESET;
		if (tmpfun)
			fun_subtest_table[start].fun_test_ptr(tmpfun);
	}
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
		if (last_test_id != current_test_id)
			time = 0;
		if (time >= TIMEOUT_MILLIS) {
			time = 0;
			kill(g_pid, SIGKILL);
		}
		time++;
		last_test_id = current_test_id;
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
	MALLOC_RESET;
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
