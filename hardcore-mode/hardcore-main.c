/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inyancat <inyancat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/11/11 16:54:54 by inyancat          #+#    #+#             */
/*   Updated  2016/11/12 21:01:46 by inyancat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <dlfcn.h>
unsigned char	*buf1, *buf2;
int				ret, do_srandom = 1;
unsigned int	seed = 0x42;
size_t			page_size;
int				errors = 0;
char			*it = "<UNSET>";
pid_t			g_pid_h;
char			g_ret_h[2];
# define HIMAGIC        0x8080808080808080lu
# define LOMAGIC        0x0101010101010101lu
# define LONGCHR_NULL(x)    (((x - LOMAGIC) & HIMAGIC) != 0)

# define	SANDBOX(x)	if (!(g_pid_h = fork())) {x;exit(TEST_SUCCESS);} if (g_pid_h > 0) { wait((int*)g_ret_h); }
#define SANDBOX_HARDCORE(x) SANDBOX(x); if (SANDBOX_CRASH || SANDBOX_RESULT == TEST_FAILED) write_result(it, false); else write_result(it, true);

const char *mtable[] = {
	"Hardcore mode refuse to start, he think your libft isn't ready !",
	"Hardcore mode refuse to start, he need a sacrifice to be started !",
	"Hardcore mode refuse to start, you may execute the ritual before start him !",
};

struct { int (*fun)(void *); char *name; void *ft; } test_table[18] = {
	{test_main_memset, "ft_memset", NULL},
	{test_main_memcpy, "ft_memcpy", NULL},
	{test_main_memccpy, "ft_memccpy", NULL},
	{test_main_memmove, "ft_memmove", NULL},
	{test_main_memchr, "ft_memchr", NULL},
	{test_main_memcmp, "ft_memcmp", NULL},
	{test_main_strlen, "ft_strlen", NULL},
	{test_main_strcpy, "ft_strcpy", NULL},
	{test_main_strncpy, "ft_strncpy", NULL},
	{test_main_strcat, "ft_strcat", NULL},
	{test_main_strncat, "ft_strncat", NULL},
	{test_main_strchr, "ft_strchr", NULL},
	{test_main_strrchr, "ft_strrchr", NULL},
	{test_main_strstr, "ft_strstr", NULL},
	{test_main_strcmp, "ft_strcmp", NULL},
	{test_main_strncmp, "ft_strncmp", NULL},
	{NULL, NULL, NULL}
};

int		hardcore_main(void *libft_so_handler)
{
	int		i = -1;

	srand(time(NULL) + clock());
	while (test_table[++i].fun)
		if (!(test_table[i].ft = dlsym(libft_so_handler, test_table[i].name)))
		{
			printf("Hardcore mode is only for the good ones !\n");
			return (-1);
		}

	if (rand() % 10 > 3)
	{
		printf("%s\n", mtable[rand() % 3]);
		return (-1);
	}

	__start_impls = calloc(sizeof(impl_t), 0xF00);

	ncurses_init();

	i = -1;
	while (test_table[++i].fun)
	{
		it = test_table[i].name;
		SANDBOX_HARDCORE(test_table[i].fun(test_table[i].ft));
	}

	/*SANDBOX_HARDCORE(test_main_memset((void *)memset));
	SANDBOX_HARDCORE(test_main_memcpy((void *)memcpy));
	SANDBOX_HARDCORE(test_main_memccpy((void *)memccpy));
	SANDBOX_HARDCORE(test_main_memmove((void *)memmove));
	SANDBOX_HARDCORE(test_main_memchr((void *)memchr));
	SANDBOX_HARDCORE(test_main_memcmp((void *)memcmp));
	SANDBOX_HARDCORE(test_main_strlen((void *)strlen));
	SANDBOX_HARDCORE(test_main_strcpy((void *)strcpy));
	SANDBOX_HARDCORE(test_main_strncpy((void *)strncpy));
	SANDBOX_HARDCORE(test_main_strcat((void *)strcat));
	SANDBOX_HARDCORE(test_main_strncat((void *)strncat));
	SANDBOX_HARDCORE(test_main_strchr((void *)strchr));
	SANDBOX_HARDCORE(test_main_strrchr((void *)strrchr));
	SANDBOX_HARDCORE(test_main_strstr((void *)strstr));
	SANDBOX_HARDCORE(test_main_strcmp((void *)strcmp));
	SANDBOX_HARDCORE(test_main_strncmp((void *)strncmp));*/

	ncurses_loop();

	ncurses_deinit();
	return (0);
}
