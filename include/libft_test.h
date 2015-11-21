/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_test.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/13 20:23:36 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/22 00:25:26 by bciss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_TEST_H
# define LIBFT_TEST_H

# include <string.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct	s_subtest {
	char	*fun_name;
	void	(*fun_test_ptr)(void *ptr);
}				t_libft_subtest;

typedef struct	s_test {
	char	*fun_name;
	void	(*fun_test_ptr)(void);
}				t_libft_test;

typedef struct	s_errs {
	int		type;
	char	*data;
	char	*code;
}				t_err;

enum		e_values {
	TEST_SUCCESS,
	TEST_FAILED,
	TEST_TIMEOUT,
	TEST_CRASH,
	TEST_INTERUPT,
	TEST_MISSING,
	TEST_NOCRASH
};

# define	LOG_FILE		"result.log"
# define	TMP_FILE		".over_malloc"

# define	COLOR_SUCCESS "\033[38;5;46m"
# define	COLOR_FAILED "\033[38;5;160m"
# define	COLOR_TIMEOUT "\033[38;5;166m"
# define	COLOR_INTERUPT "\033[38;5;93m"
# define	COLOR_CLEAR "\033[0m"

# define	BSIZE			0xF00
# define	BFSIZE			0xF0000
# define	SUBTEST_SIZE	0xF00
# define	TIMEOUT_MILLIS	1200

# define	SET_EXPLICATION(x)	current_explication = x;
# define	SET_TEST_TEXT(x)	current_test = x;
# define	SET_CURRENT_TEST_CODE(x) current_test_code = x;

# define	SANDBOX_STRINGIFY(x)	SET_CURRENT_TEST_CODE(#x)	
# define	SANDBOX(x)			SANDBOX_STRINGIFY(x); sandbox();if (!(g_pid = fork())) {x;exit(TEST_SUCCESS);} if (g_pid > 0) { wait((int*)g_ret); _SANDBOX_RAISE(g_ret[0]); unsandbox(); }
# define	SANDBOX_RAISE(x)	SANDBOX(x); if (SANDBOX_CRASH) ft_raise(TEST_CRASH); else ft_raise(g_ret[1]);
# define	SANDBOX_IRAISE(x)	SANDBOX(x); if (SANDBOX_CRASH) ft_raise(TEST_SUCCESS); else ft_raise(TEST_NOCRASH);
# define	SANDBOX_RESULT		(g_ret[1])
# define	SANDBOX_RETURN		(g_ret[0])
# define	_SANDBOX_RAISE(x)	if (x == SIGKILL) ft_raise(TEST_TIMEOUT); if (x == SIGQUIT) ft_raise(TEST_INTERUPT);
# define	SANDBOX_CRASH		(g_ret[0] == SIGSEGV || g_ret[0] == SIGBUS || g_ret[0] == SIGABRT)

# define	_MALLOC_NULL		'N'
# define	_MALLOC_RESET		'R'
# define	_MALLOC_MEMSET		'M'
# define	_MALLOC_SIZE		'S'

# define	MALLOC_NULL			lseek(g_malloc_fd, 0, SEEK_SET); write(g_malloc_fd, (char[1]){_MALLOC_NULL}, 1);
# define	MALLOC_RESET		lseek(g_malloc_fd, 0, SEEK_SET); write(g_malloc_fd, (char[1]){_MALLOC_RESET}, 1);
# define	MALLOC_MEMSET		lseek(g_malloc_fd, 0, SEEK_SET); write(g_malloc_fd, (char[1]){_MALLOC_MEMSET}, 1);
# define	MALLOC_SIZE			lseek(g_malloc_fd, 0, SEEK_SET); write(g_malloc_fd, (char[1]){_MALLOC_SIZE}, 1);

extern		char			*current_fun_name;
extern		int				current_test_id;
extern		t_libft_test	fun_test_table[];
extern		t_libft_subtest	fun_subtest_table[SUBTEST_SIZE];
extern		int				current_subtest_id;
extern		int				total_subtest;
extern		char			*current_explication;
extern		char			*current_test;
extern		pid_t			g_pid;
extern		char			g_ret[2];
extern		int				g_log_fd;
extern		int				g_malloc_fd;
extern		char			*current_test_code;

/*  Display functions  */
void	display_test_result(int value, char *explications);

/*  Signal handle functions  */
void	ft_sig_user_interupt(int s);
void	ft_sig_crash(int s);
void	ft_sig_timeout(int s);
void	ft_sig_success(int s);
void	ft_sig_missing(int s);
void	ft_sig_failed(int s);

/*  Test functions  */
void			test_ft_memset(void);
void			test_ft_bzero(void);
void			test_ft_memcpy(void);
void			test_ft_memccpy(void);
void			test_ft_memmove(void);
void			test_ft_memchr(void);
void			test_ft_memcmp(void);
void			test_ft_strlen(void);
void			test_ft_strdup(void);
void			test_ft_strcpy(void);
void			test_ft_strncpy(void);
void			test_ft_strcat(void);
void			test_ft_strncat(void);
void			test_ft_strlcat(void);
void			test_ft_strchr(void);
void			test_ft_strrchr(void);
void			test_ft_strstr(void);
void			test_ft_strnstr(void);
void			test_ft_strcmp(void);
void			test_ft_strncmp(void);
void			test_ft_atoi(void);
void			test_ft_isalpha(void);
void			test_ft_isdigit(void);
void			test_ft_isalnum(void);
void			test_ft_isascii(void);
void			test_ft_isprint(void);
void			test_ft_toupper(void);
void			test_ft_tolower(void);

void			test_ft_memalloc(void);
void			test_ft_memdel(void);
void			test_ft_strnew(void);
void			test_ft_strdel(void);
void			test_ft_strclr(void);
void			test_ft_striter(void);
void			test_ft_striteri(void);
void			test_ft_strmap(void);
void			test_ft_strmapi(void);
void			test_ft_strequ(void);
void			test_ft_strnequ(void);
void			test_ft_strsub(void);
void			test_ft_strjoin(void);
void			test_ft_strtrim(void);
void			test_ft_strsplit(void);
void			test_ft_itoa(void);
void			test_ft_putchar(void);
void			test_ft_putstr(void);
void			test_ft_putendl(void);
void			test_ft_putnbr(void);
void			test_ft_putchar_fd(void);
void			test_ft_putstr_fd(void);
void			test_ft_putendl_fd(void);
void			test_ft_putnbr_fd(void);

/*  others:  */
void    		run_subtests(void *h, int start);
void			signals(void);
void			ft_raise(int s);

/*  sanbox:  */
void			sandbox(void);
void			unsandbox(void);

#endif
