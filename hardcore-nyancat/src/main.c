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
unsigned char	*buf1, *buf2;
int				ret, do_srandom = 1;
unsigned int	seed = 0x42;
size_t			page_size;
int				errors = 0;
char			*it = "<UNSET>";
pid_t			g_pid;
char			g_ret[2];
# define HIMAGIC        0x8080808080808080lu
# define LOMAGIC        0x0101010101010101lu
# define LONGCHR_NULL(x)    (((x - LOMAGIC) & HIMAGIC) != 0)

int		ft_strcmp(char *s1, char *s2)
{
	return strcmp(s1, s2);
}

# define	SANDBOX(x)	if (!(g_pid = vfork())) {x;exit(TEST_SUCCESS);} if (g_pid > 0) { wait((int*)g_ret); }
#define SANDBOX_HARDCORE(x) SANDBOX(x); if (SANDBOX_CRASH || SANDBOX_RESULT == TEST_FAILED) write_result(it, false); else write_result(it, true);

int		main(void)
{
	ncurses_init();

	__start_impls = calloc(sizeof(impl_t), 0xF00);
	SANDBOX_HARDCORE(test_main_strcmp((void *)ft_strcmp));
	SANDBOX_HARDCORE(test_main_strncmp((void *)strncmp));
	SANDBOX_HARDCORE(test_main_memset((void *)memset));
	SANDBOX_HARDCORE(test_main_memcpy((void *)memcpy));
	SANDBOX_HARDCORE(test_main_memccpy((void *)memccpy));
	SANDBOX_HARDCORE(test_main_memmove((void *)memmove));
	SANDBOX_HARDCORE(test_main_memchr((void *)memchr));
	SANDBOX_HARDCORE(test_main_memcmp((void *)memcmp));
	SANDBOX_HARDCORE(test_main_strlen((void *)strlen));
	SANDBOX_HARDCORE(test_main_strcpy((void *)strcpy));
	SANDBOX_HARDCORE(test_main_strncpy((void *)strncpy));
	SANDBOX_HARDCORE(test_main_strcat((void *)strcat));

	ncurses_loop();

	ncurses_deinit();
	return (0);
}
