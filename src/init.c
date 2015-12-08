/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 17:30:13 by alelievr          #+#    #+#             */
/*   Updated: 2015/12/03 16:07:48 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char			*current_fun_name = "";
int				current_fun_visibility = 0;
int				current_protected = 0;
int				current_test_id = 0;
int				current_subtest_id = 0;
int				total_subtest = 0;
char			*current_test = "";
char			*current_explication = "";
char			*current_test_code = "";
pid_t			g_pid = 0;
int				g_log_fd = 0;
int				g_malloc_fd = 0;
int				g_diff_fd = 0;
char			g_ret[2];
t_libft_subtest	fun_subtest_table[SUBTEST_SIZE];
t_libft_test	fun_test_table[] = {
	{"ft_memset", test_ft_memset, 1},
	{"ft_bzero", test_ft_bzero, 1},
	{"ft_memcpy", test_ft_memcpy, 1},
	{"ft_memccpy", test_ft_memccpy, 1},
	{"ft_memmove", test_ft_memmove, 1},
	{"ft_memchr", test_ft_memchr, 1},
	{"ft_memcmp", test_ft_memcmp, 1},
	{"ft_strlen", test_ft_strlen, 1},
	{"ft_strdup", test_ft_strdup, 1},
	{"ft_strcpy", test_ft_strcpy, 1},
	{"ft_strncpy", test_ft_strncpy, 1},
	{"ft_strcat", test_ft_strcat, 1},
	{"ft_strncat", test_ft_strncat, 1},
	{"ft_strlcat", test_ft_strlcat, 1},
	{"ft_strchr", test_ft_strchr, 1},
	{"ft_strrchr", test_ft_strrchr, 1},
	{"ft_strstr", test_ft_strstr, 1},
	{"ft_strnstr", test_ft_strnstr, 1},
	{"ft_strcmp", test_ft_strcmp, 1},
	{"ft_strncmp", test_ft_strncmp, 1},
	{"ft_atoi", test_ft_atoi, 1},
	{"ft_isalpha", test_ft_isalpha, 1},
	{"ft_isdigit", test_ft_isdigit, 1},
	{"ft_isalnum", test_ft_isalnum, 1},
	{"ft_isascii", test_ft_isascii, 1},
	{"ft_isprint", test_ft_isprint, 1},
	{"ft_toupper", test_ft_toupper, 1},
	{"ft_tolower", test_ft_tolower, 1},

	{"ft_memalloc", test_ft_memalloc, 1},
	{"ft_memdel", test_ft_memdel, 1},
	{"ft_strnew", test_ft_strnew, 1},
	{"ft_strdel", test_ft_strdel, 1},
	{"ft_strclr", test_ft_strclr, 1},
	{"ft_striter", test_ft_striter, 1},
	{"ft_striteri", test_ft_striteri, 1},
	{"ft_strmap", test_ft_strmap, 1},
	{"ft_strmapi", test_ft_strmapi, 1},
	{"ft_strequ", test_ft_strequ, 1},
	{"ft_strnequ", test_ft_strnequ, 1},
	{"ft_strsub", test_ft_strsub, 1},
	{"ft_strjoin", test_ft_strjoin, 1},
	{"ft_strtrim", test_ft_strtrim, 1},
	{"ft_strsplit", test_ft_strsplit, 1},
	{"ft_itoa", test_ft_itoa, 1},
	{"ft_putchar", test_ft_putchar, 1},
	{"ft_putstr", test_ft_putstr, 1},
	{"ft_putendl", test_ft_putendl, 1},
	{"ft_putnbr", test_ft_putnbr, 1},
	{"ft_putchar_fd", test_ft_putchar_fd, 1},
	{"ft_putstr_fd", test_ft_putstr_fd, 1},
	{"ft_putendl_fd", test_ft_putendl_fd, 1},
	{"ft_putnbr_fd", test_ft_putnbr_fd, 1},

	{"ft_lstnew", test_ft_lstnew, 0},
	{"ft_lstdelone", test_ft_lstdelone, 0},
	{"ft_lstdel", test_ft_lstdel, 0},
	{"ft_lstadd", test_ft_lstadd, 0},
	{"ft_lstiter", test_ft_lstiter, 0},
	{"ft_lstmap", test_ft_lstmap, 0},

	{"ft_islower", test_ft_islower, 0},
	{"ft_isupper", test_ft_isupper, 0},
	{"ft_isnumber", test_ft_isnumber, 0},
	{"ft_isblank", test_ft_isblank, 0},
	{"ft_strtrimc", test_ft_strtrimc, 0},
	{"ft_strndup", test_ft_strndup, 0},

	{NULL, NULL, 0}
};
