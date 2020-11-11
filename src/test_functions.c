/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 17:42:18 by alelievr          #+#    #+#             */
/*   Updated: 2020/07/03 15:35:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"
#include <sys/mman.h>

#define		STRING_1	"the cake is a lie !\0I'm hidden lol\r\n"
#define		STRING_4	"phrase differente pour le test"
#define		STRING_2	"there is no stars in the sky"
#define		STRING_3	"test basic !"

#define		REG(x)		((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define		ASSERT_RETURN_VALUE(x, y) if (x != y) exit(TEST_FAILED)

#ifdef linux
# define LREG(x) REG(x)
#else
# define LREG(x) x
#endif

#define			add_fun_subtest(x) add_fun_subtest_(x, (char *)# x)
void			add_fun_subtest_(void (*fun)(void *ptr), char *funname) {
	static int		index = 0;

	if (index >= SUBTEST_SIZE || (g_nospeed == 1 && strstr(funname, "_speed")))
		return ;
	fun_subtest_table[index].fun_name = current_fun_name;
	fun_subtest_table[index].fun_test_ptr = fun;
	fun_subtest_table[index].visible = current_fun_visibility;
	fun_subtest_table[index].part = current_part;
	index++;
	total_subtest++;
}

////////////////////////////////
//         ft_memset          //
////////////////////////////////

void			test_ft_memset_basic(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SET_EXPLANATION("basic memset test (fill a buffer with 'A')");

	//Auto raise: if crash => a TEST_CRASH is raised otherwise the return of the sandbox is raised
	SANDBOX_RAISE(
			const int	size = 20;
			char	b1[BSIZE];
			char	b2[BSIZE];

			memset(b1, 'B', BSIZE);
			memset(b2, 'B', BSIZE);

			memset(b1, 'A', size);
			ft_memset(b2, 'A', size);

			if (!memcmp(b1, b2, BSIZE))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(b1, b2, size + 2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memset_unsigned(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SET_EXPLANATION("your memset does not cast the memory into unsigned chars");

	SANDBOX_RAISE(
			const int	size = 22;
			char	b1[BSIZE];
			char	b2[BSIZE];

			memset(b1, 'B', BSIZE);
			memset(b2, 'B', BSIZE);

			memset(b1, '\200', size);
			ft_memset(b2, '\200', size);

			if (!memcmp(b1, b2, BSIZE))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(b1, b2, size);
			exit(TEST_FAILED);
			);
}

void			test_ft_memset_return(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SET_EXPLANATION("your memset return address is false/your memset does not work");

	SANDBOX_RAISE(
			const int	size = 18;
			char		b1[BSIZE];

			memset(b1, 'B', BSIZE);

			char	*r1 = memset(b1, 'A', size);
			char	*r2 = ft_memset(b1, 'A', size);

			SET_DIFF_BYTES(r1, r2, size);
			ASSERT_RETURN_VALUE(r1, r2);

			r1 = memset("", 'A', (0));
			r2 = ft_memset("", 'A', 0);
			ASSERT_RETURN_VALUE(r1, r2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memset_fat(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SET_EXPLANATION("basic memset test (fill a buffer with 'A')");

	SANDBOX_RAISE(
			char	*b1 = (char*)malloc(sizeof(char) * (BFSIZE + 1));
			char	*b2 = (char*)malloc(sizeof(char) * (BFSIZE + 1));

			*b1 = 0;
			*b2 = 0;
			memset(b1, '\5', BFSIZE);
			ft_memset(b2, '\5', BFSIZE);

			if (!memcmp(b1, b2, BFSIZE))
			{
				free(b1);
				free(b2);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(b1, b2);
			free(b1);
			free(b2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memset_null(void *ptr) {
	typeof(memset)	*ft_memset = ptr;
	SET_EXPLANATION("your memset does not segfault when null parameter is sent !");

	SANDBOX_IRAISE(
			ft_memset(NULL, 'a', 12);
		   );
}

void			test_ft_memset_zero_value(void *ptr) {
	typeof(memset)	*ft_memset = ptr;
	SET_EXPLANATION("your memset change something when call with a size of 0 !");

	SANDBOX_RAISE(
			char	buff[BSIZE];
			char	buff2[BSIZE];

			memset(buff, 0, sizeof(buff));
			memset(buff2, 0, sizeof(buff2));

			ft_memset(buff, '\xff', 0);
			memset(buff2, '\xff', (0));
			if (!memcmp(buff, buff2, BSIZE))
				exit(TEST_SUCCESS);
			SET_DIFF(buff, buff2);
			exit(TEST_FAILED);
		   );
}

void			test_ft_memset_electric_memory(void *ptr) {
	typeof(memset)	*ft_memset = ptr;
	SET_EXPLANATION("your memset crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char			*buff = electric_alloc(size);

			memset(buff, 0, size);

			mprotect(buff - 4096 + size, 4096, PROT_WRITE);

			ft_memset(buff, '\xff', size);
			exit(TEST_SUCCESS);
		   );
}

void			test_ft_memset_speed(void *ptr) {
	typeof(memset)	*ft_memset = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*b1 = (char *)malloc(sizeof(char) * size);
			char	*b2 = (char *)malloc(sizeof(char) * size);
			,
			memset(b1, 'A', size);
			,
			ft_memset(b2, 'A', size);
			);
}

void            test_ft_memset(void) {
	add_fun_subtest(test_ft_memset_basic);
	add_fun_subtest(test_ft_memset_return);
	add_fun_subtest(test_ft_memset_unsigned);
	add_fun_subtest(test_ft_memset_null);
	add_fun_subtest(test_ft_memset_zero_value);
	add_fun_subtest(test_ft_memset_fat);
	add_fun_subtest(test_ft_memset_electric_memory);
	add_fun_subtest(test_ft_memset_speed);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void			test_ft_bzero_basic(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;

	SET_EXPLANATION("bzero with normal params fail !");

	SANDBOX_RAISE(
			char	str[BSIZE];
			char	str2[BSIZE];

			memset(str, 'a', BSIZE);
			memset(str2, 'a', BSIZE);

			bzero(str, 20);
			ft_bzero(str2, 20);
			if (!memcmp(str, str2, BSIZE))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(str, str2, 22);
			exit(TEST_FAILED);
			);
}

void			test_ft_bzero_zero_value(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;
	SET_EXPLANATION("your bzero change something when call with 0 !");

	SANDBOX_RAISE(
			char	buff[BSIZE];
			char	buff2[BSIZE];

			memset(buff, '\x1', sizeof(buff));
			memset(buff2, '\x1', sizeof(buff2));

			ft_bzero(buff, 0);
			bzero(buff2, (0));
			if (!memcmp(buff, buff2, BSIZE))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(buff, buff2, 10);
			exit(TEST_FAILED);
		   );
}

void			test_ft_bzero_null(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;
	SET_EXPLANATION("your bzero does not segfault when null params is sent");

	SANDBOX_IRAISE(
			ft_bzero(NULL, 0x12);
			);
}

void			test_ft_bzero_electric_memory(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;
	SET_EXPLANATION("your bzero crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char			*buff = electric_alloc(size);

			memset(buff, '\x1', size);

			mprotect(buff - 4096 + size, 4096, PROT_WRITE);

			ft_bzero(buff, size);
			exit(TEST_SUCCESS);
		   );
}

void			test_ft_bzero_speed(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*str = (char *)malloc(sizeof(char) * size);
			char	*str2 = (char *)malloc(sizeof(char) * size);

			memset(str, 'a', size);
			memset(str2, 'a', size);
			,
			bzero(str, size);
			,
			ft_bzero(str2, size);
			);
}

void            test_ft_bzero(void){
	add_fun_subtest(test_ft_bzero_basic);
	add_fun_subtest(test_ft_bzero_zero_value);
	add_fun_subtest(test_ft_bzero_null);
	add_fun_subtest(test_ft_bzero_electric_memory);
	add_fun_subtest(test_ft_bzero_speed);
}

////////////////////////////////
//         ft_memcpy          //
////////////////////////////////


void			test_ft_memcpy_basic_test(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy doesn't work with basic params");

	SANDBOX_RAISE(
			char	src[] = "test basic du memcpy !";
			char	buff1[22];
			char	buff2[22];

			memcpy(buff1, src, 22);
			ft_memcpy(buff2, src, 22);
			if (!memcmp(buff1, buff2, 22))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memcpy_return(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy's return is false/doesn't work with basic params");

	SANDBOX_RAISE(
			char	src[] = "test basic du memcpy !";
			char	buff1[22];

			char	*r1 = memcpy(buff1, src, 22);
			char	*r2 = ft_memcpy(buff1, src, 22);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			r1 = memcpy("", src, 0);
			r2 = ft_memcpy("", src, 0);
			ASSERT_RETURN_VALUE(r1, r2);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_zero_value(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not work when call with 0");

	SANDBOX_RAISE(
			char	buff[] = "test 0 du memcpy !";
			char	*src = STRING_4;
			char	buff2[] = STRING_4;

			ft_memcpy(buff2, buff, 0);
			if (!memcmp(src, buff2, strlen(buff2)))
				exit(TEST_SUCCESS);
			SET_DIFF(src, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memcpy_basic_test2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not work with basic params");

	SANDBOX_RAISE(
			char	src[] = STRING_3;
			char	buff1[] = STRING_1;
			char	buff2[] = STRING_1;

			memcpy(buff1, src, strlen(src));
			ft_memcpy(buff2, src, strlen(src));
			if (!memcmp(buff1, buff2, strlen(buff2)))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memcpy_to_small(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not segfault dst is not big enough");

	SANDBOX_IRAISE(
			ft_memcpy("", "segfaulter tu dois", 17);
			);
}

void			test_ft_memcpy_struct(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not work with basic params");
	t_test src = {"nyancat® inside", (void*)0xdeadbeef, 0x42424242424242L, 0b1010100010};

	SANDBOX_RAISE(
			char	buff1[0xF00];
			char	buff2[0xF00];

			memcpy(buff1, &src, sizeof(src));
			ft_memcpy(buff2, &src, sizeof(src));
			if (!memcmp(buff1, buff2, strlen(buff2)))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memcpy_electric_memory(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy crash because it read/write too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*src = electric_alloc(size);
			char	*buff = electric_alloc(size);

			strcpy(src, "NYANCATSH");

			mprotect(buff - 4096 + size, 4096, PROT_WRITE);
			mprotect(src - 4096 + size, 4096, PROT_READ);

			ft_memcpy(buff, src, size);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_null1(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not segv with NULL on first params");

	SANDBOX_IRAISE(
			ft_memcpy(NULL, "segfaulter tu dois", 17);
			);
}

void			test_ft_memcpy_null2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not segv with NULL on second params");

	SANDBOX_IRAISE(
			ft_memcpy("            ", NULL, 17);
			);
}

void			test_ft_memcpy_double_null1(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not behave well with NULL as both params");

	SANDBOX_RAISE(
			ft_memcpy(NULL, NULL, 0);
			);
}

void			test_ft_memcpy_double_null2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLANATION("your memcpy does not behave well with NULL as both params with size");

	SANDBOX_RAISE(
			ft_memcpy(NULL, NULL, 3);
			);
}

void			test_ft_memcpy_speed(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*src = malloc(size + 1);
			char	*buff1 = malloc(size + 1);

			memset(src, 'A', size);
			,
			memcpy(buff1, src, size);
			,
			ft_memcpy(buff1, src, size);
			);
}
void            test_ft_memcpy(void){
	add_fun_subtest(test_ft_memcpy_basic_test);
	add_fun_subtest(test_ft_memcpy_return);
	add_fun_subtest(test_ft_memcpy_zero_value);
	add_fun_subtest(test_ft_memcpy_basic_test2);
	add_fun_subtest(test_ft_memcpy_to_small);
	add_fun_subtest(test_ft_memcpy_struct);
	add_fun_subtest(test_ft_memcpy_electric_memory);
	add_fun_subtest(test_ft_memcpy_null1);
	add_fun_subtest(test_ft_memcpy_null2);
	add_fun_subtest(test_ft_memcpy_double_null1);
	add_fun_subtest(test_ft_memcpy_double_null2);
	add_fun_subtest(test_ft_memcpy_speed);
}

////////////////////////////////
//         ft_memccpy         //
////////////////////////////////

void			test_ft_memccpy_basic_test(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy doesn't work with basic params");

	SANDBOX_RAISE(
			char	src[] = "test basic du memccpy !";
			char	buff1[22];
			char	buff2[22];

			memccpy(buff1, src, 'm', 6);
			ft_memccpy(buff2, src, 'm', 6);
			if (!memcmp(buff1, buff2, 6))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_memory_unsigned(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy doesn't cast the memory into unsigned char");

	SANDBOX_RAISE(
			char	buff1[] = "abcdefghijklmnopqrstuvwxyz";
			char	buff2[] = "abcdefghijklmnopqrstuvwxyz";
			char	*src = "string with\200inside !";

			memccpy(buff1, src, '\200', 21);
			ft_memccpy(buff2, src, '\200', 21);

			if (!memcmp(buff1, buff2, 21))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_stop_char_unsigned(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy doesn't cast the stop-character into unsigned char");

	SANDBOX_RAISE(
			char	buff1[] = "abcdefghijklmnopqrstuvwxyz";
			char	buff2[] = "abcdefghijklmnopqrstuvwxyz";
			char	*src = "string with\200inside !";

			memccpy(buff1, src, 0600, 21);
			ft_memccpy(buff2, src, 0600, 21);

			if (!memcmp(buff1, buff2, 21))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_return(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy's return is false/doesn't work with basic params");

	SANDBOX_RAISE(
			char	src[] = "test basic du memccpy !";
			char	buff1[22];

			memset(buff1, 0, sizeof(buff1));

			char	*r1 = memccpy(buff1, src, 'm', 22);
			char	*r2 = ft_memccpy(buff1, src, 'm', 22);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			r1 = memccpy("", src, 'm', 0);
			r2 = ft_memccpy("", src, 'm', 0);
			SET_DIFF("", "");
			ASSERT_RETURN_VALUE(r1, r2);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memccpy_not_found(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not work with not found char");

	SANDBOX_RAISE(
			char	src[] = "test basic du memccpy !";
			char	buff1[22];
			char	buff2[22];

			char	*r1 = memccpy(buff1, src, 'z', 22);
			char	*r2 = ft_memccpy(buff2, src, 'z', 22);
			if (r1 == r2)
				exit(TEST_SUCCESS);
			SET_DIFF(r1, r2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_zero_value(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not work when call with 0");

	SANDBOX_RAISE(
			char	buff[] = "test 0 \0du\0 memccpy !";
			char	*src = STRING_4;
			char	buff2[] = STRING_4;

			ft_memccpy(buff2, buff, '\0', 0);
			if (!memcmp(src, buff2, strlen(buff2)))
				exit(TEST_SUCCESS);
			SET_DIFF(src, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_basic_test2(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not work with basic params");

	SANDBOX_RAISE(
			char	src[] = STRING_3;
			char	buff1[] = STRING_1;
			char	buff2[] = STRING_1;

			memccpy(buff1, src, ' ', strlen(src));
			ft_memccpy(buff2, src, ' ', strlen(src));
			if (!memcmp(buff1, buff2, strlen(buff2)))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_to_small(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not segfault dst is not big enough");

	SANDBOX_IRAISE(
			ft_memccpy("", "segfaulter tu dois", '\0', 17);
			);
}

void			test_ft_memccpy_struct(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not work with basic params");
	t_test src = {"nyancat® inside", (void*)0xdeadbeef, 0x42424242424242L, 0b1010100010};

	SANDBOX_RAISE(
			char	buff1[0xF00];
			char	buff2[0xF00];

			memccpy(buff1, &src, '\x42', sizeof(src));
			ft_memccpy(buff2, &src, '\x42', sizeof(src));
			if (!memcmp(buff1, buff2, strlen(buff2)))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);

}

void			test_ft_memccpy_electric_memory(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy crash because it read/write too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*src = electric_alloc(size);
			char	*buff = electric_alloc(size);

			strcpy(src, "NYANCATSH");

			mprotect(buff - 4096 + size, 4096, PROT_WRITE);
			mprotect(src - 4096 + size, 4096, PROT_READ);

			ft_memccpy(buff, &src, '\0', size);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memccpy_null1(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not segv with NULL on first params");

	SANDBOX_IRAISE(
			ft_memccpy(NULL, "segfaulter tu dois", 'e', 17);
			);
}

void			test_ft_memccpy_null2(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;
	SET_EXPLANATION("your memccpy does not segv with NULL on second params");

	SANDBOX_IRAISE(
			ft_memccpy("            ", NULL, ' ', 17);
			);
}

void			test_ft_memccpy_speed(void *ptr) {
	typeof(memccpy)	*ft_memccpy = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*src = malloc(size + 1);
			char	*buff1 = malloc(size + 1);

			memset(src, 'A', size);
			,
			memccpy(buff1, src, '\\', size);
			,
			ft_memccpy(buff1, src, '\\', size);
			);
}

void            test_ft_memccpy(void){
	add_fun_subtest(test_ft_memccpy_basic_test);
	add_fun_subtest(test_ft_memccpy_memory_unsigned);
	add_fun_subtest(test_ft_memccpy_stop_char_unsigned);
	add_fun_subtest(test_ft_memccpy_return);
	add_fun_subtest(test_ft_memccpy_not_found);
	add_fun_subtest(test_ft_memccpy_zero_value);
	add_fun_subtest(test_ft_memccpy_basic_test2);
	add_fun_subtest(test_ft_memccpy_to_small);
	add_fun_subtest(test_ft_memccpy_struct);
	add_fun_subtest(test_ft_memccpy_electric_memory);
	add_fun_subtest(test_ft_memccpy_null1);
	add_fun_subtest(test_ft_memccpy_null2);
	add_fun_subtest(test_ft_memccpy_speed);
}

////////////////////////////////
//         ft_memmove         //
////////////////////////////////

void			test_ft_memmove_basic(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not work with basic input");

	SANDBOX_RAISE(
			char	*src = "this is a good nyancat !\r\n";
			char	dst1[0xF0];
			char	dst2[0xF0];
			int		size = strlen(src);

			memmove(dst1, src, size);
			ft_memmove(dst2, src, size);
			if (!memcmp(dst1, dst2, size))
				exit(TEST_SUCCESS);
			SET_DIFF(dst1, dst2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memmove_return(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove's return is false/does not work with basic input");

	SANDBOX_RAISE(
			char	*src = "thanks to @apellicc for this test !\r\n";
			char	dst1[0xF0];
			int		size = strlen(src);

			char	*r1 = memmove(dst1, src, size);
			char	*r2 = ft_memmove(dst1, src, size);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);
			r1 = memmove("", "" - 1, 0);
			r2 = ft_memmove("", "" - 1, 0);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memmove_null_byte(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not work with \\0 and others unsigned char codes");

	SANDBOX_RAISE(
			char	*src = "thi\xffs i\xfas \0a g\xde\xadood \0nyan\0cat\0 !\r\n";
			int		size = 33;
			char	dst1[0xF0];
			char	dst2[0xF0];

			memmove(dst1, src, size);
			ft_memmove(dst2, src, size);
			if (!memcmp(dst1, dst2, size))
				exit(TEST_SUCCESS);
			SET_DIFF(dst1, dst2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memmove_long_int(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not work with integers copy");

	SANDBOX_RAISE(
			unsigned long	src = 0xdeadbeef;
			int		size = sizeof(src);
			unsigned long	dst1;
			unsigned long	dst2;

			memmove(&dst1, &src, size);
			ft_memmove(&dst2, &src, size);
			if (!memcmp(&dst1, &dst2, size))
				exit(TEST_SUCCESS);
			exit(TEST_FAILED);
			);
}

void			test_ft_memmove_overlap(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not support the overlap (test 1)");

	SANDBOX_RAISE(
			char	dst1[0xF0];
			char	dst2[0xF0];
			char	*data = "thiß ß\xde\xad\xbe\xeftriñg will be øvérlapéd !\r\n";
			int		size = 0xF0 - 0xF;

			memset(dst1, 'A', sizeof(dst1));
			memset(dst2, 'A', sizeof(dst2));

			memcpy(dst1, data, strlen(data));
			memcpy(dst2, data, strlen(data));
			memmove(dst1 + 3, dst1, size);
			ft_memmove(dst2 + 3, dst2, size);
			if (!memcmp(dst1, dst2, size))
				exit(TEST_SUCCESS);
			SET_DIFF(dst1, dst2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memmove_overlap_rev(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not support the overlap (test 2)");

	SANDBOX_RAISE(
			char	dst1[0xF0];
			char	dst2[0xF0];
			char	*data = "thiß ß\xde\xad\xbe\xeftriñg will be øvérlapéd !\r\n";
			int		size = 0xF0 - 0xF;

			memset(dst1, 'A', sizeof(dst1));
			memset(dst2, 'A', sizeof(dst2));

			memcpy(dst1, data, strlen(data));
			memcpy(dst2, data, strlen(data));
			memmove(dst1, dst1 + 3, size);
			ft_memmove(dst2, dst2 + 3, size);
			if (!memcmp(dst1, dst2, size))
				exit(TEST_SUCCESS);
			SET_DIFF(dst1, dst2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memmove_hard(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not work with a 128Mo data input !");

	SANDBOX_RAISE(
			int		size = 128 * 1024 * 1024;
			char	*dst = (char *)malloc(sizeof(char) * size);
			char	*data = (char *)malloc(sizeof(char) * size);

			memset(data, 'A', size);
			if (!dst)
				exit(TEST_INVISIBLE);
			ft_memmove(dst, data, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memmove_electric_memory(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove crash because it read too many bytes or attempt to read on dst !");

	SANDBOX_RAISE(
			int		size = 10;
			char	*dst = electric_alloc(size);
			char	*data = electric_alloc(size);

			memset(data, 'A', size);

			mprotect(dst - 4096 + size, 4096, PROT_WRITE);
			mprotect(data - 4096 + size, 4096, PROT_READ);

			ft_memmove(dst, data, size);

			dst = electric_alloc_rev(size);
			data = electric_alloc_rev(size);

			memset(data, 'A', size);

			mprotect(dst- size, 4096, PROT_WRITE);
			mprotect(data - size, 4096, PROT_READ);

			ft_memmove(dst, data, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memmove_null1(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not segfault when null params is sent");

	SANDBOX_IRAISE(
			char	b[0xF0];

			ft_memmove(NULL, b, 5);
			);
}

void			test_ft_memmove_null2(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not segfault when null params is sent");

	SANDBOX_IRAISE(
			char	b[0xF0];

			ft_memmove(b, NULL, 5);
			);
}

void			test_ft_memmove_double_null1(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not well with NULL as both parameters");

	SANDBOX_RAISE(
			ft_memmove(NULL, NULL, 0);
			);
}

void			test_ft_memmove_double_null2(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not well with NULL as both parameters and size");

	SANDBOX_RAISE(
			ft_memmove(NULL, NULL, 5);
			);
}

void			test_ft_memmove_same_pointer(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove does not support the overlap");

	SANDBOX_RAISE(
			char	data1[] = STRING_1;
			char	data2[] = STRING_1;
			int		size = strlen(STRING_1);

			memmove(data1, data1, size);
			ft_memmove(data2, data2, size);
			if (!memcmp(data1, data2, size))
				exit(TEST_SUCCESS);
			SET_DIFF(data1, data2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memmove_malloc(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;
	SET_EXPLANATION("your memmove use malloc ? why ?");

	SANDBOX_RAISE(
			char	*src = STRING_1;
			char	buff[0xF0];

			MALLOC_NULL;
			char *ret = ft_memmove(buff, src, 100);
			MALLOC_RESET;
			if (ret == NULL) {
				SET_DIFF(STRING_1, ret);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
	(void)ft_memmove;
}

void			test_ft_memmove_speed(void *ptr) {
	typeof(memmove)		*ft_memmove = ptr;

	SANDBOX_SPEED(
			int		size = BFSIZE * 128;
			char	*dst = (char *)malloc(sizeof(char) * size);
			char	*data = (char *)malloc(sizeof(char) * size);

			memset(data, 'A', size);
			,
			memmove(dst, data, size);
			,
			ft_memmove(dst, data, size);
			);
}

void            test_ft_memmove(void){
	add_fun_subtest(test_ft_memmove_basic);
	add_fun_subtest(test_ft_memmove_return);
	add_fun_subtest(test_ft_memmove_null_byte);
	add_fun_subtest(test_ft_memmove_long_int);
	add_fun_subtest(test_ft_memmove_overlap);
	add_fun_subtest(test_ft_memmove_overlap_rev);
	add_fun_subtest(test_ft_memmove_same_pointer);
	add_fun_subtest(test_ft_memmove_hard);
	add_fun_subtest(test_ft_memmove_electric_memory);
	add_fun_subtest(test_ft_memmove_null1);
	add_fun_subtest(test_ft_memmove_null2);
	add_fun_subtest(test_ft_memmove_double_null1);
	add_fun_subtest(test_ft_memmove_double_null2);
	add_fun_subtest(test_ft_memmove_malloc);
	add_fun_subtest(test_ft_memmove_speed);
}

////////////////////////////////
//         ft_memchr          //
////////////////////////////////

void			test_ft_memchr_basic(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not work with basic input");

	SANDBOX_RAISE(
			char			*src = "/|\x12\xff\x09\x42\042\42|\\";
			int				size = 10;

			if (memchr(src, '\x42', size) == ft_memchr(src, '\x42', size))
				exit(TEST_SUCCESS);
			SET_DIFF(memchr(src, '\x42', size), ft_memchr(src, '\x42', size))
			exit(TEST_FAILED);
			);
}

void			test_ft_memchr_unsigned(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not cast in unsigned the memory");

	SANDBOX_RAISE(
			char			*src = "/|\x12\xff\x09\x42\2002\42|\\";
			int				size = 10;

			if (memchr(src, '\200', size) == ft_memchr(src, '\200', size))
				exit(TEST_SUCCESS);
			SET_DIFF(memchr(src, '\200', size), ft_memchr(src, '\200', size));
			exit(TEST_FAILED);
			);
}

void			test_ft_memchr_not_found1(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not work");

	SANDBOX_RAISE(
			char			*src = "/|\x12\xff\x09\x42\042\42|\\";
			int				size = 2;

			if (memchr(src, '\x42', size) == ft_memchr(src, '\x42', size))
				exit(TEST_SUCCESS);
			SET_DIFF(memchr(src, '\x42', size), ft_memchr(src, '\x42', size))
			exit(TEST_FAILED);
			);
}

void			test_ft_memchr_not_found2(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not work");

	SANDBOX_RAISE(
			char			*src = "/|\x12\xff\x09\x42\042\42|\\";
			int				size = 10;

			if (memchr(src, '\xde', size) == ft_memchr(src, '\xde', size))
				exit(TEST_SUCCESS);
			SET_DIFF(memchr(src, '\xde', size), ft_memchr(src, '\xde', size));
			exit(TEST_FAILED);
			);
}


void			test_ft_memchr_faraway(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not work on basic input");

	SANDBOX_RAISE(
			char	src[] = "iawhf\200wf208awfh\xakufi21f4\x42w7fwa1f";

			if (memchr(src, '\x42', sizeof(src)) == ft_memchr(src, '\x42', sizeof(src)))
				exit(TEST_SUCCESS);
			exit(TEST_FAILED);
			);
}

void			test_ft_memchr_electric_memory(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr crash because it read too many bytes from s or attempt to write on s !");

	SANDBOX_RAISE(
			int				size = 15;
			char			*src = electric_alloc(size);

			strcpy(src, ":(){ :|: & };:");

			mprotect(src - 4096 + size, 4096, PROT_READ);

			ft_memchr(src, '\xde', size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memchr_null_byte(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr failed to find a \\0");

	SANDBOX_RAISE(
			char			*src = "/|\x12\xff\x09\0\x42\042\0\42|\\";
			int				size = 12;

			if (memchr(src, '\0', size) == ft_memchr(src, '\0', size))
				exit(TEST_SUCCESS);
			exit(TEST_FAILED);
			);
}

void			test_ft_memchr_null(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not segfault when null param is sent");

	SANDBOX_IRAISE(
			ft_memchr(NULL, '\0', 0x20);
			);
}

void			test_ft_memchr_speed(void *ptr) {
	typeof(memchr)		*ft_memchr = ptr;
	SET_EXPLANATION("your memchr does not work");

	SANDBOX_SPEED(
			size_t			size = BFSIZE * 16;
			char			*src = malloc(size);

			memset(src, 'A', size);
			,
			memchr(src, '\xde', size);
			,
			ft_memchr(src, '\xde', size);
			);
}

void            test_ft_memchr(void) {
	add_fun_subtest(test_ft_memchr_basic);
	add_fun_subtest(test_ft_memchr_unsigned);
	add_fun_subtest(test_ft_memchr_not_found1);
	add_fun_subtest(test_ft_memchr_not_found2);
	add_fun_subtest(test_ft_memchr_faraway);
	add_fun_subtest(test_ft_memchr_electric_memory);
	add_fun_subtest(test_ft_memchr_null_byte);
	add_fun_subtest(test_ft_memchr_null);
	add_fun_subtest(test_ft_memchr_speed);
}

////////////////////////////////
//         ft_memcmp          //
////////////////////////////////

void			test_ft_memcmp_basic(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "\xff\xaa\xde\x12MACOSX";
			char	*s2 = "\xff\xaa\xde\x12MACOSX";
			size_t	size = 10;

			int		i1 = LREG(simple_memcmp(s1, s2, size));
			int		i2 = LREG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memcmp_basic1(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "\xff\xaa\xde\x12WXYZ";
			char	*s2 = "\xff\xaa\xde\x12MACOSX";
			size_t	size = 7;

			//macos memcmp function return 1 or -1 in this case, Apple why ?
			int		i1 = REG(simple_memcmp(s1, s2, size));
			int		i2 = REG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memcmp_basic2(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "\xff\xaa\xde\x12";
			char	*s2 = "\xff\xaa\xde\x12MACOSAAAAA";
			size_t	size = 4;

			int i1 = LREG(simple_memcmp(s1, s2, size));
			int	i2 = LREG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memcmp_basic3(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "\xff\xaa\xde\xffMACOSX\xff";
			char	*s2 = "\xff\xaa\xde\x02";
			size_t	size = 8;

			int i1 = LREG(simple_memcmp(s1, s2, size));
			int	i2 = LREG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memcmp_hidden(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp stop at \\0");

	SANDBOX_RAISE(
			char	*s1 = "atoms\0\0\0\0";
			char	*s2 = "atoms\0abc";
			size_t	size = 8;

			int i1 = LREG(simple_memcmp(s1, s2, size));
			int	i2 = LREG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
		);
}

void			test_ft_memcmp_unsigned(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not cast the memory in unsigned char");

	SANDBOX_RAISE(
			char	*s1 = "\xff\xaa\xde\200";
			char	*s2 = "\xff\xaa\xde\0";
			size_t	size = 8;

			int i1 = LREG(simple_memcmp(s1, s2, size));
			int	i2 = LREG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memcmp_null_byte(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "\xff\0\0\xaa\0\xde\xffMACOSX\xff";
			char	*s2 = "\xff\0\0\xaa\0\xde\x00MBS";
			size_t	size = 9;

			int i1 = LREG(simple_memcmp(s1, s2, size));
			int	i2 = LREG(ft_memcmp(s1, s2, size));

			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_memcmp_electric_memory(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp crash because it read too many bytes !");

	SANDBOX_RAISE(
			size_t	size = 10;
			char	*b1 = electric_alloc(size);
			char	*b2 = electric_alloc(size);

			strcpy(b1, ".........");
			strcpy(b2, ".........");

			mprotect(b1 - 4096 + size, 4096, PROT_READ);
			mprotect(b2 - 4096 + size, 4096, PROT_READ);

			ft_memcmp(b1, b2, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memcmp_null1(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b1[] = "nyancat";

			ft_memcmp(NULL, b1, 4);
			);
}

void			test_ft_memcmp_null2(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b1[] = "nyancat";

			ft_memcmp(b1, NULL, 4);
			);
}

void			test_ft_memcmp_speed(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not cast the memory in unsigned char");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*s1 = malloc(sizeof(char) * size);
			char	*s2 = malloc(sizeof(char) * size);

			memset(s1, 'A', size);
			memset(s2, 'A', size);
			,
			memcmp(s1, s2, size);
			,
			ft_memcmp(s1, s2, size);
			);
}

void            test_ft_memcmp(void){
	add_fun_subtest(test_ft_memcmp_basic);
	add_fun_subtest(test_ft_memcmp_basic1);
	add_fun_subtest(test_ft_memcmp_basic2);
	add_fun_subtest(test_ft_memcmp_basic3);
	add_fun_subtest(test_ft_memcmp_unsigned);
	add_fun_subtest(test_ft_memcmp_hidden);
	add_fun_subtest(test_ft_memcmp_null_byte);
	add_fun_subtest(test_ft_memcmp_electric_memory);
	add_fun_subtest(test_ft_memcmp_null1);
	add_fun_subtest(test_ft_memcmp_null2);
	add_fun_subtest(test_ft_memcmp_speed);
}

////////////////////////////////
//         ft_strlen          //
////////////////////////////////

void			test_ft_strlen_basic(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with basic test");

	SANDBOX_RAISE(
			int		r1;
		  	int		r2;
			char	*str = "sais-tu compter ?";

			if ((r1 = ft_strlen(str)) != (r2 = strlen(str))) {
				SET_DIFF_INT(r1, r2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strlen_random(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with basic test");

	SANDBOX_RAISE(
			int		r1;
			int		r2;
			char	buff[0x100];
			int		offset;
			int		rnd_fd = open("/dev/urandom", O_RDONLY);
			const int test_count = 9001;

			if (rnd_fd < 0)
				exit(TEST_SUCCESS);

			for (int i = 0; i < test_count; i++)
				if (read(rnd_fd, buff, sizeof(buff)) > 0)
				{
					offset = rand() % sizeof(buff);
					buff[sizeof(buff) - 1] = 0;
					if ((r1 = ft_strlen(buff + offset)) != (r2 = strlen(buff + offset))) {
						SET_DIFF_INT(r1, r2);
						exit(TEST_FAILED);
					}
				}
				else
					exit(TEST_SUCCESS);
			);
}

void			test_ft_strlen_bad_opti(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with basic test / bad optimization broke it");

	SANDBOX_RAISE(
			int		r1;
			int		r2;
			const char *str = "01234567, AAAAAA, abc\xba e, ......, end of string !";

			if ((r1 = ft_strlen(str)) != (r2 = strlen(str))) {
				SET_DIFF_INT(r1, r2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strlen_unicode(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with unicode string");

	SANDBOX_RAISE(
			int		r1;
		  	int		r2;
			char	*str = "♫♪.ılılıll|̲̅̅●̲̅̅|̲̅̅=̲̅̅|̲̅̅●̲̅̅|llılılı.♫♪";

			if ((r1 = ft_strlen(str)) != (r2 = strlen(str))) {
				SET_DIFF_INT(r1, r2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strlen_empty(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with an empty string");

	SANDBOX_RAISE(

			if (ft_strlen(""))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);

			);

}

void			test_ft_strlen_null(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen does not segv when null is sended");

	SANDBOX_IRAISE(
			ft_strlen(NULL);
			);
}

void			test_ft_strlen_zero(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with a \\0 inside the string");

	SANDBOX_RAISE(
			int			r1;
			int			r2;
			const char	*s = "HAHAHAHA \0 TA FAIL XD";

			if ((r2 = ft_strlen(s)) != (r1 = strlen(s))) {
				SET_DIFF_INT(r1, r2)
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strlen_electric_memory(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 37;
			char	*s = electric_alloc(size);

			strcpy(s, "be carefull with electrical memory !");

			mprotect(s - 4096 + size, 4096, PROT_READ);

			ft_strlen(s);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strlen_little(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLANATION("your strlen doesn't work with a non aligned on 8 address");

	SANDBOX_RAISE(
			int			r1;
			int			r2;
			const char	*s = "YOLO";

			if ((unsigned long)s & 0b111)
				s++;

			if ((r2 = ft_strlen(s)) != (r1 = strlen(s))) {
				SET_DIFF_INT(r1, r2)
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strlen_speed(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*str = malloc(size + 1);

			memset(str, 'A', size);
			str[size] = 0;
			,
			strlen(str);
			,
			ft_strlen(str);
			);
}

void            test_ft_strlen(void){

	add_fun_subtest(test_ft_strlen_basic);
	add_fun_subtest(test_ft_strlen_random);
	add_fun_subtest(test_ft_strlen_bad_opti);
	add_fun_subtest(test_ft_strlen_unicode);
	add_fun_subtest(test_ft_strlen_null);
	add_fun_subtest(test_ft_strlen_empty);
	add_fun_subtest(test_ft_strlen_zero);
	add_fun_subtest(test_ft_strlen_electric_memory);
	add_fun_subtest(test_ft_strlen_little);
	add_fun_subtest(test_ft_strlen_speed);
}

////////////////////////////////
//         ft_strdup          //
////////////////////////////////

void			test_ft_strdup_last_char(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("your strdup does not add \\0 at the end of the string");

	SANDBOX_RAISE(
			char 	*str;
			char	*tmp = "HAHAHA \0 tu me vois pas !";

			MALLOC_MEMSET;
			str = ft_strdup(tmp);
			MALLOC_RESET;
			if (strcmp(str, tmp))
				exit(TEST_FAILED);
			free(str);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strdup_zero(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("your strdup don't work with empty string");

	SANDBOX_RAISE(
			char 	*str;
			char	*tmp = "";

			str = ft_strdup(tmp);
			if (strcmp(str, tmp))
				exit(TEST_FAILED);
			free(str);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strdup_size(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("your strdup did not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char 	*str;
			char	*tmp = "this is a normal test";
			int		r_size = strlen(tmp);
			int		size;

			MALLOC_SIZE;
			str = ft_strdup(tmp);
			MALLOC_RESET;
			size = get_last_malloc_size();
			if (size == r_size + 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(r_size + 1, size);
			exit(TEST_KO);
			);
}

void			test_ft_strdup_null(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("your strdup does not segv with NULL parameter");

	SANDBOX_IRAISE(
			ft_strdup(NULL);
			);
}

void			test_ft_strdup_malloc_null(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("you didn't protect your malloc return");

	SANDBOX_RAISE(
			char	*ptr;

			MALLOC_NULL;
			ptr = ft_strdup("lol");
			MALLOC_RESET;
			if (!ptr)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ptr);
			exit(TEST_FAILED);
			);
	(void)ft_strdup;
}

void			test_ft_strdup_basic(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("your strdup doesn't work with basic input");

	SANDBOX_RAISE(
			char	*str;
			char	*tmp = "I malloc so I am.";

			str = ft_strdup(tmp);
			if (strcmp(str, tmp))
				exit(TEST_FAILED);
			free(str);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strdup_electric_memory(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLANATION("your strdup crash because it read too many bytes !");

	SANDBOX_RAISE(
			char	*tmp = electric_alloc(10);

			strcpy(tmp, "\xd\xe\xa\xd\xb\xe\xe\xf.");

			mprotect(tmp - 4096 + 10, 4096, PROT_READ);

			ft_strdup(tmp);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strdup_speed(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*str = malloc(size + 1);

			memset(str, 'A', size);
			str[size] = 0;
			,
			strdup(str);
			,
			ft_strdup(str);
			);

}

void            test_ft_strdup(void) {

	add_fun_subtest(test_ft_strdup_malloc_null);
	add_fun_subtest(test_ft_strdup_basic);
	add_fun_subtest(test_ft_strdup_zero);
	add_fun_subtest(test_ft_strdup_size);
	add_fun_subtest(test_ft_strdup_last_char);
	add_fun_subtest(test_ft_strdup_electric_memory);
	add_fun_subtest(test_ft_strdup_null);
	add_fun_subtest(test_ft_strdup_speed);
}

////////////////////////////////
//         ft_calloc          //
////////////////////////////////

void			test_ft_calloc_free(void *ptr) {
	void *	(*ft_calloc)(size_t) = ptr;
	SET_EXPLANATION("your calloc don't allocate memory");

	SANDBOX_RAISE(
			free(ft_calloc(42));
			);
}

void			test_ft_calloc_malloc_null(void *ptr) {
	typeof(calloc)	*ft_calloc = ptr;
	SET_EXPLANATION("you didn't protect your malloc return");

	SANDBOX_RAISE(
			char	*ptr;

			MALLOC_NULL;
			ptr = ft_calloc(4, 20);
			MALLOC_RESET;
			if (!ptr)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ptr);
			exit(TEST_FAILED);
			);
	(void)ft_calloc;
}

void			test_ft_calloc_zero(void *ptr) {
	typeof(calloc)	*ft_calloc = ptr;
	SET_EXPLANATION("your calloc don't work with 0 size");

	SANDBOX_RAISE(
			void *str = ft_calloc(0, 0);
			if (str == NULL)
				exit(TEST_FAILED);
			free(str);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_calloc_basic(void *ptr) {
	typeof(calloc)	*ft_calloc = ptr;
	SET_EXPLANATION("your calloc don't work with empty string");

	SANDBOX_RAISE(
            int     size = 8539;

            MALLOC_MEMSET;
			void * d1 = ft_calloc(size, sizeof(int));
            MALLOC_RESET;
			void * d2 = calloc(size, sizeof(int));
			if (memcmp(d1, d2, size * sizeof(int)))
				exit(TEST_FAILED);
			free(d1);
			free(d2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_calloc_size(void *ptr) {
	typeof(calloc)	*ft_calloc = ptr;
	SET_EXPLANATION("your calloc did not allocate the good size");

	SANDBOX_RAISE(
			int		size;
            int     count = 42 * 3;

			MALLOC_SIZE;
			ft_calloc(count, sizeof(char));
			MALLOC_RESET;
			size = get_last_malloc_size();
			if (size == count * sizeof(char))
				exit(TEST_SUCCESS);
			SET_DIFF_INT((int)(count * sizeof(char)), size);
			exit(TEST_KO);
			);
}

void			test_ft_calloc_too_big(void *ptr) {
	typeof(calloc)	*ft_calloc = ptr;
	SET_EXPLANATION("your calloc does not work with too large numbers");

	SANDBOX_RAISE(
		    void *d1 = ft_calloc(ULONG_MAX, sizeof(char));
		    void *d2 = ft_calloc(2, 1073741829); // overflow the int max
		    void *d3 = ft_calloc(1, ULONG_MAX);
			if (d1 || d3 || !d2)
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);
}

void            test_ft_calloc(void) {

	add_fun_subtest(test_ft_calloc_free);
	add_fun_subtest(test_ft_calloc_malloc_null);
	add_fun_subtest(test_ft_calloc_basic);
	add_fun_subtest(test_ft_calloc_zero);
	add_fun_subtest(test_ft_calloc_size);
	//add_fun_subtest(test_ft_calloc_too_big);
}

////////////////////////////////
//         ft_strcpy          //
////////////////////////////////

void			test_ft_strcpy_basic(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy does not work with basic input");

	SANDBOX_RAISE(
			char	*src = "--> nyancat <--\n\r";
			char	dst1[30];
			char	dst2[30];

			memset(dst1, 'A', sizeof(dst1));
			memset(dst2, 'A', sizeof(dst2));

			strcpy(dst1, src);
			ft_strcpy(dst2, src);
			if (strcmp(dst1, dst2)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_return(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy return value is false / your strcpy does not work");

	SANDBOX_RAISE(
			char	*src = "--> nyancat <--\n\r";
			char	dst1[30];

			memset(dst1, 'a', sizeof(dst1));

			char	*r1 = strcpy(dst1, src);
			char	*r2 = ft_strcpy(dst1, src);

			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			char	b[1];
			r1 = strcpy(b, "");
			r2 = ft_strcpy(b, "");

			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_unicode(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy does not support unicode ?");

	SANDBOX_RAISE(
			char	*src = "œð˛ʼˇ,´˛ˀ-ºª•¶ªˆ§´";
			char	dst1[80];
			char	dst2[80];

			memset(dst1, 'a', sizeof(dst1));
			memset(dst2, 'a', sizeof(dst2));

			strcpy(dst1, src);
			ft_strcpy(dst2, src);
			if (strcmp(dst1, dst2)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_overflow(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy overflow the dst !");

	SANDBOX_RAISE(
			char	*src = "AAAA";
			char	dst1[80];
			char	dst2[80];

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strcpy(dst1, src);
			ft_strcpy(dst2, src);
			if (memcmp(dst1, dst2, 20)) {
				SET_DIFF_BYTES(dst1, dst2, 20);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_zero(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy does not set \\0 at the end of the string");

	SANDBOX_RAISE(
			char	*src = "zero test !";
			char	dst1[80];
			size_t	len = strlen(src);

			memset(dst1, 'A', 79);
			ft_strcpy(dst1, src);
			if (dst1[len]) {
				SET_DIFF(src, dst1);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_empty(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy does not work with an empty string");

	SANDBOX_RAISE(
			char	*src = "";
			char	dst1[30];
			char	dst2[30];

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strcpy(dst1, src);
			ft_strcpy(dst2, src);
			if (strcmp(dst1, dst2)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_electric_memory(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy crash because it read too many bytes or attempt to read on dst !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char			*src = "AAAAAAAAB";
			char			*dst = electric_alloc(size);

			mprotect(dst - 4096 + size, 4096, PROT_WRITE);

			ft_strcpy(dst, src);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcpy_null1(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strcpy(NULL, "olol");
			);
}

void			test_ft_strcpy_null2(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;
	SET_EXPLANATION("your strcpy does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF0] = {0};

			ft_strcpy(b, NULL);
			);
}

void			test_ft_strcpy_speed(void *ptr) {
	typeof(strcpy)	*ft_strcpy = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 2;
			char	*src = malloc(size + 1);
			char	*dst = malloc(size + 1);

			memset(src, 'A', size);
			src[size] = 0;
			,
			strcpy(dst, src);
			,
			ft_strcpy(dst, src);
			);
}

void            test_ft_strcpy(void) {
	add_fun_subtest(test_ft_strcpy_basic);
	add_fun_subtest(test_ft_strcpy_return);
	add_fun_subtest(test_ft_strcpy_unicode);
	add_fun_subtest(test_ft_strcpy_overflow);
	add_fun_subtest(test_ft_strcpy_zero);
	add_fun_subtest(test_ft_strcpy_empty);
	add_fun_subtest(test_ft_strcpy_electric_memory);
	add_fun_subtest(test_ft_strcpy_null1);
	add_fun_subtest(test_ft_strcpy_null2);
	add_fun_subtest(test_ft_strcpy_speed);
}

////////////////////////////////
//         ft_strncpy         //
////////////////////////////////

void			test_ft_strncpy_basic(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not work with basic input");

	SANDBOX_RAISE(
			char	*src = "--> nyancat <--\n\r";
			char	dst1[30];
			char	dst2[30];
			size_t	max = 12;

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strncpy(dst1, src, max);
			ft_strncpy(dst2, src, max);
			if (memcmp(dst1, dst2, 29)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_return(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy return value is false / your strncpy does not works");

	SANDBOX_RAISE(
			char	*src = "--> nyancat <--\n\r";
			char	dst1[30];
			size_t	max = 12;

			memset(dst1, 'B', sizeof(dst1));

			char	*r1 = strncpy(dst1, src, max);
			char	*r2 = ft_strncpy(dst1, src, max);

			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_unicode(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not support unicode ?");

	SANDBOX_RAISE(
			char	*src = "œð˛ʼˇ,´˛ˀ-ºª•¶ªˆ§´";
			char	dst1[80];
			char	dst2[80];
			size_t	max = 16;

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strncpy(dst1, src, max);
			ft_strncpy(dst2, src, max);
			if (memcmp(dst1, dst2, sizeof(dst1) - 1)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_empty(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not work with an empty string");

	SANDBOX_RAISE(
			char	*src = "";
			char	dst1[30];
			char	dst2[30];
			size_t	max = 29;

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strncpy(dst1, src, max);
			ft_strncpy(dst2, src, max);
			if (memcmp(dst1, dst2, 29)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
	(void)ft_strncpy;
}

void			test_ft_strncpy_zero(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not work with an 0 as lenght");

	SANDBOX_RAISE(
			char	*src = "this is a string with a \0 inside";
			char	dst1[50];
			char	dst2[50];
			size_t	max = 31;

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strncpy(dst1, src, max);
			ft_strncpy(dst2, src, max);
			if (strcmp(dst1, dst2)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_fill(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not fill with \\0 the rest of the dest buffer");

	SANDBOX_RAISE(
			char	*src = "stars";
			char	dst1[50];
			char	dst2[50];
			size_t	max = 50;

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strncpy(dst1, src, max);
			ft_strncpy(dst2, src, max);
			if (memcmp(dst1, dst2, max)) {
				SET_DIFF(dst1, dst2);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_number_0(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy put the bad number of \\0 at the end of the string");

	SANDBOX_RAISE(
			char	*src = "len\0AAAAAAAAAAAA";
			char	dst1[] = "BBBBBBBBBBBBBBBBBBBB";
			char	dst2[] = "BBBBBBBBBBBBBBBBBBBB";
			size_t	len = strlen(dst1);

			strncpy(dst1, src, 7);
			ft_strncpy(dst2, src, 7);
			for (size_t i = 0; i < len; i++)
				if (dst1[i] != dst2[i]) {
					SET_DIFF_ASCII(dst1, dst2, len);
					exit(TEST_FAILED);
				}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_final_0(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy set a \\0 at the end of the string if strlen(src) > n");

	SANDBOX_RAISE(
			char	*src = "AAAAAAAAAAAA";
			char	dst1[] = "BBBBBBBBBBBBBBBBBBBB";
			char	dst2[] = "BBBBBBBBBBBBBBBBBBBB";
			size_t	len = strlen(dst1);

			strncpy(dst1, src, 2);
			ft_strncpy(dst2, src, 2);
			for (size_t i = 0; i < len; i++)
				if (dst1[i] != dst2[i]) {
					SET_DIFF_ASCII(dst1, dst2, len);
					exit(TEST_FAILED);
				}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_electric_memory(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy crash because it read too many bytes or attempt to read on dst !");

	SANDBOX_RAISE(
			const size_t size = 10;
			char	*src = electric_alloc(size);
			char	*dst = electric_alloc(size);

			strcpy(src, "ASSEMBLY.");

			mprotect(src - 4096 + size, 4096, PROT_READ);
			mprotect(dst - 4096 + size, 4096, PROT_WRITE);

			ft_strncpy(dst, src, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncpy_null1(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strncpy(NULL, "olol", 3);
			);
}

void			test_ft_strncpy_null2(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;
	SET_EXPLANATION("your strncpy does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF0] = {0};

			ft_strncpy(b, NULL, 2);
			);
}

void			test_ft_strncpy_speed(void *ptr) {
	typeof(strncpy)	*ft_strncpy = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 2;
			char	*src = malloc(size + 1);
			char	*dst = malloc(size + 1);

			memset(src, 'A', size);
			src[size] = 0;
			,
			strncpy(dst, src, size);
			,
			ft_strncpy(dst, src, size);
			);
}

void            test_ft_strncpy(void){
	add_fun_subtest(test_ft_strncpy_basic);
	add_fun_subtest(test_ft_strncpy_return);
	add_fun_subtest(test_ft_strncpy_unicode);
	add_fun_subtest(test_ft_strncpy_empty);
	add_fun_subtest(test_ft_strncpy_zero);
	add_fun_subtest(test_ft_strncpy_fill);
	add_fun_subtest(test_ft_strncpy_number_0);
	add_fun_subtest(test_ft_strncpy_final_0);
	add_fun_subtest(test_ft_strncpy_electric_memory);
	add_fun_subtest(test_ft_strncpy_null1);
	add_fun_subtest(test_ft_strncpy_null2);
	add_fun_subtest(test_ft_strncpy_speed);
}

////////////////////////////////
//         ft_strcat          //
////////////////////////////////

void			test_ft_strcat_basic(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not work with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;

			strcat(buff1, str);
			ft_strcat(buff2, str);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcat_return(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat return value is false / does not works");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	*str2 = "yolo !";
			char	buff1[0xF00] = STRING_2;

			char	*r1 = strcat(buff1, str);
			char	*r2 = ft_strcat(buff1, str);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			char	buff2[0xF00] = "yolo";
			r1 = strcat(buff2, str);
			r2 = ft_strcat(buff2, str);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			char	buff3[0xF00] = "";
			r1 = strcat(buff3, str2);
			r2 = ft_strcat(buff3, str2);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcat_unicode(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not work with unicode");

	SANDBOX_RAISE(
			char	*str = "こんにちは";
			char	buff1[0xF00] = "=>";
			char	buff2[0xF00] = "=>";

			char	*r1 = strcat(buff1, str);
			char	*r2 = ft_strcat(buff2, str);
			if (!strcmp(r1, r2))
				exit(TEST_SUCCESS);
			SET_DIFF(r1, r2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcat_overflow(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat overflow the dst buffer");

	SANDBOX_RAISE(
			const char	*str = "AAAA";
			char		buff1[0xF0];
			char		buff2[0xF0];

			memset(buff1, 'B', 30);
			memset(buff2, 'B', 30);
			buff1[4] = 0;
			buff2[4] = 0;

			char	*r1 = strcat(buff1, str);
			char	*r2 = ft_strcat(buff2, str);
			if (!memcmp(r1, r2, 20))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(r1, r2, 20);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcat_empty1(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not work with empty string as first parameter");

	SANDBOX_RAISE(
			char	*str = "";
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			char	*r1;
			char	*r2;

			r1 = strcat(buff1, str);
			r2 = ft_strcat(buff2, str);
			if (!strcmp(buff1, buff2) && ((buff1 - r1) == (buff2 - r2)))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			if (r1 != r2)
				SET_DIFF(r1, r2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcat_empty2(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not work with empty string as second parameter");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = "";
			char	buff2[0xF00] = "";

			strcat(buff1, str);
			ft_strcat(buff2, str);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcat_null_byte(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not set a \\0 to the end");

	SANDBOX_RAISE(
			char	*str = "n\0AAAAAAAAAAAAAAAAA";
			char	buff1[0xF00] = "\0AAAAAAAAAAAAAAAA";
			char	buff2[0xF00] = "\0AAAAAAAAAAAAAAAA";

			strcat(buff1, str);
			ft_strcat(buff2, str);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcat_electric_memory(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat crash because it read too many bytes or attempt to write on buff !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*str = electric_alloc(size);
			char	*buff = electric_alloc(size);

			strcpy(buff, "AB");
			strcpy(str, "CDEFGHI");

			mprotect(str - 4096 + size, 4096, PROT_READ);

			ft_strcat(buff, str);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcat_null1(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF] = "nyan !";

			ft_strcat(NULL, b);
			);
}

void			test_ft_strcat_null2(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF] = "nyan !";

			ft_strcat(b, NULL);
			);
}

void			test_ft_strcat_speed(void *ptr) {
	typeof(strcat)	*ft_strcat = ptr;
	SET_EXPLANATION("your strcat does not set a \\0 to the end");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*str = malloc(size + 1);
			char	*buff1 = malloc(size + 1);
			char	*buff2 = malloc(size + 1);

			*buff1 = 0;
			*buff2 = 0;
			memset(str, 'A', size);
			str[size] = 0;
			,
			strcat(buff1, str);
			,
			ft_strcat(buff2, str);
			);
}

void            test_ft_strcat(void){
	add_fun_subtest(test_ft_strcat_basic);
	add_fun_subtest(test_ft_strcat_return);
	add_fun_subtest(test_ft_strcat_unicode);
	add_fun_subtest(test_ft_strcat_overflow);
	add_fun_subtest(test_ft_strcat_empty1);
	add_fun_subtest(test_ft_strcat_empty2);
	add_fun_subtest(test_ft_strcat_null_byte);
	add_fun_subtest(test_ft_strcat_electric_memory);
	add_fun_subtest(test_ft_strcat_null1);
	add_fun_subtest(test_ft_strcat_null2);
	add_fun_subtest(test_ft_strcat_speed);
}

////////////////////////////////
//         ft_strncat         //
////////////////////////////////

void			test_ft_strncat_basic(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not work with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = 5;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_return(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat return is false / does not works");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			size_t	max = 5;

			char	*r1 = strncat(buff1, str, max);
			char	*r2 = ft_strncat(buff1, str, max);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			char	buff2[0xF0] = "AAA";
			r1 = strncat(buff2, "BBB", 1);
			r2 = ft_strncat(buff2, "BBB", 1);
			SET_DIFF(r1, r2);
			ASSERT_RETURN_VALUE(r1, r2);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncat_basic1(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not work with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = 5;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_basic2(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not work with over length size");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = 1000;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_unicode(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not work with unicode string");

	SANDBOX_RAISE(
			char	*str = "こんにちは";
			char	buff1[0xF00] = "こんにちは";
			char	buff2[0xF00] = "こんにちは";
			size_t	max = 1000;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_empty1(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strcat does not work with empty string as first parameter");

	SANDBOX_RAISE(
			char	*str = "";
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = 5;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_empty2(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strcat does not work with empty string as second parameter");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = "";
			char	buff2[0xF00] = "";
			size_t	max = 5;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_null_byte(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not set a \\0 to the end");

	SANDBOX_RAISE(
			char	*str = "n\0AAAAAAAAAAAAAAAAA";
			char	buff1[0xF00] = "\0AAAAAAAAAAAAAAAA";
			char	buff2[0xF00] = "\0AAAAAAAAAAAAAAAA";
			size_t	max = 10;

			strncat(buff1, str, max);
			ft_strncat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncat_electric_memory(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat crash because it read too many bytes or attempt to write on buff !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*str = electric_alloc(size);
			char	*buff = electric_alloc(size);

			strcpy(buff, "AB");
			strcpy(str, "CDEFGHI");

			mprotect(str - 4096 + size, 4096, PROT_READ);

			ft_strncat(buff, str, 10);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncat_null1(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF] = "nyan !";

			ft_strncat(NULL, b, 2);
			);
}

void			test_ft_strncat_null2(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF] = "nyan !";

			ft_strncat(b, NULL, 2);
			);
}

void			test_ft_strncat_speed(void *ptr) {
	typeof(strncat)	*ft_strncat = ptr;
	SET_EXPLANATION("your strncat does not set a \\0 to the end");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*str = malloc(size + 1);
			char	*buff1 = malloc(size + 1);
			char	*buff2 = malloc(size + 1);

			*buff1 = 0;
			*buff2 = 0;
			memset(str, 'A', size);
			str[size] = 0;
			,
			strncat(buff1, str, size);
			,
			ft_strncat(buff2, str, size);
			);
}

void            test_ft_strncat(void){
	add_fun_subtest(test_ft_strncat_basic);
	add_fun_subtest(test_ft_strncat_return);
	add_fun_subtest(test_ft_strncat_basic1);
	add_fun_subtest(test_ft_strncat_basic2);
	add_fun_subtest(test_ft_strncat_unicode);
	add_fun_subtest(test_ft_strncat_empty1);
	add_fun_subtest(test_ft_strncat_empty2);
	add_fun_subtest(test_ft_strncat_null_byte);
	add_fun_subtest(test_ft_strncat_electric_memory);
	add_fun_subtest(test_ft_strncat_null1);
	add_fun_subtest(test_ft_strncat_null2);
	add_fun_subtest(test_ft_strncat_speed);
}

////////////////////////////////
//         ft_strlcat         //
////////////////////////////////

void			test_ft_strlcat_basic(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not work with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = strlen(STRING_1) + 4;

			strlcat(buff1, str, max);
			ft_strlcat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_return(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not work with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = strlen(STRING_1) + 4;

			size_t	r1 = strlcat(buff1, str, max);
			size_t	r2 = ft_strlcat(buff2, str, max);
			SET_DIFF_INT((int)r1, (int)r2);
			ASSERT_RETURN_VALUE(r1, r2);

			char	s1[4] = "";
			char	s2[4] = "";
			r1 = strlcat(s1, "thx to ntoniolo for this test !", 4);
			r2 = ft_strlcat(s2, "thx to ntoniolo for this test !", 4);
			SET_DIFF_INT((int)r1, (int)r2);
			ASSERT_RETURN_VALUE(r1, r2);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_strlcat_basic1(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not work with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = strlen(STRING_1) + strlen(STRING_2);

			strlcat(buff1, str, max);
			ft_strlcat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_basic2(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not work with over length size");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = 1000;

			strlcat(buff1, str, max);
			ft_strlcat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_size(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not work with a size of 0");

	SANDBOX_RAISE(
		char	*str = STRING_1;
		char	buff1[0xF00] = STRING_2;
		char	buff2[0xF00] = STRING_2;
		size_t	max = 0;

		strlcat(buff1, str, max);
		ft_strlcat(buff2, str, max);
		if (!strcmp(buff1, buff2))
			exit(TEST_SUCCESS);
		SET_DIFF(buff1, buff2);
		exit(TEST_FAILED);
		);
}

void			test_ft_strlcat_empty1(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strcat does not work with empty string as first parameter");

	SANDBOX_RAISE(
			char	*str = "";
			char	buff1[0xF00] = STRING_2;
			char	buff2[0xF00] = STRING_2;
			size_t	max = strlen(STRING_2) + 1;

			strlcat(buff1, str, max);
			ft_strlcat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_empty2(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strcat does not work with empty string as second parameter");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00] = "";
			char	buff2[0xF00] = "";
			size_t	max = strlen(STRING_1) + 1;

			strlcat(buff1, str, max);
			ft_strlcat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_null_byte(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not set a \\0 to the end");

	SANDBOX_RAISE(
			char	*str = "n\0AA";
			char	buff1[0xF00] = "\0AAAAAAAAAAAAAAAA";
			char	buff2[0xF00] = "\0AAAAAAAAAAAAAAAA";
			size_t	max = 10;

			strlcat(buff1, str, max);
			ft_strlcat(buff2, str, max);
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_electric_memory(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat crash because it read too many bytes or attempt to write on buff !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*str = electric_alloc(size);
			char	*buff = electric_alloc(size);

			strcpy(buff, "AB");
			strcpy(str, "CDEFGHI");

			mprotect(str - 4096 + size, 4096, PROT_READ);

			ft_strlcat(buff, str, 10);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_strlcat_null1(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF] = "nyan !";

			ft_strlcat(NULL, b, 2);
			);
}


void			test_ft_strlcat_return_value(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat return value is false");

	SANDBOX_RAISE(
			char	*src = "aaa";
			char	dst1[20];
			char	dst2[20];
			int		ret1;
			int		ret2;

			memset(dst1, 'B', sizeof(dst1));
			memset(dst2, 'B', sizeof(dst2));

			strlcat(dst1, src, 20);
			strlcat(dst1, src, 20);
			ret1 = strlcat(dst1, src, 20);

			ft_strlcat(dst2, src, 20);
			ft_strlcat(dst2, src, 20);
			ret2 = ft_strlcat(dst2, src, 20);
			if (ret1 == ret2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(ret1, ret2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strlcat_null2(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			char	b[0xF] = "nyan !";

			ft_strlcat(b, NULL, 2);
			);
}

void			test_ft_strlcat_speed(void *ptr) {
	typeof(strlcat)	*ft_strlcat = ptr;
	SET_EXPLANATION("your strlcat does not set a \\0 to the end");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*str = malloc(size + 1);
			char	*buff1 = malloc(size + 1);
			char	*buff2 = malloc(size + 1);

			*buff1 = 0;
			*buff2 = 0;
			memset(str, 'A', size);
			str[size] = 0;
			,
			strlcat(buff1, str, size);
			,
			ft_strlcat(buff2, str, size);
			);
}

void            test_ft_strlcat(void){
	add_fun_subtest(test_ft_strlcat_basic);
	add_fun_subtest(test_ft_strlcat_return);
	add_fun_subtest(test_ft_strlcat_basic1);
	add_fun_subtest(test_ft_strlcat_basic2);
	add_fun_subtest(test_ft_strlcat_size);
	add_fun_subtest(test_ft_strlcat_empty1);
	add_fun_subtest(test_ft_strlcat_empty2);
	add_fun_subtest(test_ft_strlcat_null_byte);
	add_fun_subtest(test_ft_strlcat_electric_memory);
	add_fun_subtest(test_ft_strlcat_return_value);
	add_fun_subtest(test_ft_strlcat_null1);
	add_fun_subtest(test_ft_strlcat_null2);
	add_fun_subtest(test_ft_strlcat_speed);
}

////////////////////////////////
//         ft_strchr          //
////////////////////////////////

void			test_ft_strchr_basic(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with basic input");

	SANDBOX_RAISE(
			char	*src = STRING_1;

			char	*d1 = strchr(src, ' ');
			char	*d2 = ft_strchr(src, ' ');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strchr_first(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with the first char of input");

	SANDBOX_RAISE(
		char	*src = STRING_1;

		char	*d1 = strchr(src, 't');
		char	*d2 = ft_strchr(src, 't');
		if (d1 == d2)
			exit(TEST_SUCCESS);
		SET_DIFF(d1, d2);
		exit(TEST_FAILED);
		);
}

void			test_ft_strchr_before_0(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with the char before \\0");

	SANDBOX_RAISE(
		char	*src = STRING_1;

		char	*d1 = strchr(src, '!');
		char	*d2 = ft_strchr(src, '!');
		if (d1 == d2)
			exit(TEST_SUCCESS);
		SET_DIFF(d1, d2);
		exit(TEST_FAILED);
		);
}

void			test_ft_strchr_not_found(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with not found char");

	SANDBOX_RAISE(
			char	*src = STRING_1;

			char	*d1 = strchr(src, 'x');
			char	*d2 = ft_strchr(src, 'x');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strchr_unicode(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with unicode");

	SANDBOX_RAISE(
			char	*src = "īœ˙ˀ˘¯ˇ¸¯.œ«‘––™ª•¡¶¢˜ˀ";

			char	*d1 = strchr(src, L'–');
			char	*d2 = ft_strchr(src, L'–');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strchr_after_0(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr find chars after \\0");

	SANDBOX_RAISE(
			char	*src = "AA\0B";

			char	*d1 = strchr(src, 'B');
			char	*d2 = ft_strchr(src, 'B');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strchr_zero(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with \\0");

	SANDBOX_RAISE(
			char	*src = "there is so \0ma\0ny \0 \\0 in t\0his stri\0ng !\0\0\0\0";

			char	*d1 = strchr(src, '\0');
			char	*d2 = ft_strchr(src, '\0');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strchr_empty(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not work with \\0");

	SANDBOX_RAISE(
			char	*src = "\0";

			char	*d1 = strchr(src, 'a');
			char	*d2 = ft_strchr(src, 'a');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strchr_electric_memory(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr crash because it read too many bytes or attempt to write on s !");

	SANDBOX_RAISE(
			char	*src = electric_alloc(10);

			strcpy(src, "123456789");

			mprotect(src - 4096 + 10, 4096, PROT_READ);

			ft_strchr(src, 'a');
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strchr_null(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;
	SET_EXPLANATION("your strchr does not segfault with NULL parameter");

	SANDBOX_IRAISE(
			ft_strchr(NULL, '\0');
			)
}

void			test_ft_strchr_speed(void *ptr) {
	typeof(strchr)	*ft_strchr = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*src = malloc(size + 1);

			memset(src, 'A', size);
			src[size] = 0;
			,
			strchr(src, 'B');
			,
			ft_strchr(src, 'B');
			);
}

void            test_ft_strchr(void){
	add_fun_subtest(test_ft_strchr_basic);
	add_fun_subtest(test_ft_strchr_first);
	add_fun_subtest(test_ft_strchr_before_0);
	add_fun_subtest(test_ft_strchr_not_found);
	add_fun_subtest(test_ft_strchr_unicode);
	add_fun_subtest(test_ft_strchr_after_0);
	add_fun_subtest(test_ft_strchr_zero);
	add_fun_subtest(test_ft_strchr_empty);
	add_fun_subtest(test_ft_strchr_electric_memory);
	add_fun_subtest(test_ft_strchr_null);
	add_fun_subtest(test_ft_strchr_speed);
}

////////////////////////////////
//         ft_strrchr         //
////////////////////////////////

void			test_ft_strrchr_basic1(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr does not work with basic input");

	SANDBOX_RAISE(
			char	*src = STRING_1;

			char	*d1 = strrchr(src, ' ');
			char	*d2 = ft_strrchr(src, ' ');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strrchr_basic2(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr does not work with basic input");

	SANDBOX_RAISE(
			char	*src = "abbbbbbbb";

			char	*d1 = strrchr(src, 'a');
			char	*d2 = ft_strrchr(src, 'a');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strrchr_not_found(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr does not work with not found char");

	SANDBOX_RAISE(
			char	*src = STRING_1;

			char	*d1 = strrchr(src, ' ');
			char	*d2 = ft_strrchr(src, ' ');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strrchr_unicode(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr does not work with unicode");

	SANDBOX_RAISE(
			char	*src = "īœ˙ˀ˘¯ˇ¸¯.œ«‘––™ª•¡¶¢˜ˀ";

			char	*d1 = strrchr(src, L'–');
			char	*d2 = ft_strrchr(src, L'–');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strrchr_zero(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr does not work with \\0");

	SANDBOX_RAISE(
			char	*src = "there is so \0ma\0ny \0 \\0 in t\0his stri\0ng !\0\0\0\0";

			char	*d1 = strrchr(src, '\0');
			char	*d2 = ft_strrchr(src, '\0');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strrchr_empty(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr does not work with empty string");

	SANDBOX_RAISE(
			char	*src = "\0";

			char	*d1 = strrchr(src, 'a');
			char	*d2 = ft_strrchr(src, 'a');
			if (d1 == d2)
				exit(TEST_SUCCESS);
			SET_DIFF(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strrchr_electric_memory(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;
	SET_EXPLANATION("your strrchr crash because it read too many bytes or attempt to write on s !");

	SANDBOX_RAISE(
			char	*src = electric_alloc(10);

			strcpy(src, "123456789");

			mprotect(src - 4096 + 10, 4096, PROT_READ);

			ft_strrchr(src, 'a');

			src = electric_alloc_rev(10);

			strcpy(src, "123456789");

			mprotect(src - 10, 4096, PROT_READ);

			ft_strrchr(src, 'a');
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strrchr_null(void *ptr) {
	SET_EXPLANATION("your strrchr does not segfault when a null parameter is sent");
	typeof(strrchr)	*ft_strrchr = ptr;

	SANDBOX_IRAISE(
			ft_strrchr(NULL, '\0');
			)
}

void			test_ft_strrchr_speed(void *ptr) {
	typeof(strrchr)	*ft_strrchr = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			char	*src = malloc(size + 1);

			memset(src, 'A', size);
			src[size] = 0;
			,
			strrchr(src, 'B');
			,
			ft_strrchr(src, 'B');
			);
}

void            test_ft_strrchr(void) {
	add_fun_subtest(test_ft_strrchr_basic1);
	add_fun_subtest(test_ft_strrchr_basic2);
	add_fun_subtest(test_ft_strrchr_not_found);
	add_fun_subtest(test_ft_strrchr_unicode);
	add_fun_subtest(test_ft_strrchr_zero);
	add_fun_subtest(test_ft_strrchr_empty);
	add_fun_subtest(test_ft_strrchr_electric_memory);
	add_fun_subtest(test_ft_strrchr_null);
	add_fun_subtest(test_ft_strrchr_speed);
}

////////////////////////////////
//         ft_strstr          //
////////////////////////////////

void			test_ft_strstr_basic(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "FF";
			char	*s2 = "see FF your FF return FF now FF";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_basic2(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "MZIRIBMZIRIBMZP";
			char	*s2 = "MZIRIBMZP";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_basic3(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s2 = "FF";
			char	*s1 = "see F your F return FF now FF";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_basic4(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "FF";
			char	*s2 = "see F your F return F now FF";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_basic5(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "aaabbbaaabbb";
			char	*s2 = "abbaaab";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_unicode(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with unicode string");

	SANDBOX_RAISE(
			char	*s1 = "こんにちは";
			char	*s2 = "にち";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_zero_len(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with empty strings");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_not_found(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with not found patern");

	SANDBOX_RAISE(
			char	*s1 = "can't found that";
			char	*s2 = "in this !";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_zero_len1(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with empty strings");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "oh no not the empty string !";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_zero_len2(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with empty strings");

	SANDBOX_RAISE(
			char	*s1 = "oh no not the empty string !";
			char	*s2 = "";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_same_ptr(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with same pointers");

	SANDBOX_RAISE(
			char	*s1 = "AAAAAAAAAAAAA";

			char	*i1 = strstr(s1, s1);
			char	*i2 = ft_strstr(s1, s1);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_bad_boyer_moore(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "Z";
			char	*s2 = "A";

			char	*i1 = strstr(s1, s2);
			char	*i2 = ft_strstr(s1, s2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strstr_electric_memory(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t size = 20;
			char	*s1 = electric_alloc(size);
			char	*s2 = electric_alloc(size);

			strcpy(s1, "is there a nyacat ?");
			strcpy(s2, "is there a nyacat ?");

			mprotect(s1 - 4096 + size, 4096, PROT_READ);
			mprotect(s2 - 4096 + size, 4096, PROT_READ);

			ft_strstr(s1, s2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strstr_null1(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strstr(NULL, "fake");
			);
}

void			test_ft_strstr_null2(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;
	SET_EXPLANATION("your strstr does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strstr("fake", NULL);
			);
}

void			test_ft_strstr_speed(void *ptr) {
	typeof(strstr)	*ft_strstr = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*s1 = malloc(size + 1);
			char	*s2 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";

			memset(s1, 'A', size);
			s1[size] = 0;
			,
			strstr(s1, s2);
			,
			ft_strstr(s1, s2);
			);
}

void            test_ft_strstr(void){
	add_fun_subtest(test_ft_strstr_basic);
	add_fun_subtest(test_ft_strstr_basic2);
	add_fun_subtest(test_ft_strstr_basic3);
	add_fun_subtest(test_ft_strstr_basic4);
	add_fun_subtest(test_ft_strstr_basic5);
	add_fun_subtest(test_ft_strstr_unicode);
	add_fun_subtest(test_ft_strstr_zero_len);
	add_fun_subtest(test_ft_strstr_not_found);
	add_fun_subtest(test_ft_strstr_zero_len1);
	add_fun_subtest(test_ft_strstr_zero_len2);
	add_fun_subtest(test_ft_strstr_same_ptr);
	add_fun_subtest(test_ft_strstr_bad_boyer_moore);
	add_fun_subtest(test_ft_strstr_electric_memory);
	add_fun_subtest(test_ft_strstr_null2);
	add_fun_subtest(test_ft_strstr_null1);
	add_fun_subtest(test_ft_strstr_speed);
}

////////////////////////////////
//         ft_strnstr         //
////////////////////////////////

void			test_ft_strnstr_basic(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "see FF your FF return FF now FF";
			char	*s2 = "FF";
			size_t	max = strlen(s1);

			char	*i1 = strnstr(s1, s2, max);
			char	*i2 = ft_strnstr(s1, s2, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_basic2(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "FF";
			char	*s2 = "see F your F return F now F";
			size_t	max = strlen(s2);

			char	*i1 = strnstr(s1, s2, max);
			char	*i2 = ft_strnstr(s1, s2, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_basic3(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "MZIRIBMZIRIBMZE123";
			char	*s2 = "MZIRIBMZE";
			size_t	max = strlen(s2);

			char	*i1 = strnstr(s1, s2, max);
			char	*i2 = ft_strnstr(s1, s2, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_basic4(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with basic input");

	SANDBOX_RAISE(
			char *big = "abcdef";
			char *little = "abcdefghijklmnop";
			size_t	max = strlen(big);

			char 	*s1 = strnstr(big, little, max);
			char 	*s2 = ft_strnstr(big, little, max);

			if (s1 == s2)
				exit(TEST_SUCCESS);
			SET_DIFF(s1, s2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_basic5(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with basic input");

	SANDBOX_RAISE(
			char *	big = "123456789";
			char *	little = "9";
			size_t	max = 8;

			char 	*s1 = strnstr(big, little, max);
			char 	*s2 = ft_strnstr(big, little, max);

			if (s1 == s2)
				exit(TEST_SUCCESS);
			SET_DIFF(s1, s2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_found(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with not found patern");

	SANDBOX_RAISE(
			char	*s1 = "FF";
			char	*s2 = "see FF your FF return FF now FF";

			char	*i1 = strnstr(s1, s2, 4);
			char	*i2 = ft_strnstr(s1, s2, 4);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_zero_len1(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with empty strings");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "oh no not the empty string !";
			size_t	max = strlen(s2);

			char	*i1 = strnstr(s1, s2, max);
			char	*i2 = ft_strnstr(s1, s2, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_zero_len2(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with empty strings");

	SANDBOX_RAISE(
			char	*s1 = "oh no not the empty string !";
			char	*s2 = "";
			size_t	max = strlen(s1);

			char	*i1 = strnstr(s1, s2, max);
			char	*i2 = ft_strnstr(s1, s2, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_zero_len3(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not work with empty strings and 0 length");

	SANDBOX_RAISE(
			char	*s1 = "oh no not the empty string !";
			char	*s2 = "";
			size_t	max = 0;

			char	*i1 = strnstr(s1, s2, max);
			char	*i2 = ft_strnstr(s1, s2, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_same_ptr(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;

	SANDBOX_RAISE(
			char	*s1 = "AAAAAAAAAAAAA";
			size_t	max = strlen(s1);

			char	*i1 = strnstr(s1, s1, max);
			char	*i2 = ft_strnstr(s1, s1, max);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_zero(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;

	SANDBOX_RAISE(
			char	*s1 = "A";

			char	*i1 = strnstr(s1, s1, 2);
			char	*i2 = ft_strnstr(s1, s1, 2);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnstr_electric_memory(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t size = 20;
			char	*s1 = electric_alloc(size);
			char	*s2 = electric_alloc(size);

			strcpy(s1, "is there a nyacat ?");
			strcpy(s2, "is there a nyacat ?");

			mprotect(s1 - 4096 + size, 4096, PROT_READ);
			mprotect(s2 - 4096 + size, 4096, PROT_READ);

			ft_strnstr(s1, s2, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strnstr_null1(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strnstr(NULL, "fake", 3);
			);
}

void			test_ft_strnstr_null2(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;
	SET_EXPLANATION("your strnstr does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strnstr("fake", NULL, 3);
			);
}

void			test_ft_strnstr_speed(void *ptr) {
	typeof(strnstr)	*ft_strnstr = ptr;

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*s2 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";
			char	*s1 = malloc(size + 1);

			memset(s1, 'A', size);
			s1[size] = 0;
			,
			strnstr(s1, s2, size);
			,
			ft_strnstr(s1, s2, size);
			);
}

void            test_ft_strnstr(void){
	add_fun_subtest(test_ft_strnstr_basic);
	add_fun_subtest(test_ft_strnstr_basic2);
	add_fun_subtest(test_ft_strnstr_basic3);
	add_fun_subtest(test_ft_strnstr_basic4);
	add_fun_subtest(test_ft_strnstr_basic5);
	add_fun_subtest(test_ft_strnstr_found);
	add_fun_subtest(test_ft_strnstr_zero_len1);
	add_fun_subtest(test_ft_strnstr_zero_len2);
	add_fun_subtest(test_ft_strnstr_zero_len3);
	add_fun_subtest(test_ft_strnstr_same_ptr);
	add_fun_subtest(test_ft_strnstr_zero);
	add_fun_subtest(test_ft_strnstr_electric_memory);
	add_fun_subtest(test_ft_strnstr_null2);
	add_fun_subtest(test_ft_strnstr_null1);
	add_fun_subtest(test_ft_strnstr_speed);
}

////////////////////////////////
//         ft_strcmp          //
////////////////////////////////


void			test_ft_strcmp_basic1(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = STRING_1;
			char	*s2 = STRING_2;

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_basic2(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "omg1";
			char	*s2 = "omg3";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_basic3(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_basic4(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "AAAAAAAAAB";
			char	*s2 = "AAAAAAAAAC";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_after_0(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp check bytes after \\0");

	SANDBOX_RAISE(
			char	*s1 = "yolo!\0B";
			char	*s2 = "yolo!\0A";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_zero1(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with zero length string");

	SANDBOX_RAISE(
			char	*s1 = "AAAAAA";
			char	*s2 = "";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_zero2(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with zero length string");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "AAAAAA";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_unsigned(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not cast in unsigned the diff");

	SANDBOX_RAISE(
			char	*s1 = "\0";
			char	*s2 = "\200";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_ascii(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with non ascii chars");

	SANDBOX_RAISE(
			char	*s1 = "\x12\xff\x65\x12\xbd\xde\xad";
			char	*s2 = "\x12\x02";

			int		i1 = REG(strcmp(s1, s2));
			int		i2 = REG(ft_strcmp(s1, s2));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strcmp_electric_memory(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*s1 = electric_alloc(size);
			char	*s2 = electric_alloc(size);

			strcpy(s1, "{'-_-'} !");
			strcpy(s2, "{'-_-'} !");

			mprotect(s1 - 4096 + size, 4096, PROT_READ);
			mprotect(s2 - 4096 + size, 4096, PROT_READ);

			ft_strcmp(s1, s2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strcmp_null1(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strcmp(NULL, "nope");
			);
}

void			test_ft_strcmp_null2(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strcmp("nope", NULL);
			ft_strcmp(NULL, NULL);
			);
}

void			test_ft_strcmp_speed(void *ptr) {
	typeof(strcmp)	*ft_strcmp = ptr;
	SET_EXPLANATION("your strcmp does not work with zero length string");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*s1 = malloc(size + 1);
			char	*s2 = malloc(size + 1);

			memset(s1, 'A', size);
			memset(s2, 'A', size);
			s1[size] = s2[size] = 0;

			,
			strcmp(s1, s2);
			,
			ft_strcmp(s1, s2);
			);
}

void            test_ft_strcmp(void){
	add_fun_subtest(test_ft_strcmp_basic1);
	add_fun_subtest(test_ft_strcmp_basic2);
	add_fun_subtest(test_ft_strcmp_basic3);
	add_fun_subtest(test_ft_strcmp_basic4);
	add_fun_subtest(test_ft_strcmp_after_0);
	add_fun_subtest(test_ft_strcmp_zero1);
	add_fun_subtest(test_ft_strcmp_zero2);
	add_fun_subtest(test_ft_strcmp_unsigned);
	add_fun_subtest(test_ft_strcmp_ascii);
	add_fun_subtest(test_ft_strcmp_electric_memory);
	add_fun_subtest(test_ft_strcmp_null1);
	add_fun_subtest(test_ft_strcmp_null2);
	add_fun_subtest(test_ft_strcmp_speed);
}

////////////////////////////////
//         ft_strncmp         //
////////////////////////////////

void			test_ft_strncmp_basic1(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = STRING_1;
			char	*s2 = STRING_2;
			size_t	size = strlen(STRING_1);

			int		i1 = REG(strncmp(s1, s2, size));
			int		i2 = REG(ft_strncmp(s1, s2, size));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_basic2(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "omg1||||||||||||||||";
			char	*s2 = "omg3";
			size_t	size = 4;

			int		i1 = REG(strncmp(s1, s2, size));
			int		i2 = REG(ft_strncmp(s1, s2, size));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_basic3(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "";

			int		i1 = REG(strncmp(s1, s2, 1));
			int		i2 = REG(ft_strncmp(s1, s2, 1));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_basic4(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "AAAAAAAAA1";
			char	*s2 = "AAAAAAAAA2";

			int		i1 = REG(strncmp(s1, s2, 3));
			int		i2 = REG(ft_strncmp(s1, s2, 3));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_zero1(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with zero length string");

	SANDBOX_RAISE(
			char	*s1 = "AAAAAA";
			char	*s2 = "";

			int		i1 = REG(strncmp(s1, s2, 6));
			int		i2 = REG(ft_strncmp(s1, s2, 6));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_zero2(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with zero length string");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "AAAAAA";

			int		i1 = REG(strncmp(s1, s2, 6));
			int		i2 = REG(ft_strncmp(s1, s2, 6));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_cast(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not cast in unsigned char the diff");

	SANDBOX_RAISE(
			char	*s1 = "\200";
			char	*s2 = "\0";

			int		i1 = REG(strncmp(s1, s2, 1));
			int		i2 = REG(ft_strncmp(s1, s2, 1));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_over_len(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "omg1";
			char	*s2 = "omg3                ";
			size_t	size = 100000;

			int		i1 = REG(strncmp(s1, s2, size));
			int		i2 = REG(ft_strncmp(s1, s2, size));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_hidden(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp doesn't stop at \\0");

	SANDBOX_RAISE(
		char	*s1 = "atoms\0\0\0\0";
		char	*s2 = "atoms\0abc";
		size_t  size = 8;

		int		i1 = REG(strncmp(s1, s2, size));
		int		i2 = REG(ft_strncmp(s1, s2, size));
		if (i1 == i2)
			exit(TEST_SUCCESS);
		SET_DIFF_INT(i1, i2);
		exit(TEST_FAILED);
		);
}

void			test_ft_strncmp_ascii(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with non ascii chars");

	SANDBOX_RAISE(
			char	*s1 = "\x12\xff\x65\x12\xbd\xde\xad";
			char	*s2 = "\x12\x02";
			size_t	size = 6;

			int		i1 = REG(strncmp(s1, s2, size));
			int		i2 = REG(ft_strncmp(s1, s2, size));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_biglittle(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work when s1 is contained in s2");

	SANDBOX_RAISE(
			char	*big = "abcdef";
			char	*little = "abcdefghijklmnop";
			size_t	size = 6;

			int		i1 = REG(strncmp(big, little, size));
			int		i2 = REG(ft_strncmp(big, little, size));
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strncmp_electric_memory(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*s1 = electric_alloc(size);
			char	*s2 = electric_alloc(size);

			strcpy(s1, "{'-_-'} !");
			strcpy(s2, "{'-_-'} !");

			mprotect(s1 - 4096 + size, 4096, PROT_READ);
			mprotect(s2 - 4096 + size, 4096, PROT_READ);

			ft_strncmp(s1, s2, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strncmp_null1(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strncmp(NULL, "nope", 3);
			);
}

void			test_ft_strncmp_null2(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not segfault when null parameter is sent");

	SANDBOX_IRAISE(
			ft_strncmp("nope", NULL, 3);
			ft_strncmp(NULL, NULL, 3);
			);
}

void			test_ft_strncmp_speed(void *ptr) {
	typeof(strncmp)	*ft_strncmp = ptr;
	SET_EXPLANATION("your strncmp does not work with zero length string");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 4;
			char	*s1 = malloc(size + 1);
			char	*s2 = malloc(size + 1);

			memset(s1, 'A', size);
			memset(s2, 'A', size);
			s1[size] = s2[size] = 0;

			,
			strncmp(s1, s2, size);
			,
			ft_strncmp(s1, s2, size);
			);
}

void            test_ft_strncmp(void){
	add_fun_subtest(test_ft_strncmp_basic1);
	add_fun_subtest(test_ft_strncmp_basic2);
	add_fun_subtest(test_ft_strncmp_basic3);
	add_fun_subtest(test_ft_strncmp_basic4);
	add_fun_subtest(test_ft_strncmp_zero1);
	add_fun_subtest(test_ft_strncmp_zero2);
	add_fun_subtest(test_ft_strncmp_cast);
	add_fun_subtest(test_ft_strncmp_hidden);
	add_fun_subtest(test_ft_strncmp_over_len);
	add_fun_subtest(test_ft_strncmp_ascii);
	add_fun_subtest(test_ft_strncmp_biglittle);
	add_fun_subtest(test_ft_strncmp_electric_memory);
	add_fun_subtest(test_ft_strncmp_null1);
	add_fun_subtest(test_ft_strncmp_null2);
	add_fun_subtest(test_ft_strncmp_speed);
}

////////////////////////////////
//         ft_atoi            //
////////////////////////////////

void			test_ft_atoi_basic(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with positive numbers");

	SANDBOX_RAISE(
			char	*n = "945";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_negative(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with negative numbers");

	SANDBOX_RAISE(
			char	*n = "-085";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_rand(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with random numbers");

	SANDBOX_RAISE(
			srand(clock());
			for (int i = 0; i < 100; i++) {
				char n[15];
				sprintf(n, "%i", rand());

				int		i1 = atoi(n);
				int		i2 = ft_atoi(n);
				if (i1 != i2) {
					SET_DIFF_INT(i1, i2);
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_atoi_blank1(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi is not working with blanks");

	SANDBOX_RAISE(
			char	*n = "\t\v\f\r\n \f-06050";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_blank2(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi is not working with blanks");

	SANDBOX_RAISE(
			char	*n = "\t\v\f\r\n \f- \f\t\n\r    06050";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_invisible(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi is not working with non space invisible char");

	SANDBOX_RAISE(
		char	*n = "\e06050";

		int		i1 = atoi(n);
		int		i2 = ft_atoi(n);
		if (i1 == i2)
			exit(TEST_SUCCESS);
		SET_DIFF_INT(i1, i2);
		exit(TEST_FAILED);
		);
}

void			test_ft_atoi_string(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi is not working with blanks");

	SANDBOX_RAISE(
			char	*n = "-123THERE IS A NYANCAT UNDER YOUR BED";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_max_int(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with int max value");

	SANDBOX_RAISE(
			char	n[15];
			sprintf(n, "%i", INT_MAX);

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_min_int(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with int min value");

	SANDBOX_RAISE(
			char	n[15];
			sprintf(n, "%i", INT_MIN);

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atoi_max_long(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with long max value");

	SANDBOX_RAISE(
			char	n[40];
			sprintf(n, "%li", LONG_MAX);

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}

void			test_ft_atoi_min_long(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with long min value");

	SANDBOX_RAISE(
			char	n[40];
			sprintf(n, "%li", LONG_MIN);

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}

void			test_ft_atoi_over_max_long(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with over long max value");

	SANDBOX_RAISE(
			char	n[40] = "99999999999999999999999999";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}

void			test_ft_atoi_over_min_long(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with over long min value");

	SANDBOX_RAISE(
			char	n[40] = "-99999999999999999999999999";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}

void			test_ft_atoi_plus0(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with the plus sign");

	SANDBOX_RAISE(
			char	n[40] = "+1234";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}

void			test_ft_atoi_plus1(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with the plus sign and spaces");

	SANDBOX_RAISE(
			char	n[40] = "\t\v\f\r\n \f+\t\v\f\r\n \f1234";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}

void			test_ft_atoi_plus2(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with the plus sign and spaces");

	SANDBOX_RAISE(
			char	n[40] = "+\t\v\f\r\n \f1234";

			int		i1 = atoi(n);
			int		i2 = ft_atoi(n);
			if (i1 == i2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i1, i2);
			exit(TEST_KO);
			);
}


void			test_ft_atoi_null(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;

	SANDBOX_IRAISE(
			ft_atoi(NULL);
			);
}

void			test_ft_atoi_speed(void *ptr) {
	typeof(atoi)	*ft_atoi = ptr;
	SET_EXPLANATION("your atoi does not work with random numbers");

	SANDBOX_SPEED(
			char	*nbrs = malloc(BFSIZE * 4);

			srand(clock());
			for (int i = 0; i < 10000; i++)
				sprintf(nbrs + (i * 14), "%-11i", rand());
			,
			for (int i = 0; i < 10000; i++)
				atoi(nbrs + (i * 14));
			,
			for (int i = 0; i < 10000; i++)
				ft_atoi(nbrs + (i * 14));
			);
}

void            test_ft_atoi(void){
	add_fun_subtest(test_ft_atoi_basic);
	add_fun_subtest(test_ft_atoi_negative);
	add_fun_subtest(test_ft_atoi_rand);
	add_fun_subtest(test_ft_atoi_blank1);
	add_fun_subtest(test_ft_atoi_blank2);
	add_fun_subtest(test_ft_atoi_invisible);
	add_fun_subtest(test_ft_atoi_string);
	add_fun_subtest(test_ft_atoi_max_int);
	add_fun_subtest(test_ft_atoi_min_int);
	add_fun_subtest(test_ft_atoi_max_long);
	add_fun_subtest(test_ft_atoi_min_long);
	add_fun_subtest(test_ft_atoi_over_max_long);
	add_fun_subtest(test_ft_atoi_over_min_long);
	add_fun_subtest(test_ft_atoi_plus0);
	add_fun_subtest(test_ft_atoi_plus1);
	add_fun_subtest(test_ft_atoi_plus2);
	add_fun_subtest(test_ft_atoi_null);
	add_fun_subtest(test_ft_atoi_speed);
}

////////////////////////////////
//         ft_isalpha         //
////////////////////////////////

#include <ctype.h>

void			test_ft_isalpha_(void *ptr) {
	typeof(isalpha)	*ft_isalpha = ptr;
	SET_EXPLANATION("your isalpha just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
		int		i;
			i = -1;
			while (i < 530)
			{
				if (!!ft_isalpha(i) != !!isalpha(i))
					exit(TEST_FAILED);
				i++;
			}
			exit(TEST_SUCCESS);
		);
}


void            test_ft_isalpha(void){
	add_fun_subtest(test_ft_isalpha_);
}

////////////////////////////////
//         ft_isdigit         //
////////////////////////////////

void			test_ft_isdigit_(void *ptr) {
	typeof(isdigit)	*ft_isdigit = ptr;
	SET_EXPLANATION("your isdigit just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 530)
			{
				if (!!ft_isdigit(i) != !!isdigit(i))
					exit(TEST_FAILED);
				i++;
			}
			exit(TEST_SUCCESS);
		);
}

void            test_ft_isdigit(void){
	add_fun_subtest(test_ft_isdigit_);
}

////////////////////////////////
//         ft_isalnum         //
////////////////////////////////

void			test_ft_isalnum_(void *ptr) {
	typeof(isalnum)	*ft_isalnum = ptr;
	SET_EXPLANATION("your isalnum just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 530)
			{
				if (!!ft_isalnum(i) != !!isalnum(i))
					exit(TEST_FAILED);
				i++;
			}
			exit(TEST_SUCCESS);
		);
}

void            test_ft_isalnum(void){
	add_fun_subtest(test_ft_isalnum_);
}

////////////////////////////////
//         ft_isascii         //
////////////////////////////////

void			test_ft_isascii_(void *ptr) {
	typeof(isascii)	*ft_isascii = ptr;
	SET_EXPLANATION("your isascii just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 530)
			{
				if (!!ft_isascii(i) != !!isascii(i)) {
					SET_DIFF_CUSTOM("%12s: |%i|\n%12s: |%i|\nparam: (%i)", current_fun_name + 3, isascii(i), current_fun_name, ft_isascii(i), i)
					exit(TEST_FAILED);
				}
				i++;
			}
			exit(TEST_SUCCESS);
		);
}

void            test_ft_isascii(void){
	add_fun_subtest(test_ft_isascii_);
}

////////////////////////////////
//         ft_isprint         //
////////////////////////////////

void			test_ft_isprint_(void *ptr) {
	typeof(isprint)	*ft_isprint = ptr;
	SET_EXPLANATION("your isprint just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 530)
			{
				if (!!ft_isprint(i) != !!isprint(i)) {
					SET_DIFF_CUSTOM("%12s: |%i|\n%12s: |%i|\nparam: (%i)", current_fun_name + 3, isprint(i), current_fun_name, ft_isprint(i), i)
					exit(TEST_FAILED);
				}
				i++;
			}
			exit(TEST_SUCCESS);
		);
}


void            test_ft_isprint(void){
	add_fun_subtest(test_ft_isprint_);
}

////////////////////////////////
//        ft_touupper         //
////////////////////////////////

void			test_ft_toupper_(void *ptr) {
	typeof(toupper)	*ft_toupper = ptr;
	SET_EXPLANATION("your toupper just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -100;
			while (i < 530)
			{
				if (ft_toupper(i) != toupper(i)) {
					SET_DIFF_CUSTOM("%12s: |%i|\n%12s: |%i|\nparam: (%i)", current_fun_name + 3, toupper(i), current_fun_name, ft_toupper(i), i)
					exit(TEST_FAILED);
				}
				i++;
			}
			exit(TEST_SUCCESS);
		);
}

void            test_ft_toupper(void) {
	add_fun_subtest(test_ft_toupper_);
}

////////////////////////////////
//         ft_tolower         //
////////////////////////////////

void			test_ft_tolower_(void *ptr) {
	typeof(tolower)	*ft_tolower = ptr;
	SET_EXPLANATION("your tolower just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -100;
			while (i < 530)
			{
				if (ft_tolower(i) != tolower(i)) {
					SET_DIFF_CUSTOM("%12s: |%i|\n%12s: |%i|\nparam: (%i)", current_fun_name + 3, tolower(i), current_fun_name, ft_tolower(i), i)
					exit(TEST_FAILED);
				}
				i++;
			}
			exit(TEST_SUCCESS);
		);
}

void            test_ft_tolower(void){
	add_fun_subtest(test_ft_tolower_);
}



///////////////////////////////
///////////////////////////////
//          PART II          //
///////////////////////////////
///////////////////////////////



////////////////////////////////
//        ft_memalloc         //
////////////////////////////////

void			test_ft_memalloc_free(void *ptr) {
	void *	(*ft_memalloc)(size_t) = ptr;
	SET_EXPLANATION("your memalloc don't allocate memory");

	SANDBOX_RAISE(
			free(ft_memalloc(42));
			);
}

void			test_ft_memalloc_zero(void *ptr) {
	void *	(*ft_memalloc)(size_t) = ptr;
	SET_EXPLANATION("your memalloc does not set allocated mem to 0");

	SANDBOX_RAISE(
			size_t	size = 514;
			char	*cmp = malloc(size);
			int		diff;

			MALLOC_MEMSET;
			char	*ret = ft_memalloc(size);
			MALLOC_RESET;

			bzero(cmp, size);
			if ((diff = memcmp(cmp, ret, size))) {
				SET_DIFF_INT(0, diff);
				exit(TEST_FAILED);
			}
			free(ret);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memalloc_too_big(void *ptr) {
	void *	(*ft_memalloc)(size_t) = ptr;
	SET_EXPLANATION("your memalloc does not return null for too big value");

	SANDBOX_RAISE(
			char	*ret = ft_memalloc(ULONG_MAX);

			if (!ret)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_memalloc_size(void *ptr) {
	void *	(*ft_memalloc)(size_t) = ptr;
	SET_EXPLANATION("your memalloc does not set allocated mem to 0");

	SANDBOX_RAISE(
			size_t	size = 514;
			int		ret_size;

			MALLOC_SIZE;
			char	*ret = ft_memalloc(size);
			MALLOC_RESET;
			ret_size = get_last_malloc_size();

			free(ret);
			if (ret_size == (int)size)
				exit(TEST_SUCCESS);
			SET_DIFF_INT((int)size, ret_size)
			exit(TEST_SUCCESS);
			);
}

void			test_ft_memalloc_malloc_null(void *ptr) {
	void *	(*ft_memalloc)(size_t) = ptr;
	SET_EXPLANATION("you did not protect your malloc return");

	SANDBOX_RAISE(
			void	*ret;

			MALLOC_NULL;
			ret = ft_memalloc(42);
			MALLOC_RESET;
			if (!ret)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_memalloc_malloc_size(void *ptr) {
	void *	(*ft_memalloc)(size_t) = ptr;
	SET_EXPLANATION("your memalloc did not allocate the good size");

	SANDBOX_RAISE(
			void	*ret;
			int		alloc_size = 42;
			int		size;

			MALLOC_SIZE;
			ret = ft_memalloc(alloc_size);
			MALLOC_RESET;

			size = get_last_malloc_size();

			if (size == alloc_size)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(alloc_size, size);
			exit(TEST_FAILED);
			);
}

void            test_ft_memalloc(void) {
	add_fun_subtest(test_ft_memalloc_free);
	add_fun_subtest(test_ft_memalloc_zero);
	add_fun_subtest(test_ft_memalloc_too_big);
	add_fun_subtest(test_ft_memalloc_malloc_null);
	add_fun_subtest(test_ft_memalloc_size);
}

////////////////////////////////
//         ft_memdel          //
////////////////////////////////

void			test_ft_memdel_test(void *ptr) {
	void	(*ft_memdel)(void **) = ptr;

	SANDBOX_RAISE(
			void	*m = malloc(42);

			ft_memdel(&m);
			if (m == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, m);
			exit(TEST_FAILED);
			);
}

void			test_ft_memdel_free(void *ptr) {
	void	(*ft_memdel)(void **) = ptr;
	SET_EXPLANATION("your memdel does not free memory !");

	SANDBOX_RAISE(
			STDOUT_TO_BUFF;
			void	*m = malloc(42);
			ft_memdel(&m);
			if (m != NULL)
				free(m);
			write(1, "", 1);
			VOID_STDOUT;
			);
}

void			test_ft_memdel_null(void *ptr) {
	void	(*ft_memdel)(void **) = ptr;
	SET_EXPLANATION("your memdel does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_memdel(NULL);
			);
}

void            test_ft_memdel(void) {
	add_fun_subtest(test_ft_memdel_test);
	add_fun_subtest(test_ft_memdel_free);
	add_fun_subtest(test_ft_memdel_null);
}

////////////////////////////////
//         ft_strnew          //
////////////////////////////////

void			test_ft_strnew_free(void *ptr) {
	void *	(*ft_strnew)(size_t) = ptr;
	SET_EXPLANATION("your strnew don't allocate memory");

	SANDBOX_RAISE(
			free(ft_strnew(42));
			);
}

void			test_ft_strnew_zero(void *ptr) {
	void *	(*ft_strnew)(size_t) = ptr;
	SET_EXPLANATION("your strnew does not set allocated mem to 0");

	SANDBOX_RAISE(
			size_t	size = 514;
			MALLOC_MEMSET;
			char	*ret = ft_strnew(size);
			MALLOC_RESET;

			for (size_t i = 0;i < size + 1;i++)
				if (ret[i] != 0) {
					SET_DIFF_INT(0, ret[i]);
					exit(TEST_FAILED);
				}
			free(ret);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strnew_size(void *ptr) {
	void *	(*ft_strnew)(size_t) = ptr;
	SET_EXPLANATION("your strnew did not allocate the good size so the \\0 test can be false");

	SANDBOX_RAISE(
			size_t	size = 514;
			int		ret_size;

			MALLOC_SIZE;
			char	*ret = ft_strnew(size);
			MALLOC_RESET;

			ret_size = get_last_malloc_size();
			if (ret_size == (int)size + 1)
				exit(TEST_SUCCESS);
			free(ret);
			SET_DIFF_INT((int)size + 1, ret_size);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnew_malloc_null(void *ptr) {
	void *	(*ft_strnew)(size_t) = ptr;
	SET_EXPLANATION("you did not protect your malloc return");

	SANDBOX_RAISE(
			void	*ret;

			MALLOC_NULL;
			ret = ft_strnew(42);
			MALLOC_RESET;
			if (!ret)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void            test_ft_strnew(void){
	add_fun_subtest(test_ft_strnew_free);
	add_fun_subtest(test_ft_strnew_zero);
	add_fun_subtest(test_ft_strnew_size);
	add_fun_subtest(test_ft_strnew_malloc_null);
}

////////////////////////////////
//         ft_strdel          //
////////////////////////////////

void			test_ft_strdel_test(void *ptr) {
	void	(*ft_strdel)(void **) = ptr;

	SANDBOX_RAISE(
			void	*m = malloc(42);

			ft_strdel(&m);
			if (m == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, m);
			exit(TEST_FAILED);
			);
}

void			test_ft_strdel_free(void *ptr) {
	void	(*ft_strdel)(void **) = ptr;
	SET_EXPLANATION("your strdel does not free memory !");

	SANDBOX_RAISE(
			STDOUT_TO_BUFF;
			void	*m = malloc(42);
			ft_strdel(&m);
			if (m != NULL)
				free(m);
			write(1, "", 1);
			VOID_STDOUT;
			);
}

void			test_ft_strdel_null(void *ptr) {
	void	(*ft_strdel)(void **) = ptr;
	SET_EXPLANATION("your strdel does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strdel(NULL);
			);
}

void            test_ft_strdel(void){
	add_fun_subtest(test_ft_strdel_test);
	add_fun_subtest(test_ft_strdel_free);
	add_fun_subtest(test_ft_strdel_null);
}

////////////////////////////////
//         ft_strclr          //
////////////////////////////////

void			test_ft_strclr_basic(void *ptr) {
	void	(*ft_strclr)(char *) = ptr;
	SET_EXPLANATION("your strclr does not set to 0 the string");

	SANDBOX_RAISE(
			char	b[] = "strclr test !\r\n";
			char	n[0xF0];
			size_t	size = strlen(b);

			memset(n, 0, size);
			ft_strclr(b);
			if (!memcmp(b, n, size))
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, memcmp(b, n, size));
			exit(TEST_FAILED);
			);

}

void			test_ft_strclr_null(void *ptr) {
	void	(*ft_strclr)(char *) = ptr;
	SET_EXPLANATION("your strclr does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strclr(NULL);
			);
}

void            test_ft_strclr(void){
	add_fun_subtest(test_ft_strclr_basic);
	add_fun_subtest(test_ft_strclr_null);
}

////////////////////////////////
//         ft_striter         //
////////////////////////////////

void			f_striter(char *s) { *s = 'F'; }

void			test_ft_striter_basic(void *ptr) {
	void	(*ft_striter)(char *, void (*)(char *)) = ptr;
	SET_EXPLANATION("your striter does no work");

	SANDBOX_RAISE(
			char	b[] = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				f_striter(b2 + i);
			b2[size] = 0;
			ft_striter(b, f_striter);
			if (!strcmp(b, b2))
				exit(TEST_SUCCESS);
			SET_DIFF(b, b2);
			exit(TEST_FAILED);
			);
}

void			test_ft_striter_electric_memory(void *ptr) {
	void	(*ft_striter)(char *, void (*)(char *)) = ptr;
	SET_EXPLANATION("your striter crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*b = electric_alloc(size);

			strcpy(b, "123456789");

			ft_striter(b, f_striter);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_striter_null1(void *ptr) {
	void	(*ft_striter)(char *, void (*)(char *)) = ptr;
	SET_EXPLANATION("your striter does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_striter(NULL, f_striter)
			);
}

void			test_ft_striter_null2(void *ptr) {
	void	(*ft_striter)(char *, void (*)(char *)) = ptr;
	SET_EXPLANATION("your striter does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_striter("you should not ...", NULL);
			);
}

void            test_ft_striter(void){
	add_fun_subtest(test_ft_striter_basic);
	add_fun_subtest(test_ft_striter_electric_memory);
	add_fun_subtest(test_ft_striter_null1);
	add_fun_subtest(test_ft_striter_null2);
}

////////////////////////////////
//        ft_striteri         //
////////////////////////////////

void			f_striteri(unsigned i, char *s) { *s = i + '0'; }

void			test_ft_striteri_basic(void *ptr) {
	void	(*ft_striteri)(char *, void (*)(unsigned, char *)) = ptr;
	SET_EXPLANATION("your striteri does no work");

	SANDBOX_RAISE(
			char	b[] = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				f_striteri(i, b2 + i);
			b2[size] = 0;
			ft_striteri(b, f_striteri);
			if (!strcmp(b, b2))
				exit(TEST_SUCCESS);
			SET_DIFF(b, b2);
			exit(TEST_FAILED);
			);
}

void			test_ft_striteri_electric_memory(void *ptr) {
	void	(*ft_striteri)(char *, void (*)(unsigned, char *)) = ptr;
	SET_EXPLANATION("your striteri crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*b = electric_alloc(size);

			strcpy(b, "123456789");

			ft_striteri(b, f_striteri);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_striteri_null1(void *ptr) {
	void	(*ft_striteri)(char *, void (*)(unsigned, char *)) = ptr;
	SET_EXPLANATION("your striteri does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_striteri(NULL, f_striteri)
			);
}

void			test_ft_striteri_null2(void *ptr) {
	void	(*ft_striteri)(char *, void (*)(unsigned, char *)) = ptr;
	SET_EXPLANATION("your striteri does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_striteri("you should not ...", NULL);
			ft_striteri(NULL, NULL);
			);
}

void            test_ft_striteri(void){
	add_fun_subtest(test_ft_striteri_basic);
	add_fun_subtest(test_ft_striteri_electric_memory);
	add_fun_subtest(test_ft_striteri_null1);
	add_fun_subtest(test_ft_striteri_null2);
}

////////////////////////////////
//         ft_strmap          //
////////////////////////////////

char			f_strmap(char c) { return (c + 7); }

void			test_ft_strmap_basic(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("your strmap does not work");

	SANDBOX_RAISE(
			char	*b = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				b2[i] = f_strmap(b[i]);
			b2[size] = 0;
			char	*ret = ft_strmap(b, f_strmap);
			if (!strcmp(b2, ret))
				exit(TEST_SUCCESS);
			SET_DIFF(ret, b2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmap_zero(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("your strmap did not set \\0 at the end of the string");

	SANDBOX_RAISE(
			char	*b = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				b2[i] = f_strmap(b[i]);
			b2[size] = 0;
			MALLOC_MEMSET;
			char	*ret = ft_strmap(b, f_strmap);
			MALLOC_RESET;
			if (!memcmp(b2, ret, size + 1))
				exit(TEST_SUCCESS);
			SET_DIFF(b2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmap_free(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("your strmap does not malloc ?");

	SANDBOX_RAISE(
			char	*b = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				b2[i] = f_strmap(b[i]);
			b2[size] = 0;
			STDOUT_TO_BUFF;
			char *newstr = ft_strmap(b, f_strmap);
			if (!strcmp(newstr, b2))
				exit(TEST_SUCCESS);
			free(newstr);
			write(1, "", 1);
			VOID_STDOUT;
			SET_DIFF(b2, newstr);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmap_size(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("your strmap did not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char	*b = "override this !";
			size_t	size = strlen(b);
			int		ret_size;

			MALLOC_SIZE;
			ft_strmap(b, f_strmap);
			MALLOC_RESET;
			ret_size = get_last_malloc_size();

			if (ret_size == (int)size + 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT((int)size + 1, ret_size);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmap_malloc_null(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("you did not protect your malloc return");

	SANDBOX_RAISE(
			char	*b = "override this !";

			MALLOC_NULL;
			char *newstr = ft_strmap(b, f_strmap);
			MALLOC_RESET;
			if (newstr == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF(NULL, newstr);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmap_null1(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("your strmap does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strmap(NULL, f_strmap);
			);
}

void			test_ft_strmap_null2(void *ptr) {
	char *	(*ft_strmap)(const char *, char (*)(char)) = ptr;
	SET_EXPLANATION("your strmap does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strmap("olol", NULL);
			ft_strmap(NULL, NULL);
			);
}

void            test_ft_strmap(void){
	add_fun_subtest(test_ft_strmap_basic);
	add_fun_subtest(test_ft_strmap_zero);
	add_fun_subtest(test_ft_strmap_free);
	add_fun_subtest(test_ft_strmap_size);
	add_fun_subtest(test_ft_strmap_malloc_null);
	add_fun_subtest(test_ft_strmap_null1);
	add_fun_subtest(test_ft_strmap_null2);
}

////////////////////////////////
//         ft_strmapi         //
////////////////////////////////

char			f_strmapi(unsigned i, char c) { return (c + i); }

void			test_ft_strmapi_basic(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi does not work");

	SANDBOX_RAISE(
			char	*b = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				b2[i] = f_strmapi(i, b[i]);
			b2[size] = 0;
			char	*ret = ft_strmapi(b, f_strmapi);
			if (!strcmp(b2, ret))
				exit(TEST_SUCCESS);
			SET_DIFF(ret, b2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmapi_malloc_size(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi does not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char	*b = "override this !";
			size_t	size = strlen(b);
			size_t	alloc_size;

			MALLOC_SIZE;
			ft_strmapi(b, f_strmapi);
			alloc_size = get_last_malloc_size();
			if (alloc_size == size + 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT((int)size + 1, (int)alloc_size);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmapi_zero(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi did not set \\0 at the end of the string");

	SANDBOX_RAISE(
			char	*b = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				b2[i] = f_strmapi(i, b[i]);
			b2[size] = 0;
			MALLOC_MEMSET;
			char	*ret = ft_strmapi(b, f_strmapi);
			MALLOC_RESET;
			if (!memcmp(b2, ret, size + 1))
				exit(TEST_SUCCESS);
			SET_DIFF(ret, b2);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmapi_size(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi did not set \\0 at the end of the string");

	SANDBOX_RAISE(
			char	*b = "override this !";
			size_t	size = strlen(b);
			int		ret_size;

			MALLOC_SIZE;
			ft_strmapi(b, f_strmapi);
			MALLOC_RESET;
			ret_size = get_last_malloc_size();
			if ((int)size + 1 == ret_size)
				exit(TEST_SUCCESS);
			SET_DIFF_INT((int)size + 1, ret_size);
			exit(TEST_KO);
			);
}

void			test_ft_strmapi_free(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi does not malloc ?");

	SANDBOX_RAISE(
			char	*b = "override this !";
			char	b2[0xF0];
			size_t	size = strlen(b);

			for (size_t i = 0; i < size; i++)
				b2[i] = f_strmapi(i, b[i]);
			b2[size] = 0;
			STDOUT_TO_BUFF;
			char *newstr = ft_strmapi(b, f_strmapi);
			if (!strcmp(newstr, b2))
				exit(TEST_SUCCESS);
			free(newstr);
			write(1, "", 1);
			VOID_STDOUT;
			SET_DIFF(b2, newstr);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmapi_malloc_null(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("you did not protect your malloc return");

	SANDBOX_RAISE(
			char	*b = "override this !";

			MALLOC_NULL;
			char *newstr = ft_strmapi(b, f_strmapi);
			MALLOC_RESET;
			if (newstr == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF(NULL, newstr);
			exit(TEST_FAILED);
			);
}

void			test_ft_strmapi_null1(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strmapi(NULL, f_strmapi);
			);
}

void			test_ft_strmapi_null2(void *ptr) {
	char *	(*ft_strmapi)(const char *, char (*)(unsigned, char)) = ptr;
	SET_EXPLANATION("your strmapi does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strmapi("olol", NULL);
			ft_strmapi(NULL, NULL);
			);
}
void            test_ft_strmapi(void){
	add_fun_subtest(test_ft_strmapi_basic);
	add_fun_subtest(test_ft_strmapi_free);
	add_fun_subtest(test_ft_strmapi_zero);
	add_fun_subtest(test_ft_strmapi_malloc_size);
	add_fun_subtest(test_ft_strmapi_malloc_null);
	add_fun_subtest(test_ft_strmapi_null1);
	add_fun_subtest(test_ft_strmapi_null2);
}

////////////////////////////////
//         ft_strequ          //
////////////////////////////////

void			test_ft_strequ_basic(void *ptr) {
	SET_EXPLANATION("your strequ does not work basic input");
	int		(*ft_strequ)(const char *s1, const char *s2) = ptr;

	SANDBOX_RAISE(
			char	*s1 = STRING_1;
			char	*s2 = STRING_2;

			int		i1 = ft_strequ(s1, s2);
			if (i1 == 0)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_strequ_basic2(void *ptr) {
	int		(*ft_strequ)(const char *s1, const char *s2) = ptr;
	SET_EXPLANATION("your strequ does not work with empty string");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "";

			int		i1 = ft_strequ(s1, s2);
			if (i1 == 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_strequ_same_pointer(void *ptr) {
	int		(*ft_strequ)(const char *s1, const char *s2) = ptr;
	SET_EXPLANATION("your strequ does not work with empty string");

	SANDBOX_RAISE(
			char	*s1 = "NYANCAT INSIDE";

			int		i1 = ft_strequ(s1, s1);
			if (i1 == 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_strequ_electric_memory(void *ptr) {
	int		(*ft_strequ)(const char *s1, const char *s2) = ptr;
	SET_EXPLANATION("your strequ crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*s1 = electric_alloc(size);
			char	*s2 = electric_alloc(size);

			strcpy(s1, "rm -rf ~/");
			strcpy(s2, "rm -rf ~/");

			mprotect(s1 - 4096 + size, 4096, PROT_READ);
			mprotect(s2 - 4096 + size, 4096, PROT_READ);

			ft_strequ(s1, s2);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strequ_null1(void *ptr) {
	int		(*ft_strequ)(const char *s1, const char *s2) = ptr;
	SET_EXPLANATION("your strequ does not segfault when null parameter is sent");

	SANDBOX_PROT(
			char	*s1 = "AAAAAAAA";

			ft_strequ(NULL, s1);
			);
}

void			test_ft_strequ_null2(void *ptr) {
	int		(*ft_strequ)(const char *s1, const char *s2) = ptr;
	SET_EXPLANATION("your strequ does not segfault when null parameter is sent");

	SANDBOX_PROT(
			char	*s1 = "AAAAAAAA";

			ft_strequ(s1, NULL);
			ft_strequ(NULL, NULL);
			);
}

void            test_ft_strequ(void){
	add_fun_subtest(test_ft_strequ_basic);
	add_fun_subtest(test_ft_strequ_basic2);
	add_fun_subtest(test_ft_strequ_same_pointer);
	add_fun_subtest(test_ft_strequ_electric_memory);
	add_fun_subtest(test_ft_strequ_null1);
	add_fun_subtest(test_ft_strequ_null2);
}

////////////////////////////////
//         ft_strnequ         //
////////////////////////////////

void			test_ft_strnequ_basic(void *ptr) {
	SET_EXPLANATION("your strnequ does not work with basic input");
	int		(*ft_strnequ)(const char *s1, const char *s2, size_t n) = ptr;

	SANDBOX_RAISE(
			char	*s1 = STRING_1;
			char	*s2 = STRING_2;

			int		i1 = ft_strnequ(s1, s2, strlen(s1));
			if (i1 == 0)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnequ_basic2(void *ptr) {
	int		(*ft_strnequ)(const char *s1, const char *s2, size_t n) = ptr;
	SET_EXPLANATION("your strnequ does not work with empty string");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "";

			int		i1 = ft_strnequ(s1, s2, 1000);
			if (i1 == 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnequ_same_pointer(void *ptr) {
	int		(*ft_strnequ)(const char *s1, const char *s2, size_t n) = ptr;
	SET_EXPLANATION("your strnequ does not work with empty string");

	SANDBOX_RAISE(
			char	*s1 = "NYANCAT INSIDE";

			int		i1 = ft_strnequ(s1, s1, strlen(s1));
			if (i1 == 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(0, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_strnequ_electric_memory(void *ptr) {
	int		(*ft_strnequ)(const char *s1, const char *s2, size_t n) = ptr;
	SET_EXPLANATION("your strnequ crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*s1 = electric_alloc(size);
			char	*s2 = electric_alloc(size);

			strcpy(s1, "rm -rf ~/");
			strcpy(s2, "rm -rf ~/");

			mprotect(s1 - 4096 + size, 4096, PROT_READ);
			mprotect(s2 - 4096 + size, 4096, PROT_READ);

			ft_strnequ(s1, s2, size);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strnequ_null1(void *ptr) {
	int		(*ft_strnequ)(const char *s1, const char *s2, size_t n) = ptr;
	SET_EXPLANATION("your strnequ does not segfault when null parameter is sent");

	SANDBOX_PROT(
			char	*s1 = "AAAAAAAA";

			ft_strnequ(NULL, s1, 3);
			);
}

void			test_ft_strnequ_null2(void *ptr) {
	int		(*ft_strnequ)(const char *s1, const char *s2, size_t n) = ptr;
	SET_EXPLANATION("your strnequ does not segfault when null parameter is sent");

	SANDBOX_PROT(
			char	*s1 = "AAAAAAAA";

			ft_strnequ(s1, NULL, 3);
			ft_strnequ(NULL, NULL, 3);
			);
}

void            test_ft_strnequ(void){
	add_fun_subtest(test_ft_strnequ_basic);
	add_fun_subtest(test_ft_strnequ_basic2);
	add_fun_subtest(test_ft_strnequ_same_pointer);
	add_fun_subtest(test_ft_strnequ_electric_memory);
	add_fun_subtest(test_ft_strnequ_null1);
	add_fun_subtest(test_ft_strnequ_null2);
}

////////////////////////////////
//         ft_substr          //
////////////////////////////////

void			test_ft_substr_basic(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not work with valid input");

	SANDBOX_RAISE(
			char	*str = "i just want this part #############";
			size_t	size = 22;

			char	*ret = ft_substr(str, 0, size);
			if (!strncmp(ret, str, size)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF("i just want this part", ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_substr_basic2(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not work with valid input");

	SANDBOX_RAISE(
			char	*str = "i just want this part #############";
			size_t	size = 20;

			char	*ret = ft_substr(str, 5, size);
			if (!strncmp(ret, str + 5, size)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF("t want this part ###", ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_substr_zero_len(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not work with empty strings");

	SANDBOX_RAISE(
			char	*str = "";
			size_t	size = 0;

			char	*ret = ft_substr(str, 5, size);
			if (!strncmp(ret, str + 5, size)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF("", ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_substr_size(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr did not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char	*str = "i just want this part #############";
			size_t	size = 10;
			int		ret_size;

			MALLOC_SIZE;
			ft_substr(str, 5, size);
			MALLOC_RESET;
			ret_size = get_last_malloc_size();

			if ((int)size + 1 == ret_size) {
				exit(TEST_SUCCESS);
			}
			SET_DIFF_INT((int)size + 1, ret_size);
			exit(TEST_FAILED);
			);
}

void			test_ft_substr_zero(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not set \\0 to the end of the string");

	SANDBOX_RAISE(
			char	str[] = "i just want this part #############";
			size_t	size = 20;

			MALLOC_MEMSET;
			char	*ret = ft_substr(str, 5, size);
			MALLOC_RESET;
			str[size + 5] = 0;
			if (!memcmp(ret, str + 5, size + 1)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF("t want this part ###", ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_substr_malloc_null(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("you did not protect your malloc");

	SANDBOX_RAISE(
			char	*s = "malloc protection !";

			MALLOC_NULL;
			char	*ret = ft_substr(s, 0, 5);
			MALLOC_RESET;
			if (ret == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			(void)s;
			(void)ft_substr;
			);
}

void			test_ft_substr_all(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not work for a whole string");

	SANDBOX_RAISE(
			char	*s = "all of this !";
			size_t	size = strlen(s);

			char	*ret = ft_substr(s, 0, size);

			if (!strcmp(s, ret)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(s, ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_substr_electric_memory(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr crash because it read too many bytes !");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char	*str = electric_alloc(size);

			strcpy(str, "YOLO !!!!");

			ft_substr(str, 0, size);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_substr_null(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_substr(NULL, 0, 12);
			);
}

void			test_ft_substr_big_start(void *ptr) {
	char	*(*ft_substr)(const char *, size_t, size_t) = ptr;
	SET_EXPLANATION("your substr does not work when start >= ft_strlen(s)");

	SANDBOX_RAISE(
			char	*str = "01234";
			size_t	size = 10;

			char	*ret = ft_substr(str, 10, size);
			if (!strncmp(ret, "", 1)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF("", ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void            test_ft_substr(void){
	add_fun_subtest(test_ft_substr_basic);
	add_fun_subtest(test_ft_substr_basic2);
	add_fun_subtest(test_ft_substr_zero_len);
	add_fun_subtest(test_ft_substr_size);
	add_fun_subtest(test_ft_substr_zero);
	add_fun_subtest(test_ft_substr_malloc_null);
	add_fun_subtest(test_ft_substr_all);
	add_fun_subtest(test_ft_substr_electric_memory);
	add_fun_subtest(test_ft_substr_null);
	add_fun_subtest(test_ft_substr_big_start);
}

////////////////////////////////
//         ft_strjoin         //
////////////////////////////////

void			test_ft_strjoin_basic(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("your strjoin does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "my favorite animal is";
			char	*s2 = " ";
			char	*s3 = "the nyancat";

			char	*res = ft_strjoin(ft_strjoin(s1, s2), s3);
			if (!strcmp(res, "my favorite animal is the nyancat"))
				exit(TEST_SUCCESS);
			SET_DIFF("my favorite animal is the nyancat", res);
			exit(TEST_FAILED);
			);
}

void			test_ft_strjoin_free(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("your strjoin does not allocate the memory");

	SANDBOX_RAISE(
			char	*s1 = "my favorite animal is";
			char	*s2 = " ";
			char	*s3 = "the nyancat";

			char	*tmp = ft_strjoin(s1, s2);
			char	*res = ft_strjoin(tmp, s3);
			free(tmp);
			if (!strcmp(res, "my favorite animal is the nyancat")) {
				free(res);
				exit(TEST_SUCCESS);
			}
			SET_DIFF("my favorite animal is the nyancat", res);
			free(res);
			exit(TEST_FAILED);
			);
}

void			test_ft_strjoin_overlap(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("your strjoin does not work with overlap input");

	SANDBOX_RAISE(
			char	*s1 = "my favorite animal is ";
			char	*s2 = s1 + 20;

			char	*res = ft_strjoin(s2, s1);
			if (!strcmp(res, "s my favorite animal is "))
				exit(TEST_SUCCESS);
			SET_DIFF("s my favorite animal is ", res);
			exit(TEST_FAILED);
			);
}

void			test_ft_strjoin_malloc_null(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("you did not protect your malloc return");

	SANDBOX_RAISE(
			char	*s1 = "where is my ";
			char	*s2 = "malloc ???";

			MALLOC_NULL;
			char	*res = ft_strjoin(s2, s1);
			MALLOC_RESET;
			if (res == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, res);
			exit(TEST_FAILED);
			);
}

void			test_ft_strjoin_zero(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("your strjoin does not set \\0 to the end of the string");

	SANDBOX_RAISE(
			char	*s1 = "where is my ";
			char	*s2 = "malloc ???";
			char	*s3 = "where is my malloc ???";

			MALLOC_MEMSET;
			char	*res = ft_strjoin(s1, s2);
			MALLOC_RESET;
			if (!strcmp(res, s3))
				exit(TEST_SUCCESS);
			SET_DIFF(s3, res);
			exit(TEST_FAILED);
			);
}

void			test_ft_strjoin_null1(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("your strjoin does not segfault/return null when null parameter is sent");

	SANDBOX_PROT(
			char	*s1 = "where is my ";

			char	*res = ft_strjoin(NULL, s1);
			if (res == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, res);
			exit(TEST_FAILED);
			);
}

void			test_ft_strjoin_null2(void *ptr) {
	char *	(*ft_strjoin)(char *, char *) = ptr;
	SET_EXPLANATION("your strjoin does not segfault/return null when null parameter is sent");

	SANDBOX_PROT(
			char	*s1 = "where is my ";

			char	*res = ft_strjoin(s1, NULL);
			if (res == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, res);
			exit(TEST_FAILED);
			);
}

void            test_ft_strjoin(void){
	add_fun_subtest(test_ft_strjoin_basic);
	add_fun_subtest(test_ft_strjoin_free);
	add_fun_subtest(test_ft_strjoin_overlap);
	add_fun_subtest(test_ft_strjoin_malloc_null);
	add_fun_subtest(test_ft_strjoin_zero);
	add_fun_subtest(test_ft_strjoin_null1);
	add_fun_subtest(test_ft_strjoin_null2);
}

////////////////////////////////
//         ft_strtrim         //
////////////////////////////////

#define TRIM_SET_PLACEHOLDER " \n\t"

void			test_ft_strtrim_basic(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !\n   \n \n \t\t\n  ";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_basic2(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_basic3(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "Hello \t  Please\n Trim me !";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_empty(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not work with empty input");

	SANDBOX_RAISE(
			char	*s1 = "";
			char	*s2 = "";

			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_blank(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not work with full blank input");

	SANDBOX_RAISE(
			char	*s1 = "  \t \t \n   \n\n\n\t";
			char	*s2 = "";

			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_size(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim did not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char	*s1 = "\t   \n\n\n  \n\n\t    Hello \t  Please\n Trim me !\t\t\t\n  \t\t\t\t  ";
			char	*s2 = "Hello \t  Please\n Trim me !";
			int		r_size = strlen(s2);
			int		size;

			MALLOC_SIZE;
			ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			MALLOC_RESET;
			size = get_last_malloc_size();
			if (size == r_size + 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(r_size + 1, size);
			exit(TEST_KO);
			);
}

void			test_ft_strtrim_free(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not allocate memory");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !\n   \n \n \t\t\n  ";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			if (!strcmp(ret, s2)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(s2, ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_malloc_null(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("you dont protect your malloc return");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !\n   \n \n \t\t\n  ";

			MALLOC_NULL;
			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			MALLOC_RESET;
			if (ret == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_zero(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not set \\0 to the end of the string");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !\n   \n \n \t\t\n  ";
			char	*s2 = "Hello \t  Please\n Trim me !";

			MALLOC_MEMSET;
			char	*ret = ft_strtrim(s1, TRIM_SET_PLACEHOLDER);
			MALLOC_RESET;
			if (!strcmp(s2, ret)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(s2, ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrim_null(void *ptr) {
	char *		(*ft_strtrim)(const char *, const char *) = ptr;
	SET_EXPLANATION("your strtrim does not segfault/return null when null parameter is sent");

	SANDBOX_PROT(
			char	*ret = ft_strtrim(NULL, TRIM_SET_PLACEHOLDER);
			if (!ret)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void            test_ft_strtrim(void){
	add_fun_subtest(test_ft_strtrim_basic);
	add_fun_subtest(test_ft_strtrim_basic2);
	add_fun_subtest(test_ft_strtrim_basic3);
	add_fun_subtest(test_ft_strtrim_blank);
	add_fun_subtest(test_ft_strtrim_empty);
	add_fun_subtest(test_ft_strtrim_size);
	add_fun_subtest(test_ft_strtrim_free);
	add_fun_subtest(test_ft_strtrim_malloc_null);
	add_fun_subtest(test_ft_strtrim_zero);
	add_fun_subtest(test_ft_strtrim_null);
}

////////////////////////////////
//        ft_split         //
////////////////////////////////

void			split_cmp_array(char ** expected, char ** got)
{
	for (; *expected; expected++, got++)
	{
		if (*got == NULL || strcmp(*expected, *got))
		{
			SET_DIFF(*expected, *got);
			exit(TEST_FAILED);
		}
	}

	exit(TEST_SUCCESS);
}

void			test_ft_split_basic(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with basic input");

	SANDBOX_RAISE(
			char	*string = "      split       this for   me  !       ";
			char	**expected = ((char*[6]){"split", "this", "for", "me", "!", NULL});

			char	**result = ft_split(string, ' ');

			split_cmp_array(expected, result);
			);
}

void			test_ft_split_space(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with full space string");

	SANDBOX_RAISE(
			char	**expected = ((char*[1]){NULL});
			char	*string = "                  ";

			char	**result = ft_split(string, ' ');

			split_cmp_array(expected, result);
			);
}

void			test_ft_split_begin(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with one word");
	char	**expected = (char*[2]){"olol", NULL};

	SANDBOX_RAISE(
			char	*s = "                  olol";

			char	**result = ft_split(s, ' ');

			split_cmp_array(expected, result);
			);
}

void			test_ft_split_end(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with one word");
	char	**expected = (char*[2]){"olol", NULL};

	SANDBOX_RAISE(
			char	*s = "olol                     ";

			char	**result = ft_split(s, ' ');

			split_cmp_array(expected, result);
			);
}

void			test_ft_split_empty(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with empty string");
	char	**expected = (char*[2]){NULL};

	SANDBOX_RAISE(
			char	*s = "";
			char	**result = ft_split(s, '\65');

			split_cmp_array(expected, result);
			);
}

void			test_ft_split_full(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with full string");
	char	**expected = (char*[2]){NULL};

	SANDBOX_RAISE(
			char	*s = "0 0 0 0 0 0 0 0 0";
			char	**result = ft_split(s, ' ');

			split_cmp_array(expected, result);
			);
}

void			test_ft_split_free(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with basic input");
	char	**expected = (char*[6]){"split  ", "this", "for", "me", "!", NULL};

	SANDBOX_RAISE(
			char	*s = "split  ||this|for|me|||||!|";
			int		i = 0;
			char	**result = ft_split(s, '|');

			while (result[i]) {
				if (strcmp(result[i], *expected)) {
					SET_DIFF(*expected, result[i]);
					exit(TEST_FAILED);
				}
				free(result[i]);
				i++;
				expected++;
			}
			free(result);

			exit(TEST_SUCCESS);
			);
}

void			test_ft_split_malloc_null(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("you did not protect your split");

	SANDBOX_RAISE(
			char	*s = "      split       this for   me  !       ";

			MALLOC_NULL;
			char	**result = ft_split(s, ' ');
			MALLOC_RESET;
			if (!result)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, result);
			exit(TEST_FAILED);
			);
}

void			test_ft_split_zero(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not work with basic input");
	char	**expected = (char*[6]){"split", "this", "for", "me", "!", NULL};

	SANDBOX_RAISE(
			char	*s = "      split       this for   me  !       ";

			MALLOC_MEMSET;
			char	**result = ft_split(s, ' ');
			MALLOC_RESET;
			while (*result) {
				if (strcmp(*result, *expected)) {
					SET_DIFF(*expected, *result);
					exit(TEST_FAILED);
				}
				result++;
				expected++;
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_split_null(void *ptr) {
	char	**(*ft_split)(char *, char) = ptr;
	SET_EXPLANATION("your split does not segfault/return null when null parameter is sent");

	SANDBOX_PROT(
			char	**expected = ft_split(NULL, ' ');
			if (!expected)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, expected);
			exit(TEST_FAILED);
			)
}

void            test_ft_split(void) {
	add_fun_subtest(test_ft_split_basic);
	add_fun_subtest(test_ft_split_space);
	add_fun_subtest(test_ft_split_begin);
	add_fun_subtest(test_ft_split_end);
	add_fun_subtest(test_ft_split_empty);
	add_fun_subtest(test_ft_split_full);
	add_fun_subtest(test_ft_split_free);
	add_fun_subtest(test_ft_split_malloc_null);
	add_fun_subtest(test_ft_split_zero);
	add_fun_subtest(test_ft_split_null);
}

////////////////////////////////
//         ft_itoa            //
////////////////////////////////

void			test_ft_itoa_basic(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa does not work with basic input");

	SANDBOX_RAISE(
			char	*i1 = ft_itoa(-623);
			char	*i2 = ft_itoa(156);
			char	*i3 = ft_itoa(-0);

			if (strcmp(i1, "-623")) {
				SET_DIFF("-623", i1);
				exit(TEST_FAILED);
			}
			if (strcmp(i2, "156")) {
				SET_DIFF("156", i2);
				exit(TEST_FAILED);
			}
			if (strcmp("0", i3)) {
				SET_DIFF("0", i3);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_itoa_random(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa does not work with random number");

	SANDBOX_RAISE(
			srand(clock());
			int		n;
			char	*d;

			for (int i = 0; i < 2000; i++) {
				n = rand();
				d = ft_itoa(n);
				if (atoi(d) != n) {
					SET_DIFF_INT(n, atoi(d))
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_itoa_max_int(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa does not work with max int");

	SANDBOX_RAISE(
			char	*i1 = ft_itoa(INT_MAX);

			if (atoi(i1) != INT_MAX) {
				SET_DIFF("2147483647", i1);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_itoa_size(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa does not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			int		size;
			char	*i1;

			MALLOC_SIZE;
			i1 = ft_itoa(-5859);
			MALLOC_RESET;
			size = get_last_malloc_size();

			if (size == 6)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(6, size);
			exit(TEST_KO);
			);
}

void			test_ft_itoa_size2(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa does not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			int		size;
			char	*i1;

			MALLOC_SIZE;
			i1 = ft_itoa(0);
			MALLOC_RESET;
			size = get_last_malloc_size();

			if (size == 2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(2, size);
			exit(TEST_KO);
			);
}

void			test_ft_itoa_min_int(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa does not work with min int");

	SANDBOX_RAISE(
			char	*i1 = ft_itoa(INT_MIN);

			if (atoi(i1) != INT_MIN) {
				SET_DIFF("-2147483648", i1);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_itoa_zero_byte(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa don't set \\0 at the end of the string");

	SANDBOX_RAISE(
			MALLOC_MEMSET;
			char	*i1 = ft_itoa(-1234);
			MALLOC_RESET;

			if (strcmp(i1, "-1234")) {
				SET_DIFF("-1234", i1);
				exit(TEST_FAILED);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_itoa_malloc_null(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("you did not protect your malloc");

	SANDBOX_RAISE(
			MALLOC_NULL;
			char	*i1 = ft_itoa(-1234);
			MALLOC_RESET;

			if (!i1)
				exit(TEST_SUCCESS);
			SET_DIFF(NULL, i1);
			exit(TEST_FAILED);
			);
}

void			test_ft_itoa_free_int_min(void *ptr) {
	char	*(*ft_itoa)(int) = ptr;
	SET_EXPLANATION("your itoa did not allocate memory for the int min value");

	SANDBOX_RAISE(
			char	*i1 = ft_itoa(INT_MIN);

			STDERR_TO_BUFF;
			free(i1);
			VOID_STDERR;
			exit(TEST_SUCCESS);
			);
}

void            test_ft_itoa(void){
	add_fun_subtest(test_ft_itoa_basic);
	add_fun_subtest(test_ft_itoa_random);
	add_fun_subtest(test_ft_itoa_max_int);
	add_fun_subtest(test_ft_itoa_min_int);
	add_fun_subtest(test_ft_itoa_zero_byte);
	add_fun_subtest(test_ft_itoa_malloc_null);
	add_fun_subtest(test_ft_itoa_size);
	add_fun_subtest(test_ft_itoa_size2);
	add_fun_subtest(test_ft_itoa_free_int_min);
}

////////////////////////////////
//         ft_putchar         //
////////////////////////////////

void			test_ft_putchar_basic(void *ptr) {
	typeof(putchar)	*ft_putchar = ptr;
	SET_EXPLANATION("your putchar does not work with basic input");

	SANDBOX_RAISE(
			char	buff[10];
			char	c = 'o';
			STDOUT_TO_BUFF;
			ft_putchar(c);
			GET_STDOUT(buff, 10);
			if (buff[0] == c)
				exit(TEST_SUCCESS);
			SET_DIFF(buff, &c);
			exit(TEST_FAILED);
			);
}

void			test_ft_putchar_ascii(void *ptr) {
	typeof(putchar)	*ft_putchar = ptr;
	SET_EXPLANATION("your putchar does not work with all ascii chars");

	SANDBOX_RAISE(
			char	buff[200];
			char	buff2[200];
			STDOUT_TO_BUFF;
			for (int i = 0; i < 128; i++) {
				ft_putchar(i);
				buff2[i] = i;
			}
			GET_STDOUT(buff, 1000);
			for (int i = 0; i < 128; i++)
				if (buff[i] != buff2[i]) {
					SET_DIFF(buff2 + 1, buff + 1);
					exit(TEST_FAILED);
				}
			exit(TEST_SUCCESS);
			);
}

// Here's a famous japanese Haiku! It's good for your culture :)
wchar_t* haiku_0 = L"古池や蛙飛び込む水の音\
ふるいけやかわずとびこむみずのおと";

void			test_ft_putchar_unicode(void *ptr) {
	typeof(putchar)	*ft_putchar = ptr;
	SET_EXPLANATION("your putchar does not work with unicode");

	SANDBOX_RAISE(
			char	buff[10];
			char	buff2[10];
			int		len = 0;

			int i = 0;
			while (haiku_0[i++])
			{
				wchar_t c = haiku_0[i];
				putwchart(c, &len, buff2);
				buff2[len] = 0;

				STDOUT_TO_BUFF;
				ft_putchar(c);
				GET_STDOUT(buff, 10);

				if (strcmp(buff, buff2))
				{
					SET_DIFF(buff2, buff);
					exit(TEST_KO);
				}
			}
			exit(TEST_SUCCESS);

			);
	(void)ft_putchar;
}

void            test_ft_putchar(void){
	add_fun_subtest(test_ft_putchar_basic);
	add_fun_subtest(test_ft_putchar_ascii);
	add_fun_subtest(test_ft_putchar_unicode);
}

////////////////////////////////
//         ft_putstr          //
////////////////////////////////

void			test_ft_putstr_basic(void *ptr) {
	void		(*ft_putstr)(const char *) = ptr;
	SET_EXPLANATION("your putstr does not work with basic input");

	SANDBOX_RAISE(
			char	*buff1 = STRING_1;
			char	buff2[0xF00];

			STDOUT_TO_BUFF;
			ft_putstr(buff1);
			write(1, "", 1);
			GET_STDOUT(buff2, sizeof(buff2));
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putstr_ascii(void *ptr) {
	void		(*ft_putstr)(const char *) = ptr;
	SET_EXPLANATION("your putstr does not work with non ascii chars");

	SANDBOX_RAISE(
			char	*buff1 = "string \x01 of \x63 non \x0a ascii \x12 chars\x1d";
			char	buff2[0xF00];

			STDOUT_TO_BUFF;
			ft_putstr(buff1);
			write(1, "", 1);
			GET_STDOUT(buff2, sizeof(buff2));
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putstr_unicode(void *ptr) {
	void		(*ft_putstr)(const char *) = ptr;
	SET_EXPLANATION("your putstr does not work with non ascii chars");

	SANDBOX_RAISE(
			wchar_t	*buff1 = L"よくやった";
			wchar_t	buff2[0xF00];

//			STDOUT_TO_BUFF;
			ft_putstr((char*)buff1);
			write(1, "", 1);
//			GET_STDOUT((char*)buff2, sizeof(buff2));
			if (!strcmp((char*)buff1, (char*)buff2))
				exit(TEST_SUCCESS);
			SET_DIFF((char*)buff1, (char*)buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putstr_null(void *ptr) {
	void		(*ft_putstr)(const char *) = ptr;
	SET_EXPLANATION("your putstr does not segfault/print something when null parameter is sent");

	SANDBOX_PROT(
			char	buff2[0xF00] = {0};

			STDOUT_TO_BUFF;
			ft_putstr(NULL);
			GET_STDOUT(buff2, sizeof(buff2));
			if (!buff2[0])
				raise(SIGINT);
			SET_DIFF("(null)", buff2);
			exit(TEST_FAILED);
			);
}

void            test_ft_putstr(void){
	add_fun_subtest(test_ft_putstr_basic);
	add_fun_subtest(test_ft_putstr_ascii);
//	add_fun_subtest(test_ft_putstr_unicode);
	add_fun_subtest(test_ft_putstr_null);
}

////////////////////////////////
//         ft_putendl         //
////////////////////////////////

void			test_ft_putendl_basic(void *ptr) {
	void		(*ft_putendl)(const char *) = ptr;
	SET_EXPLANATION("your putendl does not work with basic input");

	SANDBOX_RAISE(
			char	buff1[0xF00] = STRING_1;
			char	buff2[0xF00];

			STDOUT_TO_BUFF;
			ft_putendl(buff1);
			write(1, "", 1);
			GET_STDOUT(buff2, sizeof(buff2));
			strcat(buff1, "\n");
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putendl_ascii(void *ptr) {
	void		(*ft_putendl)(const char *) = ptr;
	SET_EXPLANATION("your putendl does not work with non ascii chars");

	SANDBOX_RAISE(
			char	buff1[0xF00] = "string \x01 of \x63 non \x0a ascii \x12 chars\x1d";
			char	buff2[0xF00];

			STDOUT_TO_BUFF;
			ft_putendl(buff1);
			write(1, "", 1);
			GET_STDOUT(buff2, sizeof(buff2));
			strcat(buff1, "\n");
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putendl_unicode(void *ptr) {
	void		(*ft_putendl)(const char *) = ptr;
	SET_EXPLANATION("your putendl does not work with non ascii chars");

	SANDBOX_RAISE(
			wchar_t	buff1[0xF00] = L"よくやった";
			wchar_t	buff2[0xF00];

//			STDOUT_TO_BUFF;
			ft_putendl((char*)buff1);
			write(1, "", 1);
//			GET_STDOUT((char*)buff2, sizeof(buff2));
			if (!strcmp((char*)buff1, (char*)buff2))
				exit(TEST_SUCCESS);
			SET_DIFF((char*)buff1, (char*)buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putendl_null(void *ptr) {
	void		(*ft_putendl)(const char *) = ptr;
	SET_EXPLANATION("your putendl does not segfault/print something when null parameter is sent");

	SANDBOX_PROT(
			char	buff2[0xF00] = {0};

			STDOUT_TO_BUFF;
			ft_putendl(NULL);
			GET_STDOUT(buff2, sizeof(buff2));
			if (!buff2[0])
				raise(SIGINT);
			SET_DIFF("(null)\n", buff2);
			exit(TEST_FAILED);
			);
}

void            test_ft_putendl(void){
	add_fun_subtest(test_ft_putendl_basic);
	add_fun_subtest(test_ft_putendl_ascii);
//	add_fun_subtest(test_ft_putendl_unicode);
	add_fun_subtest(test_ft_putendl_null);
}

////////////////////////////////
//         ft_putnbr          //
////////////////////////////////

void			test_ft_putnbr_basic(void *ptr) {
	void		(*ft_putnbr)(int) = ptr;
	SET_EXPLANATION("your putnbr does not work");

	SANDBOX_RAISE(
			int		i = 0;
			char	buff[0xF0];

			STDOUT_TO_BUFF;
			ft_putnbr(i);
			GET_STDOUT(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void			test_ft_putnbr_int_max(void *ptr) {
	void		(*ft_putnbr)(int) = ptr;
	SET_EXPLANATION("your putnbr does not work with int max");

	SANDBOX_RAISE(
			int		i = INT_MAX;
			char	buff[0xF0];

			STDOUT_TO_BUFF;
			ft_putnbr(i);
			GET_STDOUT(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void			test_ft_putnbr_int_min(void *ptr) {
	void		(*ft_putnbr)(int) = ptr;
	SET_EXPLANATION("your putnbr does not work with int min");

	SANDBOX_RAISE(
			int		i = INT_MIN;
			char	buff[0xF0];

			STDOUT_TO_BUFF;
			ft_putnbr(i);
			GET_STDOUT(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

#define ASSERT_PUTNBR(n1, n2) if (n1 != n2) { SET_DIFF_INT(n1, n2); exit(TEST_FAILED); }
void			test_ft_putnbr_random(void *ptr) {
	void		(*ft_putnbr)(int) = ptr;
	SET_EXPLANATION("your putnbr does not work with random numbers")

	SANDBOX_RAISE(
			int		nbr;
			char	buff[0xF0];

			srand(clock());
			nbr = rand();
			STDOUT_TO_BUFF;
			ft_putnbr(nbr);
			GET_STDOUT(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			nbr = rand();
			STDOUT_TO_BUFF;
			ft_putnbr(nbr);
			GET_STDOUT(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			nbr = rand();
			STDOUT_TO_BUFF;
			ft_putnbr(nbr);
			GET_STDOUT(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			nbr = rand();
			STDOUT_TO_BUFF;
			ft_putnbr(nbr);
			GET_STDOUT(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			exit(TEST_SUCCESS);
			);
}

void			test_ft_putnbr_itoa(void *ptr) {
	void		(*ft_putnbr)(int) = ptr;
	SET_EXPLANATION("your putnbr allocate memory, wtf ???");

	SANDBOX_RAISE(
			int		i = INT_MIN;
			char	buff[0xF0];

			STDOUT_TO_BUFF;
			MALLOC_NULL;
			ft_putnbr(i);
			MALLOC_RESET;
			GET_STDOUT(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void            test_ft_putnbr(void){
	add_fun_subtest(test_ft_putnbr_basic);
	add_fun_subtest(test_ft_putnbr_int_max);
	add_fun_subtest(test_ft_putnbr_int_min);
	add_fun_subtest(test_ft_putnbr_random);
	add_fun_subtest(test_ft_putnbr_itoa);
}

////////////////////////////////
//       ft_putchar_fd        //
////////////////////////////////

void			test_ft_putchar_fd_basic(void *ptr) {
	void		(*ft_putchar_fd)(int fd, int c) = ptr;
	SET_EXPLANATION("your putchar_fd does not work with basic input");

	SANDBOX_RAISE(
			char	buff[10];
			char	c = 'o';
			STDERR_TO_BUFF;
			ft_putchar_fd(c, STDERR_FILENO);
			GET_STDERR(buff, 10);
			if (buff[0] == c)
				exit(TEST_SUCCESS);
			SET_DIFF(&c, buff);
			exit(TEST_FAILED);
			);
}

void			test_ft_putchar_fd_ascii(void *ptr) {
	void		(*ft_putchar_fd)(int fd, int c) = ptr;
	SET_EXPLANATION("your putchar_fd does not work with all ascii chars");

	SANDBOX_RAISE(
			char	buff[200];
			char	buff2[200];
			STDERR_TO_BUFF;
			for (int i = 0; i < 128; i++) {
				ft_putchar_fd(i, STDERR_FILENO);
				buff2[i] = i;
			}
			GET_STDERR(buff, 1000);
			for (int i = 0; i < 128; i++)
				if (buff[i] != buff2[i]) {
					SET_DIFF(buff2 + 1, buff + 1);
					exit(TEST_FAILED);
				}
			exit(TEST_SUCCESS);
			);
}

void            test_ft_putchar_fd(void){
	add_fun_subtest(test_ft_putchar_fd_basic);
	add_fun_subtest(test_ft_putchar_fd_ascii);
}

////////////////////////////////
//       ft_putstr_fd         //
////////////////////////////////

void			test_ft_putstr_fd_basic(void *ptr) {
	void		(*ft_putstr_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putstr_fd does not work with basic input");

	SANDBOX_RAISE(
			char	*buff1 = STRING_1;
			char	buff2[0xF00];

			STDERR_TO_BUFF;
			ft_putstr_fd(buff1, STDERR_FILENO);
			write(1, "", 1);
			GET_STDERR(buff2, sizeof(buff2));
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putstr_fd_ascii(void *ptr) {
	void		(*ft_putstr_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putstr_fd does not work with non ascii chars");

	SANDBOX_RAISE(
			char	*buff1 = "string \x01 of \x63 non \x0a ascii \x12 chars\x1d";
			char	buff2[0xF00];

			STDERR_TO_BUFF;
			ft_putstr_fd(buff1, STDERR_FILENO);
			write(1, "", 1);
			GET_STDERR(buff2, sizeof(buff2));
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putstr_fd_unicode(void *ptr) {
	void		(*ft_putstr_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putstr_fd does not work with non ascii chars");

	SANDBOX_RAISE(
			wchar_t	*buff1 = L"よくやった";
			wchar_t	buff2[0xF00];

//			STDERR_TO_BUFF;
			ft_putstr_fd((char*)buff1, STDERR_FILENO);
			write(1, "", 1);
//			GET_STDERR((char*)buff2, sizeof(buff2));
			if (!strcmp((char*)buff1, (char*)buff2))
				exit(TEST_SUCCESS);
			SET_DIFF((char*)buff1, (char*)buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putstr_fd_null(void *ptr) {
	void		(*ft_putstr_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putstr_fd does not segfault/print something when null parameter is sent");

	SANDBOX_PROT(
			char	buff2[0xF00] = {0};

			STDERR_TO_BUFF;
			ft_putstr_fd(NULL, STDERR_FILENO);
			GET_STDERR(buff2, sizeof(buff2));
			if (!buff2[0])
				raise(SIGINT);
			SET_DIFF("(null)", buff2);
			exit(TEST_FAILED);
			);
}

void            test_ft_putstr_fd(void){
	add_fun_subtest(test_ft_putstr_fd_basic);
	add_fun_subtest(test_ft_putstr_fd_ascii);
//	add_fun_subtest(test_ft_putstr_fd_unicode);
	add_fun_subtest(test_ft_putstr_fd_null);
}

////////////////////////////////
//       ft_putendl_fd        //
////////////////////////////////

void			test_ft_putendl_fd_basic(void *ptr) {
	void		(*ft_putendl_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putendl_fd does not work with basic input");

	SANDBOX_RAISE(
			char	buff1[0xF00] = STRING_1;
			char	buff2[0xF00];

			STDERR_TO_BUFF;
			ft_putendl_fd(buff1, STDERR_FILENO);
			write(1, "", 1);
			GET_STDERR(buff2, sizeof(buff2));
			strcat(buff1, "\n");
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putendl_fd_ascii(void *ptr) {
	void		(*ft_putendl_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putendl_fd does not work with non ascii chars");

	SANDBOX_RAISE(
			char	buff1[0xF00] = "string \x01 of \x63 non \x0a ascii \x12 chars\x1d";
			char	buff2[0xF00];

			STDERR_TO_BUFF;
			ft_putendl_fd(buff1, STDERR_FILENO);
			write(1, "", 1);
			GET_STDERR(buff2, sizeof(buff2));
			strcat(buff1, "\n");
			if (!strcmp(buff1, buff2))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putendl_fd_unicode(void *ptr) {
	void		(*ft_putendl_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putendl_fd does not work with non ascii chars");

	SANDBOX_RAISE(
			wchar_t	buff1[0xF00] = L"よくやった";
			wchar_t	buff2[0xF00];

//			STDERR_TO_BUFF;
			ft_putendl_fd((char*)buff1, STDERR_FILENO);
			write(1, "", 1);
//			GET_STDERR((char*)buff2, sizeof(buff2));
			if (!strcmp((char*)buff1, (char*)buff2))
				exit(TEST_SUCCESS);
			SET_DIFF((char*)buff1, (char*)buff2);
			exit(TEST_FAILED);
			);
}

void			test_ft_putendl_fd_null(void *ptr) {
	void		(*ft_putendl_fd)(const char *, int fd) = ptr;
	SET_EXPLANATION("your putendl_fd does not segfault/print something when null parameter is sent");

	SANDBOX_PROT(
			char	buff2[0xF00] = {0};

			STDERR_TO_BUFF;
			ft_putendl_fd(NULL, STDERR_FILENO);
			GET_STDERR(buff2, sizeof(buff2));
			if (!buff2[0])
				raise(SIGINT);
			SET_DIFF("(null)\n", buff2);
			exit(TEST_FAILED);
			);
}

void            test_ft_putendl_fd(void){
	add_fun_subtest(test_ft_putendl_fd_basic);
	add_fun_subtest(test_ft_putendl_fd_ascii);
//	add_fun_subtest(test_ft_putendl_fd_unicode);
	add_fun_subtest(test_ft_putendl_fd_null);
}



////////////////////////////////
//       ft_putnbr_fd         //
////////////////////////////////

void			test_ft_putnbr_fd_basic(void *ptr) {
	void		(*ft_putnbr_fd)(int, int fd) = ptr;
	SET_EXPLANATION("your putnbr_fd does not work");

	SANDBOX_RAISE(
			int		i = 0;
			char	buff[0xF0];

			STDERR_TO_BUFF;
			ft_putnbr_fd(i, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void			test_ft_putnbr_fd_int_max(void *ptr) {
	void		(*ft_putnbr_fd)(int, int fd) = ptr;
	SET_EXPLANATION("your putnbr_fd does not work with int max");

	SANDBOX_RAISE(
			int		i = INT_MAX;
			char	buff[0xF0];

			STDERR_TO_BUFF;
			ft_putnbr_fd(i, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void			test_ft_putnbr_fd_int_min(void *ptr) {
	void		(*ft_putnbr_fd)(int, int fd) = ptr;
	SET_EXPLANATION("your putnbr_fd does not work with int min");

	SANDBOX_RAISE(
			int		i = INT_MIN;
			char	buff[0xF0];

			STDERR_TO_BUFF;
			ft_putnbr_fd(i, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void			test_ft_putnbr_fd_random(void *ptr) {
	void		(*ft_putnbr_fd)(int, int fd) = ptr;
	SET_EXPLANATION("your putnbr_fd does not work with random numbers")

	SANDBOX_RAISE(
			int		nbr;
			char	buff[0xF0];

			srand(clock());
			nbr = rand();
			STDERR_TO_BUFF;
			ft_putnbr_fd(nbr, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			int		nb = atoi(buff);
			ASSERT_PUTNBR(nbr, nb);
			srand(clock());
			nbr = rand();
			STDERR_TO_BUFF;
			ft_putnbr_fd(nbr, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			srand(clock());
			nbr = rand();
			STDERR_TO_BUFF;
			ft_putnbr_fd(nbr, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			srand(clock());
			nbr = rand();
			STDERR_TO_BUFF;
			ft_putnbr_fd(nbr, STDERR_FILENO);
			GET_STDERR(buff, 0xF0);
			ASSERT_PUTNBR(nbr, atoi(buff));
			exit(TEST_SUCCESS);
			);
}

void			test_ft_putnbr_fd_itoa(void *ptr) {
	void		(*ft_putnbr_fd)(int, int) = ptr;
	SET_EXPLANATION("your putnbr_fd allocate memory, wtf ???");

	SANDBOX_RAISE(
			int		i = INT_MIN;
			char	buff[0xF0];

			STDERR_TO_BUFF;
			MALLOC_NULL;
			ft_putnbr_fd(i, STDERR_FILENO);
			MALLOC_RESET;
			GET_STDERR(buff, 0xF0);
			if (atoi(buff) == i)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(i, atoi(buff));
			exit(TEST_FAILED);
			);
}

void            test_ft_putnbr_fd(void){
	add_fun_subtest(test_ft_putnbr_fd_basic);
	add_fun_subtest(test_ft_putnbr_fd_int_max);
	add_fun_subtest(test_ft_putnbr_fd_int_min);
	add_fun_subtest(test_ft_putnbr_fd_random);
	add_fun_subtest(test_ft_putnbr_fd_itoa);
}



////////////////////////////////
////////////////////////////////
//          Part III          //
////////////////////////////////
////////////////////////////////


////////////////////////////////
//        ft_lstnew           //
////////////////////////////////

void			test_ft_lstnew_basic(void *ptr) {
	t_list	*(*ft_lstnew)(const void *) = ptr;
	SET_EXPLANATION("your lstnew does not work with basic input");

	SANDBOX_RAISE(
			char	*data = "hello, i'm a data";
			t_list	*l = ft_lstnew(data);

			if (!strcmp(data, l->content))
				exit(TEST_SUCCESS);
			SET_DIFF(data, l->content);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstnew_free(void *ptr) {
	t_list	*(*ft_lstnew)(const void *) = ptr;
	SET_EXPLANATION("your lstnew does not allocate memory");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			char	*data = "hello, i'm a data";
			t_list	*l = ft_lstnew(data);
			write(STDERR_FILENO, "", 1);

			if (!strcmp(data, l->content)) {
				free(l);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(data, l->content);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstnew_null(void *ptr) {
	t_list	*(*ft_lstnew)(const void *) = ptr;
	SET_EXPLANATION("your lstnew does not work with null parameter");

	SANDBOX_RAISE(
			t_list	*l = ft_lstnew(NULL);

			if (!l->content)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, l->content);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstnew_malloc_null(void *ptr) {
	t_list	*(*ft_lstnew)(const void *) = ptr;
	SET_EXPLANATION("your malloc return is not protected");

	SANDBOX_RAISE(
			char	*data = "hello, i'm a data";

			MALLOC_NULL;
			t_list	*l = ft_lstnew(data);
			MALLOC_RESET;
			if (!l)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, l);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstnew(void){
	add_fun_subtest(test_ft_lstnew_basic);
	add_fun_subtest(test_ft_lstnew_free);
	add_fun_subtest(test_ft_lstnew_null);
	add_fun_subtest(test_ft_lstnew_malloc_null);
}

////////////////////////////////
//        ft_lstdelone        //
////////////////////////////////

static bool		g_delone_called;
void			lstdelone_f(void *d) {
	free(d);
	g_delone_called = true;
}

t_list			*lstnew(void *d) {
	t_list *ret = malloc(sizeof(t_list));

	if (!ret)
		return (NULL);
	ret->next = NULL;
	ret->content = d;
	return (ret);
}

void			test_ft_lstdelone_basic(void *ptr) {
	void		(*ft_lstdelone)(t_list *, void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstdelone does not work");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l = lstnew(malloc(10));

			g_delone_called = false;
			ft_lstdelone(l, lstdelone_f);

      write(STDERR_FILENO, "", 1);
			if (g_delone_called)
				exit(TEST_SUCCESS);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstdelone(void) {
	add_fun_subtest(test_ft_lstdelone_basic);
}

////////////////////////////////
//         ft_lstclear        //
////////////////////////////////

int				__delNum = 0;
void			lstdel_f(void *lst) {
	(void)lst;
	__delNum++;
}

void			test_ft_lstclear_basic(void *ptr) {
	void		(*ft_lstclear)(t_list **, void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstclear does not work with basic input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup("nyancat"));

			l->next = lstnew(strdup("#TEST#"));
			ft_lstclear(&l, lstdelone_f);
			write(STDERR_FILENO, "", 1);
			if (!l)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstclear_free(void *ptr) {
	void		(*ft_lstclear)(t_list **, void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstclear does not free the list");

	STDERR_TO_BUFF;
	SANDBOX_IRAISE(
			t_list	*l = lstnew(strdup("nyancat"));
			t_list	*tmp;

			l->next = lstnew(strdup("#TEST#"));
			tmp = l->next;
			ft_lstclear(&l, lstdelone_f);
			write(STDERR_FILENO, "", 1);

			if (!l) {
				free(tmp);
				exit(TEST_SUCCESS);
			}
			SET_DIFF_PTR(NULL, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstclear_number(void *ptr) {
	void		(*ft_lstclear)(t_list **, void (*)(void *)) = ptr;
	SET_EXPLANATION("bad call number of the function pointer");
	t_list	*list;

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			char	*content = "hello !";

			__delNum = 0;
			list = malloc(sizeof(t_list));
			bzero(list, sizeof(t_list));
			list->next = malloc(sizeof(t_list));
			bzero(list->next, sizeof(t_list));
			list->content = content;
			list->next->content = content + 2;
			ft_lstclear(&list, lstdel_f);
			write(STDERR_FILENO, "", 1);
			if (__delNum == 2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(2, __delNum);
			exit(TEST_FAILED);
	)
	VOID_STDERR;
}

void			test_ft_lstclear(void) {
	add_fun_subtest(test_ft_lstclear_basic);
	add_fun_subtest(test_ft_lstclear_free);
	add_fun_subtest(test_ft_lstclear_number);
}

////////////////////////////////
//      ft_lstadd_front       //
////////////////////////////////

void			test_ft_lstadd_front_basic(void *ptr) {
	void	(*ft_lstadd_front)(t_list **, t_list *new) = ptr;
	SET_EXPLANATION("your lstadd_front does not work with basic input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup("nyacat"));
			t_list	*n = lstnew(strdup("OK"));

			ft_lstadd_front(&l, n);

      if (l == n && !strcmp(l->content, "OK")) {
				exit(TEST_SUCCESS);
			}
			SET_DIFF_PTR(n, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstadd_front_free(void *ptr) {
	void	(*ft_lstadd_front)(t_list **, t_list *new) = ptr;
	SET_EXPLANATION("your lstadd_front does not work with basic input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup("nyacat"));
			t_list	*n = lstnew(strdup("OK"));

			ft_lstadd_front(&l, n);

      if (l == n && !strcmp(l->content, "OK")) {
				free(l->next);
				free(l);
				exit(TEST_SUCCESS);
			}
			free(l->next);
			free(l);
			SET_DIFF_PTR(n, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstadd_front_null(void *ptr) {
	void	(*ft_lstadd_front)(t_list **, t_list *new) = ptr;
	SET_EXPLANATION("your lstadd_front does not work with null node input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l =  NULL;
			t_list	*n = lstnew(strdup("OK"));

			ft_lstadd_front(&l, n);
			if (l == n && !strcmp(l->content, "OK")) {
				free(l->next);
				free(l);
				exit(TEST_SUCCESS);
			}
			free(l->next);
			free(l);
			SET_DIFF_PTR(l, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstadd_front(void){
	add_fun_subtest(test_ft_lstadd_front_basic);
	add_fun_subtest(test_ft_lstadd_front_free);
	add_fun_subtest(test_ft_lstadd_front_null);
}

////////////////////////////////
//     ft_lstadd_back        //
////////////////////////////////

void			test_ft_lstadd_back_basic(void *ptr) {
	void	(*ft_lstadd_back)(t_list **, t_list *new) = ptr;
	SET_EXPLANATION("your lstadd_back does not work with basic input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup("nyacat"));
			t_list	*n = lstnew(strdup("OK"));

			ft_lstadd_back(&l, n);
			if (l->next == n && !strcmp(l->next->content, "OK")) {
				exit(TEST_SUCCESS);
			}
			SET_DIFF_PTR(n, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstadd_back_free(void *ptr) {
	void	(*ft_lstadd_back)(t_list **, t_list *new) = ptr;
	SET_EXPLANATION("your lstadd_back does not work with basic input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup("nyacat"));
			t_list	*n = lstnew(strdup("OK"));

			ft_lstadd_back(&l, n);
			if (l->next == n && !strcmp(l->next->content, "OK")) {
				free(l->next);
				free(l);
				exit(TEST_SUCCESS);
			}
			free(l->next);
			free(l);
			SET_DIFF_PTR(n, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstadd_back_null(void *ptr) {
	void	(*ft_lstadd_back)(t_list **, t_list *new) = ptr;
	SET_EXPLANATION("your lstadd_back does not work with null node input");

	STDERR_TO_BUFF;
	SANDBOX_RAISE(
			t_list	*l =  NULL;
			t_list	*n = lstnew(strdup("OK"));

			ft_lstadd_back(&l, n);
			if (l == n && !strcmp(l->content, "OK")) {
				free(l->next);
				free(l);
				exit(TEST_SUCCESS);
			}
			free(l->next);
			free(l);
			SET_DIFF_PTR(l, l);
			exit(TEST_FAILED);
			);
	VOID_STDERR;
}

void			test_ft_lstadd_back(void){
	add_fun_subtest(test_ft_lstadd_back_basic);
	add_fun_subtest(test_ft_lstadd_back_free);
	add_fun_subtest(test_ft_lstadd_back_null);
}

////////////////////////////////
//        ft_lstsize          //
////////////////////////////////

void			test_ft_lstsize_basic(void *ptr) {
	int		(*ft_lstsize)(t_list *) = ptr;
	SET_EXPLANATION("your lstsize does not work with basic input");

	SANDBOX_RAISE(
			t_list	*l;
			int actual;
			int expected;
	
			l = lstnew(strdup("1"));
			l->next = lstnew(strdup("2"));
			l->next->next = lstnew(strdup("3"));
			expected = 3;
			actual = ft_lstsize(l);
			if (actual == expected)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(expected, actual);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstsize_null(void *ptr) {
	int		(*ft_lstsize)(t_list *) = ptr;
	SET_EXPLANATION("your lstsize does not work with basic input");

	SANDBOX_RAISE(
			t_list	*l = NULL;
			int actual;
			int expected = 0;
	
			actual = ft_lstsize(l);
			if (actual == expected)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(expected, actual);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstsize(void){
	add_fun_subtest(test_ft_lstsize_basic);
	add_fun_subtest(test_ft_lstsize_null);
}

////////////////////////////////
//        ft_lstlast          //
////////////////////////////////

void			test_ft_lstlast_basic(void *ptr) {
	t_list *	(*ft_lstlast)(t_list *) = ptr;
	SET_EXPLANATION("your lstlast does not work with basic input");

	SANDBOX_RAISE(
			t_list	*l;
			t_list	*expected;
			t_list	*actual;
	
			l = lstnew(strdup("1"));
			l->next = lstnew(strdup("2"));
			l->next->next = lstnew(strdup("3"));
			expected = l->next->next;
			actual = ft_lstlast(l);
			if (actual == expected)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(expected, actual);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstlast_null(void *ptr) {
	t_list *	(*ft_lstlast)(t_list *) = ptr;
	SET_EXPLANATION("your lstlast does not work with basic input");

	SANDBOX_RAISE(
			t_list	*l = NULL;
			t_list	*expected;
			t_list	*actual;

			expected = NULL;
			actual = ft_lstlast(l);
			if (actual == expected)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(expected, actual);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstlast(void){
	add_fun_subtest(test_ft_lstlast_basic);
	add_fun_subtest(test_ft_lstlast_null);
}

// TODO: Add back, lst size and lst last

////////////////////////////////
//        ft_lstiter          //
////////////////////////////////

void			lstiter_f(void *content) {
	char *s = content;

	s[0] = 'O';
	s[1] = 'K';
}

void			test_ft_lstiter_basic(void *ptr) {
	void	(*ft_lstiter)(t_list *, void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstiter does not work with basic input");

	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup("KO !"));

			l->next = lstnew(strdup("KO !"));
			l->next->next = lstnew(strdup("KO !"));
      		ft_lstiter(l, lstiter_f);
			if (!strcmp(l->content, "OK !") && !strcmp(l->next->content, "OK !") && !strcmp(l->next->next->content, "OK !"))
				exit(TEST_SUCCESS);
			SET_DIFF("OK !", l->content);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstiter_null(void *ptr) {
	void	(*ft_lstiter)(t_list *, void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstiter does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_lstiter(NULL, lstiter_f);
			);
}

void			test_ft_lstiter(void){
	add_fun_subtest(test_ft_lstiter_basic);
	add_fun_subtest(test_ft_lstiter_null);
}

////////////////////////////////
//         ft_lstmap          //
////////////////////////////////

void *		lstmap_f(void *content) {
	(void)content;
	return ("OK !");
}

void			test_ft_lstmap_basic(void *ptr) {
	t_list *	(*ft_lstmap)(t_list *, void * (*)(void *), void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstmap does not work with basic input");

	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup(" 1 2 3 "));
			t_list	*ret;

			l->next = lstnew(strdup("ss"));
			l->next->next = lstnew(strdup("-_-"));
			ret = ft_lstmap(l, lstmap_f, NULL);
			if (!strcmp(ret->content, "OK !") && !strcmp(ret->next->content, "OK !") && !strcmp(ret->next->next->content, "OK !") && !strcmp(l->content, " 1 2 3 ") && !strcmp(l->next->content, "ss") && !strcmp(l->next->next->content, "-_-"))
				exit(TEST_SUCCESS);
			SET_DIFF(" 1 2 3 ", l->content);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstmap_null(void *ptr) {
	t_list *	(*ft_lstmap)(t_list *, void * (*)(void *), void (*)(void *)) = ptr;
	SET_EXPLANATION("your lstmap does not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_lstmap(NULL, lstmap_f, NULL);
			);
}

void			test_ft_lstmap_malloc_null(void *ptr) {
	t_list *	(*ft_lstmap)(t_list *, void *(*)(void *)) = ptr;
	SET_EXPLANATION("you did not protect your malloc");

	SANDBOX_RAISE(
			t_list	*l = lstnew(strdup(" 1 2 3 "));
			t_list	*ret;

			l->next = lstnew(strdup("ss"));
			l->next->next = lstnew(strdup("-_-"));
			MALLOC_DEBUG;
			ret = ft_lstmap(l, lstmap_f);
			MALLOC_RESET;
			if (!ret)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_lstmap(void){
	add_fun_subtest(test_ft_lstmap_basic);
	add_fun_subtest(test_ft_lstmap_null);
//	add_fun_subtest(test_ft_lstmap_malloc_null);
}

////////////////////////////////
//         ft_islower         //
////////////////////////////////

void			test_ft_islower_(void *ptr) {
	typeof(islower)	*ft_islower = ptr;
	SET_EXPLANATION("your islower does not work ...");

	SANDBOX_RAISE(
			for (int i = -50; i < 530; i++) {
				if (!!ft_islower(i) != !!islower(i)) {
					SET_DIFF_INT(islower(i), ft_islower(i));
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_islower(void) {
	add_fun_subtest(test_ft_islower_);
}

////////////////////////////////
//         ft_isupper         //
////////////////////////////////

void			test_ft_isupper_(void *ptr) {
	typeof(isupper)	*ft_isupper = ptr;
	SET_EXPLANATION("your isupper does not work ...");

	SANDBOX_RAISE(
			for (int i = -50; i < 530; i++) {
				if (!!ft_isupper(i) != !!isupper(i)) {
					SET_DIFF_INT(isupper(i), ft_isupper(i));
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_isupper(void) {
	add_fun_subtest(test_ft_isupper_);
}

////////////////////////////////
//         ft_isnumber         //
////////////////////////////////

#ifdef linux
int				isnumber(char c) { const char *numbers="0123456789"; return strchr(numbers, c) != NULL; }
#endif

void			test_ft_isnumber_(void *ptr) {
	typeof(isnumber)	*ft_isnumber = ptr;
	SET_EXPLANATION("your isnumber does not work ...");

	SANDBOX_RAISE(
			for (int i = -50; i < 530; i++) {
				if (!!ft_isnumber(i) != !!isnumber(i)) {
					SET_DIFF_INT(isnumber(i), ft_isnumber(i));
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_isnumber(void) {
	add_fun_subtest(test_ft_isnumber_);
}

////////////////////////////////
//         ft_isblank         //
////////////////////////////////

void			test_ft_isblank_(void *ptr) {
	typeof(isblank)	*ft_isblank = ptr;
	SET_EXPLANATION("your isblank does not work ...");

	SANDBOX_RAISE(
			for (int i = -50; i < 530; i++) {
				if (!!ft_isblank(i) != !!isblank(i)) {
					SET_DIFF_INT(isblank(i), ft_isblank(i));
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_isblank(void) {
	add_fun_subtest(test_ft_isblank_);
}

////////////////////////////////
//         ft_strtrimc         //
////////////////////////////////

void			test_ft_strtrimc_basic(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "\t\t\t\t\t\t\t\tHello \t  Please\n Trim me !\t\t\t\t\t\t\t";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrimc(s1, '\t');
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_basic2(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "                   Hello \t  Please\n Trim me !";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrimc(s1, ' ');
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_basic3(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not work with basic input");

	SANDBOX_RAISE(
			char	*s1 = "Hello \t  Please\n Trim me !";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrimc(s1, ' ');
			if (!strcmp(ret, s2))
				exit(TEST_SUCCESS);
			SET_DIFF(s2, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_size(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc did not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char	*s1 = "\t\t\t\t\t\t\t\tHello \t  Please\n Trim me !\t\t\t\t\t\t\t";
			char	*s2 = "Hello \t  Please\n Trim me !";
			int		r_size = strlen(s2);
			int		size;

			MALLOC_SIZE;
			ft_strtrimc(s1, '\t');
			MALLOC_RESET;
			size = get_last_malloc_size();
			if (size == r_size + 1)
				exit(TEST_SUCCESS);
			SET_DIFF_INT(r_size + 1, size);
			exit(TEST_KO);
			);
}

void			test_ft_strtrimc_free(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not allocate memory");

	SANDBOX_RAISE(
			char	*s1 = "\x12\x12\x12Hello \t  Please\n Trim me !\x12";
			char	*s2 = "Hello \t  Please\n Trim me !";

			char	*ret = ft_strtrimc(s1, '\x12');
			if (!strcmp(ret, s2)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(s2, ret);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_not_found(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not work with not found character ");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !\n   \n \n \t\t\n  ";

			char	*ret = ft_strtrimc(s1, '\xff');
			if (!strcmp(s1, ret))
				exit(TEST_SUCCESS);
			SET_DIFF(s1, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_malloc_null(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("you did not protect your malloc return");

	SANDBOX_RAISE(
			char	*s1 = "   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me !\n   \n \n \t\t\n  ";

			MALLOC_NULL;
			char	*ret = ft_strtrimc(s1, '\xff');
			MALLOC_RESET;
			if (ret == NULL)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_zero(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not set \\0 to the end of the string");

	SANDBOX_RAISE(
			char	*s1 = "\n\n\n\n\nHello \t  Please\n Trim me ! \n\n\n\n\n";
			char	*s2 = "Hello \t  Please\n Trim me ! ";

			MALLOC_MEMSET;
			char	*ret = ft_strtrimc(s1, '\n');
			MALLOC_RESET;
			if (!strcmp(s2, ret)) {
				free(ret);
				exit(TEST_SUCCESS);
			}
			SET_DIFF(ret, s2);
			free(ret);
			exit(TEST_FAILED);
			);
}

void			test_ft_strtrimc_null(void *ptr) {
	char *		(*ft_strtrimc)(const char *, char c) = ptr;
	SET_EXPLANATION("your strtrimc does not segfault/return null when null parameter is sent");

	SANDBOX_PROT(
			char	*ret = ft_strtrimc(NULL, ' ');
			if (!ret)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ret);
			exit(TEST_FAILED);
			);
}

void            test_ft_strtrimc(void){
	add_fun_subtest(test_ft_strtrimc_basic);
	add_fun_subtest(test_ft_strtrimc_basic2);
	add_fun_subtest(test_ft_strtrimc_basic3);
	add_fun_subtest(test_ft_strtrimc_size);
	add_fun_subtest(test_ft_strtrimc_not_found);
	add_fun_subtest(test_ft_strtrimc_free);
	add_fun_subtest(test_ft_strtrimc_malloc_null);
	add_fun_subtest(test_ft_strtrimc_zero);
	add_fun_subtest(test_ft_strtrimc_null);
}

////////////////////////////////
//         ft_strndup          //
////////////////////////////////

void			test_ft_strndup_last_char(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup does not add \\0 at the end of the string");

	SANDBOX_RAISE(
			char 	*str;
			char	*tmp = "HAHAHA \0 tu me vois pas !";

			MALLOC_MEMSET;
			str = ft_strndup(tmp, 12);
			MALLOC_RESET;
			if (strcmp(str, tmp))
				exit(TEST_FAILED);
			free(str);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strndup_zero(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup don't work with empty string");

	SANDBOX_RAISE(
			char 	*str;
			char	*tmp = "";

			str = ft_strndup(tmp, 5);
			if (strcmp(str, tmp))
				exit(TEST_FAILED);
			free(str);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strndup_size(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup did not allocate the good size so the \\0 test may be false");

	SANDBOX_RAISE(
			char 	*str;
			char	*tmp = "this is a normal test";
			int		r_size = 5;
			int		size;

			MALLOC_SIZE;
			str = ft_strndup(tmp, r_size);
			MALLOC_RESET;
			size = get_last_malloc_size();
			if (size != r_size + 1) {
				SET_DIFF_INT(r_size + 1, size);
				exit(TEST_KO);
			}
			exit(TEST_SUCCESS);
			);
}

void			test_ft_strndup_null(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup does not segv with NULL");

	SANDBOX_IRAISE(
			ft_strndup(NULL, 5);
			);
}

void			test_ft_strndup_malloc_null(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("you dindn't protect your malloc return");

	SANDBOX_RAISE(
			char	*ptr;

			MALLOC_NULL;
			ptr = ft_strndup("lol", 5);
			MALLOC_RESET;
			if (!ptr)
				exit(TEST_SUCCESS);
			SET_DIFF_PTR(NULL, ptr);
			exit(TEST_FAILED);
			);
	(void)ft_strndup;
}

void			test_ft_strndup_basic(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup doesn't work with basic input");

	SANDBOX_RAISE(
			char	*str;
			char	*tmp = "I malloc so I am.";

			str = ft_strndup(tmp, strlen(tmp));
			if (strcmp(str, tmp)) {
				SET_DIFF(str, tmp);
				exit(TEST_FAILED);
			}
			free(str);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strndup_electric_memory(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup doesn't work with basic input");

	SANDBOX_RAISE(
			const size_t	size = 10;
			char			*str = electric_alloc(size);

			strcpy(str, "!@#$%^&*(");
			mprotect(str - 4096 + size, 4096, PROT_READ);

			ft_strndup(str, size);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strndup_basic2(void *ptr) {
	typeof(strndup)	*ft_strndup = ptr;
	SET_EXPLANATION("your strndup doesn't work with basic input");

	SANDBOX_RAISE(
			char	*str;
			char	*tmp = "I malloc so I am.";
			char	*res=  "I m";

			str = ft_strndup(tmp, 3);
			if (strcmp(str, res)) {
				SET_DIFF(res, str);
				exit(TEST_FAILED);
			}
			free(str);
			exit(TEST_SUCCESS);
			);

}

void            test_ft_strndup(void){

	add_fun_subtest(test_ft_strndup_malloc_null);
	add_fun_subtest(test_ft_strndup_basic);
	add_fun_subtest(test_ft_strndup_basic2);
	add_fun_subtest(test_ft_strndup_zero);
	add_fun_subtest(test_ft_strndup_size);
	add_fun_subtest(test_ft_strndup_last_char);
	add_fun_subtest(test_ft_strndup_electric_memory);
	add_fun_subtest(test_ft_strndup_null);
}

void			test_ft_atof_basic(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with basic input");

	SANDBOX_RAISE(
			const char *	nbr = "0.6588";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_negative(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with negative basic input");

	SANDBOX_RAISE(
			const char *	nbr = "-6.6588";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_random(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with random numbers");

	SANDBOX_RAISE(
			char	buff[0xF0];
			double	d1;
			double	d2;

			srand(clock());
			for (int i = 0; i < 1000; i++) {
				sprintf(buff, "%lf", (double)(clock() * rand() / (rand() * 10)));

				d1 = atof(buff);
				d2 = ft_atof(buff);
				if ((float)d1 != (float)d2) {
					SET_DIFF_DOUBLE(d1, d2);
					exit(TEST_FAILED);
				}
			}
			exit(TEST_SUCCESS);
			);
}

#include <math.h>
void			test_ft_atof_inf_plus(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with inf value");

	SANDBOX_RAISE(
			char	buff[0xF00];
			double	d1;
			double	d2;

			sprintf(buff, "%lf", INFINITY);
			d1 = atof(buff);
			d2 = ft_atof(buff);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_inf_min(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with basic -inf value");

	SANDBOX_RAISE(
			char	buff[0xF00];
			double	d1;
			double	d2;

			sprintf(buff, "%lf", -INFINITY);
			d1 = atof(buff);
			d2 = ft_atof(buff);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_zero(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with 0");

	SANDBOX_RAISE(
			const char *	nbr = "0";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_nan(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with nan value");

	SANDBOX_RAISE(
			char	buff[0xF00];
			double	d1;
			double	d2;

			sprintf(buff, "%lf", NAN);
			d1 = atof(buff);
			d2 = ft_atof(buff);
			if (isnan(d1) == isnan(d2))
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_blanks1(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with blank chars");

	SANDBOX_RAISE(
			char *	nbr = "  \v\r  \t\n\n\f-9.457787";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_blanks2(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with blank chars");

	SANDBOX_RAISE(
			char *	nbr = "  \v\r  \t\n\n\f+42.442422f    ";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_blanks3(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with - inside basic blanks");

	SANDBOX_RAISE(
			char *	nbr = "  \v\r  \t\n\n\f-    \v\f\r\n\n  \t\t656.45776    \r\n";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_KO);
			);
}

#include <float.h>
void			test_ft_atof_max(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with max double value");

	SANDBOX_RAISE(
			char	nbr[0xF000];
			double	d1;
			double	d2;
			sprintf(nbr, "%lf", DBL_MAX);

			d1 = atof(nbr);
			d2 = atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_KO);
			);
	(void)ft_atof;
}

void			test_ft_atof_min(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with min double value");

	SANDBOX_RAISE(
			char	nbr[0xF0];
			double	d1;
			double	d2;
			sprintf(nbr, "%lf", DBL_MIN);

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_KO);
			);
}

void			test_ft_atof_exp1(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with positive exponential value");

	SANDBOX_RAISE(
			const char *	nbr = "6.6545e+8";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_exp2(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("your atof does not work with negative exponential value");

	SANDBOX_RAISE(
			const char *	nbr = "4.42125787e-3";
			double	d1;
			double	d2;

			d1 = atof(nbr);
			d2 = ft_atof(nbr);
			if ((float)d1 == (float)d2)
				exit(TEST_SUCCESS);
			SET_DIFF_DOUBLE(d1, d2);
			exit(TEST_FAILED);
			);
}

void			test_ft_atof_null(void *ptr) {
	typeof(atof)	*ft_atof = ptr;
	SET_EXPLANATION("atof protection");

	SANDBOX_PROT(
			ft_atof(NULL);
			);
}

void			test_ft_atof(void) {
	add_fun_subtest(test_ft_atof_basic);
	add_fun_subtest(test_ft_atof_negative);
	add_fun_subtest(test_ft_atof_zero);
	add_fun_subtest(test_ft_atof_random);
	add_fun_subtest(test_ft_atof_inf_plus);
	add_fun_subtest(test_ft_atof_inf_min);
	add_fun_subtest(test_ft_atof_nan);
	add_fun_subtest(test_ft_atof_blanks1);
	add_fun_subtest(test_ft_atof_blanks2);
	add_fun_subtest(test_ft_atof_blanks3);
	add_fun_subtest(test_ft_atof_max);
	add_fun_subtest(test_ft_atof_min);
	add_fun_subtest(test_ft_atof_exp1);
	add_fun_subtest(test_ft_atof_exp2);
	add_fun_subtest(test_ft_atof_null);
}

void		test_ft_strlcpy_basic(void *ptr) {
	typeof(strlcpy)	*ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy does not works with basic input");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00];
			char	buff2[0xF00];

			memset(buff1, 'A', sizeof(buff1) - 1);
			memset(buff2, 'A', sizeof(buff2) - 1);
			buff1[sizeof(buff1) - 1] = 0;
			buff2[sizeof(buff1) - 1] = 0;

			strlcpy(buff1, str, sizeof(buff1));
			ft_strlcpy(buff2, str, sizeof(buff2));
			if (!memcmp(buff1, buff2, strlen(str) + 1))
				exit(TEST_SUCCESS);
			SET_DIFF(buff1, buff2);
			exit(TEST_FAILED);
			);
}

void		test_ft_strlcpy_return(void *ptr) {
	typeof(strlcpy)	*ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy does not return the good value");

	SANDBOX_RAISE(
			char	*str = STRING_1;
			char	buff1[0xF00];
			char	buff2[0xF00];
			size_t	r1;
			size_t	r2;

			r1 = strlcpy(buff1, str, sizeof(buff1));
			r2 = ft_strlcpy(buff2, str, sizeof(buff2));
			if (r1 == r2)
				exit(TEST_SUCCESS);
			SET_DIFF_INT((int)r1, (int)r2);
			exit(TEST_FAILED);
			);
}

void		test_ft_strlcpy_min(void *ptr) {
	typeof(strlcpy)	*ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy does works whe size < strlen(src)");

	SANDBOX_RAISE(
			char	*str = "hello !";
			char	buff1[0xF00];
			char	buff2[0xF00];
			size_t	r1;
			size_t	r2;

			memset(buff1, 'A', 20);
			memset(buff2, 'A', 20);

			r1 = strlcpy(buff1, str, 2);
			r2 = ft_strlcpy(buff2, str, 2);
			if (r1 == r2 && !memcmp(buff1, buff2, 20))
				exit(TEST_SUCCESS);
			if (r1 != r2) {
				SET_DIFF_INT((int)r1, (int)r2);
			} else {
				SET_DIFF_BYTES(buff1, buff2, 20);
			}
			exit(TEST_FAILED);
			);
}

void		test_ft_strlcpy_zero(void *ptr) {
	typeof(strlcpy)	*ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy does not works with 0-length string");

	SANDBOX_RAISE(
			char	*str = "";
			char	buff1[0xF00];
			char	buff2[0xF00];
			size_t	r1;
			size_t	r2;

			memset(buff1, 'A', 20);
			memset(buff2, 'A', 20);

			r1 = strlcpy(buff1, str, sizeof(buff1));
			r2 = ft_strlcpy(buff2, str, sizeof(buff2));
			if (r1 == r2 && !memcmp(buff1, buff2, 20))
				exit(TEST_SUCCESS);
			if (r1 != r2) {
				SET_DIFF_INT((int)r1, (int)r2);
			} else {
				SET_DIFF_BYTES(buff1, buff2, 20);
			}
			exit(TEST_FAILED);
			);
}

void		test_ft_strlcpy_overflow(void *ptr) {
	typeof(strlcpy)	*ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy overflow the dest");

	SANDBOX_RAISE(
			char	*str = "BBBB";
			char	buff1[0xF00];
			char	buff2[0xF00];

			memset(buff1, 'A', 20);
			memset(buff2, 'A', 20);

			strlcpy(buff1, str, sizeof(buff1));
			ft_strlcpy(buff2, str, sizeof(buff2));
			if (!memcmp(buff1, buff2, 20))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(buff1, buff2, 20);
			exit(TEST_FAILED);
			);
}

void		test_ft_strlcpy_null(void *ptr) {
	typeof(strlcpy)	*ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy doe not segfault when null parameter is sent");

	SANDBOX_PROT(
			ft_strlcpy(NULL, NULL, 10);
			);
}

void		test_ft_strlcpy_zerosize(void *ptr) {
	typeof(strlcpy) *ft_strlcpy = ptr;
	SET_EXPLANATION("your strlcpy copies while destsize is zero, \
or does not return the size of the string it tried to create");

	SANDBOX_RAISE(
			char	*str = "BBBB";
			char	buff1[0xF00];
			char	buff2[0xF00];

			memset(buff1, 'A', 20);
			memset(buff2, 'A', 20);

			if (strlcpy(buff1, str, 0) != ft_strlcpy(buff2, str, 0))
				exit(TEST_FAILED);
			if (!memcmp(buff1, buff2, 20))
				exit(TEST_SUCCESS);
			SET_DIFF_BYTES(buff1, buff2, 20);
			exit(TEST_FAILED);
			);
}

void		test_ft_strlcpy(void) {
	add_fun_subtest(test_ft_strlcpy_basic);
	add_fun_subtest(test_ft_strlcpy_return);
	add_fun_subtest(test_ft_strlcpy_overflow);
	add_fun_subtest(test_ft_strlcpy_min);
	add_fun_subtest(test_ft_strlcpy_zero);
	add_fun_subtest(test_ft_strlcpy_zerosize);
	add_fun_subtest(test_ft_strlcpy_null);
}
