/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 17:42:18 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/22 00:33:25 by bciss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

void			add_fun_subtest(void (*fun)(void *ptr)) {
	static int		index = 0;

	if (index >= SUBTEST_SIZE)
		return ;
	fun_subtest_table[index].fun_name = current_fun_name;
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

	SANDBOX_IRAISE(
			ft_memset(NULL, 'a', 12);
		   );
}

void			test_ft_memset_zero_value(void *ptr) {
	typeof(memset)	*ft_memset = ptr;
	SET_EXPLICATION("your memset change something when call with a size of 0 !");

	SANDBOX_RAISE(
			char	buff[BSIZE] = {[BSIZE - 1]=0};
			char	buff2[BSIZE] = {[BSIZE - 1]=0};

			ft_memset(buff, '\xff', 0);
			memset(buff2, '\xff', 0);
			if (memcmp(buff, buff2, BSIZE))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
		   );
}

void            test_ft_memset(void) {
	add_fun_subtest(test_ft_memset_basic);
	add_fun_subtest(test_ft_memset_null);
	add_fun_subtest(test_ft_memset_zero_value);
	add_fun_subtest(test_ft_memset_fat);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void			test_ft_bzero_basic(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;

	SET_EXPLICATION("bzero with normal params fail !");

	SANDBOX_RAISE(
			char	str[BSIZE];
			char	str2[BSIZE];

			memset(str, 'a', BSIZE);
			memset(str2, 'a', BSIZE);

			bzero(str, 60);
			ft_bzero(str2, 60);
			if (memcmp(str, str2, BSIZE))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);
}

void			test_ft_bzero_zero_value(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;

	SET_EXPLICATION("your bzero change something when call with 0 !");

	SANDBOX_RAISE(
			char	buff[BSIZE] = {[BSIZE - 1]=0};
			char	buff2[BSIZE] = {[BSIZE - 1]=0};

			ft_bzero(buff, 0);
			bzero(buff2, 0);
			if (memcmp(buff, buff2, BSIZE))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
		   );
}

void			test_ft_bzero_null(void *ptr) {
	typeof(bzero)	*ft_bzero = ptr;
	SET_EXPLICATION("your bzero does not segfault when null params is sent");

	SANDBOX_IRAISE(
			ft_bzero(NULL, 0x12);
			);
}


void            test_ft_bzero(void){
	add_fun_subtest(test_ft_bzero_basic);
	add_fun_subtest(test_ft_bzero_zero_value);
	add_fun_subtest(test_ft_bzero_null);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////


void			test_ft_memcpy_basic_test(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy doesn't work with basic params");

	SANDBOX_RAISE(
			char	buff[] = "test basic du memcpy !";
			char	buff2[22];

			ft_memcpy(buff2, buff, 22);
			if (memcmp(buff, buff2, 22))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_zero_value(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not work when call with 0");

	SANDBOX_RAISE(
			char	buff[] = "test 0 du memcpy !";
			char	buff2[] = "phrase differente pour le test";

			ft_memcpy(buff2, buff, 0);
			if (memcmp("phrase differente pour le test", buff2, strlen(buff2)))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_basic_test2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not work with basic params");

	SANDBOX_RAISE(
			char	buff[] = "test basic numero 2";
			char	buff2[] = "phrase differente pour le test";

			ft_memcpy(buff2, buff, strlen(buff));
			if (memcmp("test basic numero 2our le test", buff2, strlen(buff2)))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_to_small(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not segfault dst is not big enough");

	SANDBOX_IRAISE(
			ft_memcpy("", "segfaulter tu dois", 17);
			);
}

void			test_ft_memcpy_null1(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not segv with NULL on first params");

	SANDBOX_IRAISE(
			ft_memcpy(NULL, "segfaulter tu dois", 17);
			);
}

void			test_ft_memcpy_null2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not segv with NULL on second params");

	SANDBOX_IRAISE(
			ft_memcpy("            ", NULL, 17);
			);
}

void            test_ft_memcpy(void){
	add_fun_subtest(test_ft_memcpy_basic_test);
	add_fun_subtest(test_ft_memcpy_zero_value);
	add_fun_subtest(test_ft_memcpy_basic_test2);
	add_fun_subtest(test_ft_memcpy_to_small);
	add_fun_subtest(test_ft_memcpy_null1);
	add_fun_subtest(test_ft_memcpy_null2);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

/*void			test_ft_memcpy_basic_test(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy doesn't work with basic params");

	SANDBOX_RAISE(
			char	buff[] = "test basic du memcpy !";
			char	buff2[22];

			ft_memcpy(buff2, buff, 22);
			if (memcmp(buff, buff2, 22))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_zero_value(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not work when call with 0");

	SANDBOX_RAISE(
			char	buff[] = "test 0 du memcpy !";
			char	buff2[] = "phrase differente pour le test";

			ft_memcpy(buff2, buff, 0);
			if (memcmp("phrase differente pour le test", buff2, strlen(buff2)))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_basic_test2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not work with basic params");

	SANDBOX_RAISE(
			char	buff[] = "test basic numero 2";
			char	buff2[] = "phrase differente pour le test";

			ft_memcpy(buff2, buff, strlen(buff));
			if (memcmp("test basic numero 2our le test", buff2, strlen(buff2)))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_memcpy_to_small(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not segfault dst is not big enough");

	SANDBOX_IRAISE(
			ft_memcpy("", "segfaulter tu dois", 17);
			);
}

void			test_ft_memcpy_null1(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not segv with NULL on first params");

	SANDBOX_IRAISE(
			ft_memcpy(NULL, "segfaulter tu dois", 17);
			);
}

void			test_ft_memcpy_null2(void *ptr) {
	typeof(memcpy)	*ft_memcpy = ptr;
	SET_EXPLICATION("your memcpy does not segv with NULL on second params");

	SANDBOX_IRAISE(
			ft_memcpy("            ", NULL, 17);
			);
}
*/
void            test_ft_memccpy(void){
/*	add_fun_subtest(test_ft_memccpy_basic_test);
	add_fun_subtest(test_ft_memccpy_zero_value);
	add_fun_subtest(test_ft_memccpy_basic_test2);
	add_fun_subtest(test_ft_memccpy_to_small);
	add_fun_subtest(test_ft_memccpy_null1);
	add_fun_subtest(test_ft_memccpy_null2);
*/}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_memmove(void){}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_memchr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_memcmp(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void			test_ft_strlen_zero(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLICATION("your strlen doesn't work with a \\0 inside the string");

	SANDBOX_RAISE(

			if (ft_strlen("HAHAHAHA \0 TA FAIL XD") != 9)
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strlen_empty(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLICATION("your strlen doesn't work with an empty string");

	SANDBOX_RAISE(

			if (ft_strlen(""))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void			test_ft_strlen_null(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLICATION("your strlen does not segv when null is sended");

	SANDBOX_IRAISE(
			ft_strlen(NULL);
			);
}

void			test_ft_strlen_basic(void *ptr) {
	typeof(strlen)	*ft_strlen = ptr;
	SET_EXPLICATION("your strlen doesn't work with basic test");

	SANDBOX_RAISE(

			if (ft_strlen("sais-tu compter ?") != strlen("parceque lui oui!"))
				exit(TEST_FAILED);
			exit(TEST_SUCCESS);
			);

}

void            test_ft_strlen(void){

	add_fun_subtest(test_ft_strlen_basic);
	add_fun_subtest(test_ft_strlen_null);
	add_fun_subtest(test_ft_strlen_empty);
	add_fun_subtest(test_ft_strlen_zero);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void			test_ft_strdup_last_char(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLICATION("your strdup does not add \\0 at the end of the sring");

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
	SET_EXPLICATION("your strdup don't work with empty string");

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

void			test_ft_strdup_null(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLICATION("your strdup does not segv with NULL");

	SANDBOX_IRAISE(
			ft_strdup(NULL);
			);
}

void			test_ft_strdup_malloc_null(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLICATION("you dindn't protect your malloc return");

//	SANDBOX_RAISE(
//			char	*str;
//			char	*tmp = "I malloc so I am.";
			
			lseek(g_malloc_fd, 0, SEEK_SET);
			write(g_malloc_fd, (char[1]){_MALLOC_NULL}, 1);

			printf("malloc = %p\n", malloc(10));
			fflush(stdout);
//			str = strdup(tmp);
			MALLOC_RESET;
//			if (str == NULL)
//				exit(TEST_SUCCESS);
//			exit(TEST_FAILED);
//			);
	(void)ft_strdup;
}

void			test_ft_strdup_basic(void *ptr) {
	typeof(strdup)	*ft_strdup = ptr;
	SET_EXPLICATION("your strdup doesn't with basic input");

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

void            test_ft_strdup(void){

	add_fun_subtest(test_ft_strdup_malloc_null);
	add_fun_subtest(test_ft_strdup_basic);
	add_fun_subtest(test_ft_strdup_zero);
	add_fun_subtest(test_ft_strdup_last_char);
	add_fun_subtest(test_ft_strdup_null);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strcpy(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strncpy(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strcat(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strncat(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strlcat(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strchr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strrchr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strstr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strnstr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strcmp(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strncmp(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_atoi(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

#include <ctype.h>

void			test_ft_isalpha_(void *ptr) {
	typeof(isalpha)	*ft_isalpha = ptr;
	SET_EXPLICATION("your ft_isalpha just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 130)
			{
				if (ft_isalpha(i) != isalpha(i))
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
//         ft_bzero           //
////////////////////////////////

void			test_ft_isdigit_(void *ptr) {
	typeof(isdigit)	*ft_isdigit = ptr;
	SET_EXPLICATION("your ft_isdigit just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 130)
			{
				if (ft_isdigit(i) != isdigit(i))
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
//         ft_bzero           //
////////////////////////////////

void			test_ft_isalnum_(void *ptr) {
	typeof(isalnum)	*ft_isalnum = ptr;
	SET_EXPLICATION("your ft_isalnum just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 130)
			{
				if (ft_isalnum(i) != isalnum(i))
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
//         ft_bzero           //
////////////////////////////////

void			test_ft_isascii_(void *ptr) {
	typeof(isascii)	*ft_isascii = ptr;
	SET_EXPLICATION("your ft_isascii just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 130)
			{
				if (ft_isascii(i) != isascii(i))
					exit(TEST_FAILED);
				i++;
			}
			exit(TEST_SUCCESS);
		);
}

void            test_ft_isascii(void){
	add_fun_subtest(test_ft_isascii_);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void			test_ft_isprint_(void *ptr) {
	typeof(isprint)	*ft_isprint = ptr;
	SET_EXPLICATION("your ft_isprint just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 130)
			{
				if (ft_isprint(i) != isprint(i))
					exit(TEST_FAILED);
				i++;
			}
			exit(TEST_SUCCESS);
		);
}


void            test_ft_isprint(void){
	add_fun_subtest(test_ft_isprint_);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

/*void			test_ft_toupper_(void *ptr) {
	typeof(toupper)	*ft_toupper = ptr;
	SET_EXPLICATION("your ft_isprint just doesn't work, REALLY ?!");

	SANDBOX_RAISE(
			int		i;
			i = -1;
			while (i < 130)
			{
				if (ft_isprint(i) != isprint(i))
					exit(TEST_FAILED);
				i++;
			}
			exit(TEST_SUCCESS);
		);
}*/

void            test_ft_toupper(void){
//	add_fun_subtest(test_ft_toupper_);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_tolower(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_memalloc(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_memdel(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strnew(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strdel(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strclr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_striter(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_striteri(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strmap(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strmapi(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strequ(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strnequ(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strsub(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strjoin(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strtrim(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_strsplit(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_itoa(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putchar(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putstr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putendl(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putnbr(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putchar_fd(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putstr_fd(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putendl_fd(void){ }

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void            test_ft_putnbr_fd(void){ }
