/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 17:30:13 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/22 17:55:11 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char			*current_fun_name = "";
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
	{"ft_memset", test_ft_memset},
	{"ft_bzero", test_ft_bzero},
	{"ft_memcpy", test_ft_memcpy},
	{"ft_memccpy", test_ft_memccpy},
	{"ft_memmove", test_ft_memmove},
	{"ft_memchr", test_ft_memchr},
	{"ft_memcmp", test_ft_memcmp},
	{"ft_strlen", test_ft_strlen},
	{"ft_strdup", test_ft_strdup},
	{"ft_strcpy", test_ft_strcpy},
	{"ft_strncpy", test_ft_strncpy},
	{"ft_strcat", test_ft_strcat},
	{"ft_strncat", test_ft_strncat},
	{"ft_strlcat", test_ft_strlcat},
	{"ft_strchr", test_ft_strchr},
	{"ft_strrchr", test_ft_strrchr},
	{"ft_strstr", test_ft_strstr},
	{"ft_strnstr", test_ft_strnstr},
	{"ft_strcmp", test_ft_strcmp},
	{"ft_strncmp", test_ft_strncmp},
	{"ft_atoi", test_ft_atoi},
	{"ft_isalpha", test_ft_isalpha},
	{"ft_isdigit", test_ft_isdigit},
	{"ft_isalnum", test_ft_isalnum},
	{"ft_isascii", test_ft_isascii},
	{"ft_isprint", test_ft_isprint},
	{"ft_toupper", test_ft_toupper},
	{"ft_tolower", test_ft_tolower},

	{"ft_memalloc", test_ft_memalloc},
	{"ft_memdel", test_ft_memdel},
	{"ft_strnew", test_ft_strnew},
	{"ft_strdel", test_ft_strdel},
	{"ft_strclr", test_ft_strclr},
	{"ft_striter", test_ft_striter},
	{"ft_striteri", test_ft_striteri},
	{"ft_strmap", test_ft_strmap},
	{"ft_strmapi", test_ft_strmapi},
	{"ft_strequ", test_ft_strequ},
	{"ft_strnequ", test_ft_strnequ},
	{"ft_strsub", test_ft_strsub},
	{"ft_strjoin", test_ft_strjoin},
	{"ft_strtrim", test_ft_strtrim},
	{"ft_strsplit", test_ft_strsplit},
	{"ft_itoa", test_ft_itoa},
	{"ft_putchar", test_ft_putchar},
	{"ft_putstr", test_ft_putstr},
	{"ft_putendl", test_ft_putendl},
	{"ft_putnbr", test_ft_putnbr},
	{"ft_putchar_fd", test_ft_putchar_fd},
	{"ft_putstr_fd", test_ft_putstr_fd},
	{"ft_putendl_fd", test_ft_putendl_fd},
	{"ft_putnbr_fd", test_ft_putnbr_fd},
	{NULL, NULL}
};
