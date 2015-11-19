/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 17:42:18 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/19 19:31:37 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

void			add_fun_subtest(char *name, void (*fun)(void *ptr)) {
	static int		index = 0;

	if (index >= SUBTEST_SIZE)
		return ;
	fun_subtest_table[index].fun_name = name;
	fun_subtest_table[index].fun_test_ptr = fun;
	index++;
	total_subtest++;
}

////////////////////////////////
//         ft_memset          //
////////////////////////////////

void			test_ft_memset_basic(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SET_EXPLICATION("basic memset test (fill a buffer with 'A')");

	//Auto raise: if crash => a TEST_CRASH is raised otherwise the return of the sandbox is raised
	SANDBOX_RAISE(
			char	b1[BSIZE + 1];
			char	b2[BSIZE + 1];
			
			b1[BSIZE] = 0;
			b2[BSIZE] = 0;
			memset(b1, 'A', BSIZE);
			ft_memset(b2, 'A', BSIZE);
	
			if (strcmp(b1, b2))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
		   );
}

void			test_ft_memset_fat(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SET_EXPLICATION("basic memset test (fill a buffer with 'A')");

	//Auto raise: if crash => a TEST_CRASH is raised otherwise the return of the sandbox is raised
	SANDBOX_RAISE(
			char	*b1 = (char*)malloc(sizeof(char) * (BFSIZE + 1));
			char	*b2 = (char*)malloc(sizeof(char) * (BFSIZE + 1));
			
			*b1 = 0;
			*b2 = 0;
			memset(b1, '\5', BFSIZE);
			ft_memset(b2, '\5', BFSIZE);
	
			if (strcmp(b1, b2))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
		   );
}

void			test_ft_memset_null(void *ptr) {
	typeof(memset)	*ft_memset = ptr;
	SET_EXPLICATION("your memset does not segfault when null parameter is sent !");

	SANDBOX(
			ft_memset(NULL, 'a', 12);
		   );
	if (SANDBOX_CRASH)
		ft_raise(TEST_SUCCESS);
	ft_raise(TEST_FAILED);
}

void			test_ft_memset_bad_value(void *ptr) {
	typeof(memset)	*ft_memset = ptr;
	SET_EXPLICATION("your memset does not segfault when null parameter is sent !");

	SANDBOX(
			char	buff[0xF00];

			ft_memset(buff, '\xff', -562);
		   );
	if (SANDBOX_CRASH)
		ft_raise(TEST_SUCCESS);
	ft_raise(TEST_FAILED);
}

void            test_ft_memset(void) {
	add_fun_subtest(current_fun_name, test_ft_memset_basic);
	add_fun_subtest(current_fun_name, test_ft_memset_null);
	add_fun_subtest(current_fun_name, test_ft_memset_bad_value);
	add_fun_subtest(current_fun_name, test_ft_memset_fat);
}

void            test_ft_bzero(void){ }
void            test_ft_memcpy(void){ }
void            test_ft_memccpy(void){ }
void            test_ft_memmove(void){ }
void            test_ft_memchr(void){ }
void            test_ft_memcmp(void){ }
void            test_ft_strlen(void){ }
void            test_ft_strdup(void){ }
void            test_ft_strcpy(void){ }
void            test_ft_strncpy(void){ }
void            test_ft_strcat(void){ }
void            test_ft_strncat(void){ }
void            test_ft_strlcat(void){ }
void            test_ft_strchr(void){ }
void            test_ft_strrchr(void){ }
void            test_ft_strstr(void){ }
void            test_ft_strnstr(void){ }
void            test_ft_strcmp(void){ }
void            test_ft_strncmp(void){ }
void            test_ft_atoi(void){ }
void            test_ft_isalpha(void){ }
void            test_ft_isdigit(void){ }
void            test_ft_isalnum(void){ }
void            test_ft_isascii(void){ }
void            test_ft_isprint(void){ }
void            test_ft_toupper(void){ }
void            test_ft_tolower(void){ }

void            test_ft_memalloc(void){ }
void            test_ft_memdel(void){ }
void            test_ft_strnew(void){ }
void            test_ft_strdel(void){ }
void            test_ft_strclr(void){ }
void            test_ft_striter(void){ }
void            test_ft_striteri(void){ }
void            test_ft_strmap(void){ }
void            test_ft_strmapi(void){ }
void            test_ft_strequ(void){ }
void            test_ft_strnequ(void){ }
void            test_ft_strsub(void){ }
void            test_ft_strjoin(void){ }
void            test_ft_strtrim(void){ }
void            test_ft_strsplit(void){ }
void            test_ft_itoa(void){ }
void            test_ft_putchar(void){ }
void            test_ft_putstr(void){ }
void            test_ft_putendl(void){ }
void            test_ft_putnbr(void){ }
void            test_ft_putchar_fd(void){ }
void            test_ft_putstr_fd(void){ }
void            test_ft_putendl_fd(void){ }
void            test_ft_putnbr_fd(void){ }
