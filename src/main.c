/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/13 19:59:29 by alelievr          #+#    #+#             */
/*   Updated: 2023/01/19 03:24:35 by knagase          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <execinfo.h>
#include <locale.h>
#include <sys/mman.h>
#include "libft_test.h"
#include "init.c"
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

int		fd_pipe[2];
int		_stdout;
bool	g_hardcore = false;

static t_option options[] = {
	{"--versus", 'v', &g_versus},
	{"--bench", 'b', &g_bench},
	{"--speed", 's', &g_nospeed},
	{"--help", 'h', &g_help},
	{"--nobenchlog", 'n', &g_nobenchlog},
	{"--hardcore", '\0', &g_hardcore},
	{NULL, 0, NULL}
};

void	*electric_alloc(size_t size)
{
	long	page_size = sysconf(_SC_PAGESIZE);
	void	*ptr = mmap(NULL, page_size * 2, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	memset(ptr, 'Z', page_size * 2);
	mprotect(ptr + page_size, page_size, PROT_NONE);
	return (ptr + page_size - size);
}

void	*electric_alloc_rev(size_t size)
{
	long	page_size = sysconf(_SC_PAGESIZE);
	void	*ptr = mmap(NULL, page_size * 2, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	memset(ptr, 'Z', page_size * 2);
	mprotect(ptr, page_size, PROT_NONE);
	return (ptr + page_size + size);
}

unsigned long long	ft_clock(void) {
	return (clock());
}

void	fd_to_buffer(int fd) {
	_stdout = dup(fd);
	if (pipe(fd_pipe) != 0)
		return ;
	dup2(fd_pipe[1], fd);
	close(fd_pipe[1]);
}

int		get_last_malloc_size(void) {
	return ((int)OFF_ALLOC_SIZE);
}

char	*get_fd_buffer(int fd, char *buff, size_t size) {
	int		ret;

	if (buff == NULL) {
		char	b[0xF000 + 1];
		ret = read(fd_pipe[0], b, sizeof(b) - 1);
		if (ret != -1)
			b[ret] = 0;
		dup2(_stdout, fd);
		return (NULL);
	}
	ret = read(fd_pipe[0], buff, size);
	if (ret != -1)
		buff[ret] = 0;
	dup2(_stdout, fd);
	return (buff);
}

void	ft_exit(char *str, ...) {
	va_list v;
	va_start(v, str);
	vprintf(str, v);
	va_end(v);
	exit(-1);
}

void	ft_raise(int s) {
	display_test_result(s, current_explication);
}

_Bool	str_find_array(char **array, char *str)
{
	while (*array)
		if (strstr(str, *array++))
			return true;
	return false;
}

void	load_test(char **function_list) {
	for (int i = 0; fun_test_table[i].fun_name; i++)
		if (!*function_list || str_find_array(function_list, fun_test_table[i].fun_name))
		{
			current_test_id++;
			current_fun_name = fun_test_table[i].fun_name;
			current_fun_visibility = fun_test_table[i].visible;
			current_part = fun_test_table[i].part;
			fun_test_table[i].fun_test_ptr();
		}
}

void	load_bench(char **function_list)
{
	for (int i = 0; fun_bench_table[i].fun_name; i++)
		if (!*function_list || str_find_array(function_list, fun_bench_table[i].fun_name))
		{
			current_test_id++;
			current_fun_name = fun_bench_table[i].fun_name;
			current_fun_visibility = fun_bench_table[i].visible;
			fun_bench_table[i].fun_test_ptr();
		}
}

void	run_subtests(void *h, int start) {
	void		*tmpfun;
	static void	*handle = NULL;

	if (!handle)
		handle = h;
	for (int i = start; fun_subtest_table[i].fun_name; i++, current_subtest_id++)
	{
		MALLOC_RESET;
		RESET_DIFF;
		SET_CURRENT_PROTECTED(INVISIBLE);
		current_part = fun_subtest_table[i].part;
		current_fun_name = fun_subtest_table[i].fun_name;
		tmpfun = dlsym(handle, fun_subtest_table[i].fun_name);
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
	exit(0);
}

void	run_subbench(void *h, void *h2)
{
	void		*tmpfun;
	void		*vsfun = NULL;

	for (int i = 0; fun_subbench_table[i].fun_name; i++, current_subtest_id++)
	{
		RESET_DIFF;
		SET_CURRENT_PROTECTED(INVISIBLE);
		SET_BENCHITER(1);
		current_fun_name = fun_subbench_table[i].fun_name;
		tmpfun = dlsym(h, fun_subbench_table[i].fun_name);

		if (h2)
			vsfun = dlsym(h2, fun_subbench_table[i].fun_name);

		if (tmpfun)
			fun_subbench_table[i].fun_bench_ptr(tmpfun, vsfun);
		else if (fun_subbench_table[i].visible)
			ft_raise(TEST_MISSING);
	}
	current_fun_name = "";
	display_test_result(TEST_FINISHED, "");
	printf("\nSee %s for more informations !\n", LOG_FILE);
	dprintf(g_log_fd, "\n");
	close(g_log_fd);
	exit(0);
}

void	*timer(void *t) {
	static int	time = 0;
	static int	last_test_id = 0;

	int			timeout_millis = TIMEOUT_MILLIS;
	if (g_bench || g_versus != NULL)
		timeout_millis *= 10;
	(void)t;
	while (42) {
		if (last_test_id != current_subtest_id)
			time = 0;
		if (time >= timeout_millis) {
			time = 0;
			g_time.state = TEST_CRASH;
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

char	**get_options(char **av)
{
	int		i = 0, j;
	_Bool	is_option;
	int (*strcmp_sys)(const char *s1, const char *s2) = strcmp;

	while (av[++i])
	{
		j = -1;
		is_option = false;
		while (options[++j].long_name)
		{
			if (!strcmp(options[j].long_name, av[i]) ||
					!strcmp_sys((char *)(char[3]){'-', options[j].short_name, 0}, av[i]))
			{
				if (av[i + 1] && options[j].short_name == 'v')
					*(unsigned long *)options[j].arg = (unsigned long)av[++i];
				else if (options[j].arg)
					*(char *)options[j].arg = 1;
				is_option = true;
			}
		}
		if (!is_option || !strcmp("--", av[i]))
			break ;
	}
	return av + i;
}

static void	usage() __attribute((noreturn));
static void	usage() {
	printf("usage ./run_test <opt> <functions>\n"
			"-h or --help: display help\n"
			"-s or --speed: run the test without speed evaluation\n"
			"-b or --bench: speed test of your library (vs system)\n"
			"-n or --nobenchlog: disable the bench test result output in the ranking file\n"
			"-v or --versus: run with a shared library in parameter, "
			"do the same than -b but with the parameter instead of the system's library\n"
			"you can additionally specify function name to test only these function\n\n"
			"ex1: ./run_test -v beat-me.so ft_mem # test all mem function with beat-me.so functions in reference\n"
			"ex2: ./run_test ft_strdup # test strdup function\n");
	exit(0);
}

int		main(unused int ac, char **av) {
	void	*handle = NULL;
	void	*handle_vs = NULL;
	char	**function_list = NULL;

	function_list = get_options(av);
	g_nospeed = !g_nospeed;
	if (g_help)
		usage();
	setlocale(LC_ALL, "");
	if ((g_shared_mem = mmap(NULL, sizeof(t_map), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0)) == MAP_FAILED)
		puts("failed to create shared memory map !"), raise(SIGKILL);
	int	fd;
	if ((fd = open(SHARED_MEM_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1)
		ft_exit("can't open/create shared memory file !\n");
	write(fd, &g_shared_mem, 8);
	close(fd);
	if ((g_log_fd = open(LOG_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1)
		ft_exit("can't open/create logfile !\n");
	if ((g_diff_fd = open(DIFF_FILE, O_RDWR | O_TRUNC | O_CREAT, 0600)) == -1)
		ft_exit("can't create/open diff file !\n");
	MALLOC_RESET;
	RESET_DIFF;
	if (!(handle = dlopen("./libft.so", RTLD_LAZY)))
		ft_exit(dlerror());

	//hardcore mode:
	if (g_hardcore)
	{
		hardcore_main(handle);
		return (0);
	}

	/* Ignore user interupt signals: */
//	signal(SIGINT, SIG_IGN);
//	signal(SIGQUIT, SIG_IGN);

	/* Threads for timeout */
	load_timer();

	if (*function_list)
		g_nobenchlog = 1;

	/* Running test for evry function: */
	if (g_bench == 0 && g_versus == NULL)
	{
		load_test(function_list);
		if (current_test_id == 0)
			ft_exit("can't find the specified function(s)\n");
		run_subtests(handle, 0);
	}
	else
	{
		load_bench(function_list);
		if (current_test_id == 0)
			ft_exit("can't find the specified function(s)\n");
		if (g_versus == (char *)0x1)
			ft_exit("versus: bad argument, please enter a shared library file\n");
		if (g_versus != NULL)
			if (!(handle_vs = dlopen(g_versus, RTLD_LAZY)))
				ft_exit("failed to load [%s] shared library\n", g_versus);
		run_subbench(handle, handle_vs);
	}
	munmap(g_shared_mem, 0xF00);
	return (0);
}
