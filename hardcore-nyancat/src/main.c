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
# define HIMAGIC        0x8080808080808080lu
# define LOMAGIC        0x0101010101010101lu
# define LONGCHR_NULL(x)    (((x - LOMAGIC) & HIMAGIC) != 0)

int		ft_strcmp(char *s1, char *s2)
{
	return strcmp(s1, s2);
}


int		main(void)
{
	ncurses_init();

	__start_impls = calloc(sizeof(impl_t), 0xF00);
	test_main_strcpy((void *)strcpy);
	test_main_strcmp((void *)ft_strcmp);
	test_main_memset((void *)memset);
	test_main_memcpy((void *)memcpy);
	test_main_memccpy((void *)memccpy);
	test_main_memmove((void *)memmove);
	test_main_memchr((void *)memchr);

	write_result("ft_strcmp", false);

	ncurses_loop();

	ncurses_deinit();
	return (0);
}
