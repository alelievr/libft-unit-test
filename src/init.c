/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2015/11/17 17:30:13 by alelievr          #+#    #+#             */
/*   Updated: 2019/10/20 11:06:28 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char			*current_fun_name = "";
int				current_fun_visibility = 0;
int				current_protected = 0;
int				current_test_id = 0;
int				current_subtest_id = 0;
int				current_benchtype = 0;
int				current_benchiter = 1;
int				current_part = -1;
int				total_subtest = 0;
char			*current_test = "";
char			*current_explication = "";
char			*current_test_code = "";
pid_t			g_pid = 0;
int				g_log_fd = 0;
int				g_diff_fd = 0;
int				g_ret;
t_tdiff			g_time;
t_map			*g_shared_mem;
char			g_nospeed = 0;
char			g_bench = 0;
char			g_nobenchlog = 0;
char			g_help = 0;
char			*g_versus = NULL;
t_libft_subtest	fun_subtest_table[SUBTEST_SIZE];
t_libft_subbench fun_subbench_table[SUBTEST_SIZE];
t_libft_test	fun_test_table[] = {
	{"ft_memset", test_ft_memset, 1, 1},
	{"ft_bzero", test_ft_bzero, 1, 1},
	{"ft_memcpy", test_ft_memcpy, 1, 1},
	{"ft_memccpy", test_ft_memccpy, 1, 1},
	{"ft_memmove", test_ft_memmove, 1, 1},
	{"ft_memchr", test_ft_memchr, 1, 1},
	{"ft_memcmp", test_ft_memcmp, 1, 1},
	{"ft_strlen", test_ft_strlen, 1, 1},
	{"ft_isalpha", test_ft_isalpha, 1, 1},
	{"ft_isdigit", test_ft_isdigit, 1, 1},
	{"ft_isalnum", test_ft_isalnum, 1, 1},
	{"ft_isascii", test_ft_isascii, 1, 1},
	{"ft_isprint", test_ft_isprint, 1, 1},
	{"ft_toupper", test_ft_toupper, 1, 1},
	{"ft_tolower", test_ft_tolower, 1, 1},
	{"ft_strchr", test_ft_strchr, 1, 1},
	{"ft_strrchr", test_ft_strrchr, 1, 1},
	{"ft_strncmp", test_ft_strncmp, 1, 1},
	{"ft_strlcpy", test_ft_strlcpy, 1, 1},
	{"ft_strlcat", test_ft_strlcat, 1, 1},
	{"ft_strnstr", test_ft_strnstr, 1, 1},
	{"ft_atoi", test_ft_atoi, 1, 1},
	{"ft_calloc", test_ft_calloc, 1, 1},
	{"ft_strdup", test_ft_strdup, 1, 1},

	{"ft_substr", test_ft_substr, 2, 1}, 
	{"ft_strjoin", test_ft_strjoin, 2, 1},
	{"ft_strtrim", test_ft_strtrim, 2, 1},
	{"ft_split", test_ft_split, 2, 1}, 
	{"ft_itoa", test_ft_itoa, 2, 1},
	{"ft_strmapi", test_ft_strmapi, 2, 1},
	{"ft_putchar_fd", test_ft_putchar_fd, 2, 1},
	{"ft_putstr_fd", test_ft_putstr_fd, 2, 1},
	{"ft_putendl_fd", test_ft_putendl_fd, 2, 1},
	{"ft_putnbr_fd", test_ft_putnbr_fd, 2, 1},

	{"ft_lstnew", test_ft_lstnew, 3, 1},
	{"ft_lstadd_front", test_ft_lstadd_front, 3, 1},
	{"ft_lstsize", test_ft_lstsize, 3, 1},
	{"ft_lstlast", test_ft_lstlast, 3, 1},
	{"ft_lstadd_back", test_ft_lstadd_back, 3, 1},
	{"ft_lstdelone", test_ft_lstdelone, 3, 1},
	{"ft_lstclear", test_ft_lstclear, 3, 1},
	{"ft_lstiter", test_ft_lstiter, 3, 1},
	{"ft_lstmap", test_ft_lstmap, 3, 1},

	{"ft_memalloc", test_ft_memalloc, 3, 0},
	{"ft_strcpy", test_ft_strcpy, 3, 0},
	{"ft_strncpy", test_ft_strncpy, 3, 0},
	{"ft_strcat", test_ft_strcat, 3, 0},
	{"ft_strncat", test_ft_strncat, 3, 0},
	{"ft_strstr", test_ft_strstr, 3, 0},
	{"ft_strcmp", test_ft_strcmp, 3, 0},
	{"ft_memdel", test_ft_memdel, 3, 0},
	{"ft_strnew", test_ft_strnew, 3, 0},
	{"ft_strdel", test_ft_strdel, 3, 0},
	{"ft_strclr", test_ft_strclr, 3, 0},
	{"ft_striter", test_ft_striter, 3, 0},
	{"ft_striteri", test_ft_striteri, 3, 0},
	{"ft_strmap", test_ft_strmap, 3, 0},
	{"ft_strequ", test_ft_strequ, 3, 0},
	{"ft_strnequ", test_ft_strnequ, 3, 0},
	{"ft_putchar", test_ft_putchar, 3, 0},
	{"ft_putstr", test_ft_putstr, 3, 0},
	{"ft_putendl", test_ft_putendl, 3, 0},
	{"ft_putnbr", test_ft_putnbr, 3, 0},
	{"ft_islower", test_ft_islower, 3, 0},
	{"ft_isupper", test_ft_isupper, 3, 0},
	{"ft_isnumber", test_ft_isnumber, 3, 0},
	{"ft_isblank", test_ft_isblank, 3, 0},
	{"ft_strtrimc", test_ft_strtrimc, 3, 0},
	{"ft_strndup", test_ft_strndup, 3, 0},
	{"ft_atof", test_ft_atof, 3, 0},

	{NULL, NULL, 4, 0}
};

t_libft_bench	fun_bench_table[] = {
	{"ft_memset", bench_ft_memset, 1},
	{"ft_bzero", bench_ft_bzero, 1},
	{"ft_memcpy", bench_ft_memcpy, 1},
	{"ft_memccpy", bench_ft_memccpy, 1},
	{"ft_memmove", bench_ft_memmove, 1},
	{"ft_memchr", bench_ft_memchr, 1},
	{"ft_memcmp", bench_ft_memcmp, 1},
	{"ft_strlen", bench_ft_strlen, 1},
	{"ft_strchr", bench_ft_strchr, 1},
	{"ft_strrchr", bench_ft_strrchr, 1},
	{"ft_strncmp", bench_ft_strncmp, 1},
	{"ft_strlcat", bench_ft_strlcat, 1},
	{"ft_strnstr", bench_ft_strnstr, 1},
	{"ft_atoi", bench_ft_atoi, 1},
	{"ft_strdup", bench_ft_strdup, 1},

	{"ft_strncpy", bench_ft_strncpy, 0},
	{"ft_strcmp", bench_ft_strcmp, 0},
	{"ft_strcpy", bench_ft_strcpy, 0},
	{"ft_strcat", bench_ft_strcat, 0},
	{"ft_strncat", bench_ft_strncat, 0},
	{"ft_strstr", bench_ft_strstr, 0},

	{NULL, NULL, 0}
};
