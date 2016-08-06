/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/06 16:42:40 by alelievr          #+#    #+#             */
/*   Updated: 2016/08/06 16:58:14 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

#define			add_fun_subbench(x) add_fun_subbench_(x, (char *)# x)
void			add_fun_subbench_(void (*fun)(void *ptr), char *funname) {
	static int		index = 0;

	if (index >= SUBTEST_SIZE)
		return ;
	(void)funname;
	fun_subbench_table[index].fun_name = current_fun_name;
	fun_subbench_table[index].fun_test_ptr = fun;
	fun_subbench_table[index].visible = current_fun_visibility;
	index++;
	total_subtest++;
}

////////////////////////////////
//         ft_memset          //
////////////////////////////////

void			bench_ft_memset_speed(void *ptr) {
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

void            bench_ft_memset(void) {
	add_fun_subbench(bench_ft_memset_speed);
}

////////////////////////////////
//         ft_bzero           //
////////////////////////////////

void			bench_ft_bzero_speed(void *ptr) {
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

void            bench_ft_bzero(void){
	add_fun_subbench(bench_ft_bzero_speed);
}

////////////////////////////////
//         ft_memcpy          //
////////////////////////////////

void			bench_ft_memcpy_speed(void *ptr) {
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
void            bench_ft_memcpy(void){
	add_fun_subbench(bench_ft_memcpy_speed);
}

////////////////////////////////
//         ft_memccpy         //
////////////////////////////////

void			bench_ft_memccpy_speed(void *ptr) {
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

void            bench_ft_memccpy(void){
	add_fun_subbench(bench_ft_memccpy_speed);
}

////////////////////////////////
//         ft_memmove         //
////////////////////////////////

void			bench_ft_memmove_speed(void *ptr) {
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

void            bench_ft_memmove(void){
	add_fun_subbench(bench_ft_memmove_speed);
}

////////////////////////////////
//         ft_memchr          //
////////////////////////////////

void			bench_ft_memchr_speed(void *ptr) {
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

void            bench_ft_memchr(void) {
	add_fun_subbench(bench_ft_memchr_speed);
}

////////////////////////////////
//         ft_memcmp          //
////////////////////////////////

void			bench_ft_memcmp_speed(void *ptr) {
	typeof(memcmp)		*ft_memcmp = ptr;
	SET_EXPLANATION("your memcmp does not cast the memory in unsigned char");

	SANDBOX_SPEED(
			size_t	size = BFSIZE * 16;
			uint8_t	*s1 = malloc(sizeof(uint8_t) * size);
			uint8_t	*s2 = malloc(sizeof(uint8_t) * size);

			memset(s1, 'A', size);
			memset(s2, 'A', size);
			,
			memcmp(s1, s2, size);
			,
			ft_memcmp(s1, s2, size);
			);
}

void            bench_ft_memcmp(void){
	add_fun_subbench(bench_ft_memcmp_speed);
}

////////////////////////////////
//         ft_strlen          //
////////////////////////////////

void			bench_ft_strlen_speed(void *ptr) {
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

void            bench_ft_strlen(void) {
	add_fun_subbench(bench_ft_strlen_speed);
}

////////////////////////////////
//         ft_strdup          //
////////////////////////////////

void			bench_ft_strdup_speed(void *ptr) {
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

void            bench_ft_strdup(void) {
	add_fun_subbench(bench_ft_strdup_speed);
}

////////////////////////////////
//         ft_strcpy          //
////////////////////////////////

void			bench_ft_strcpy_speed(void *ptr) {
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

void            bench_ft_strcpy(void) {
	add_fun_subbench(bench_ft_strcpy_speed);
}

////////////////////////////////
//         ft_strncpy         //
////////////////////////////////

void			bench_ft_strncpy_speed(void *ptr) {
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

void            bench_ft_strncpy(void){
	add_fun_subbench(bench_ft_strncpy_speed);
}

////////////////////////////////
//         ft_strcat          //
////////////////////////////////

void			bench_ft_strcat_speed(void *ptr) {
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

void            bench_ft_strcat(void){
	add_fun_subbench(bench_ft_strcat_speed);
}

////////////////////////////////
//         ft_strncat         //
////////////////////////////////

void			bench_ft_strncat_speed(void *ptr) {
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

void            bench_ft_strncat(void){
	add_fun_subbench(bench_ft_strncat_speed);
}

////////////////////////////////
//         ft_strlcat         //
////////////////////////////////

void			bench_ft_strlcat_speed(void *ptr) {
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

void            bench_ft_strlcat(void){
	add_fun_subbench(bench_ft_strlcat_speed);
}

////////////////////////////////
//         ft_strchr          //
////////////////////////////////

void			bench_ft_strchr_speed(void *ptr) {
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

void            bench_ft_strchr(void){
	add_fun_subbench(bench_ft_strchr_speed);
}

////////////////////////////////
//         ft_strrchr         //
////////////////////////////////

void			bench_ft_strrchr_speed(void *ptr) {
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

void            bench_ft_strrchr(void){
	add_fun_subbench(bench_ft_strrchr_speed);
}

////////////////////////////////
//         ft_strstr          //
////////////////////////////////

void			bench_ft_strstr_speed(void *ptr) {
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

void            bench_ft_strstr(void){
	add_fun_subbench(bench_ft_strstr_speed);
}

////////////////////////////////
//         ft_strnstr         //
////////////////////////////////

void			bench_ft_strnstr_speed(void *ptr) {
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

void            bench_ft_strnstr(void){
	add_fun_subbench(bench_ft_strnstr_speed);
}

////////////////////////////////
//         ft_strcmp          //
////////////////////////////////

void			bench_ft_strcmp_speed(void *ptr) {
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

void            bench_ft_strcmp(void){
	add_fun_subbench(bench_ft_strcmp_speed);
}

////////////////////////////////
//         ft_strncmp         //
////////////////////////////////

void			bench_ft_strncmp_speed(void *ptr) {
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

void            bench_ft_strncmp(void){
	add_fun_subbench(bench_ft_strncmp_speed);
}

////////////////////////////////
//         ft_atoi            //
////////////////////////////////

void			bench_ft_atoi_speed(void *ptr) {
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

void            bench_ft_atoi(void){
	add_fun_subbench(bench_ft_atoi_speed);
}
