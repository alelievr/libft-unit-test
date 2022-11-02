/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_test.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2015/11/13 20:23:36 by alelievr          #+#    #+#             */
/*   Updated: 2022/10/28 13:10:45 by ladloff          ###   ########.fr       */
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
# include <stdint.h>
# include <limits.h>
# include <time.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>

# ifdef linux
#  pragma GCC diagnostic ignored "-Wunused-value"
#  include <bsd/string.h>
# endif

typedef struct	s_subtest {
	char	*fun_name;
	void	(*fun_test_ptr)(void *ptr);
	int		part;
	_Bool	visible;
}				t_libft_subtest;

typedef struct	s_subbench {
	char	*fun_name;
	void	(*fun_bench_ptr)(void* ptr1, void *ptr2);
	_Bool	visible;
}				t_libft_subbench;

typedef struct	s_test {
	char	*fun_name;
	void	(*fun_test_ptr)(void);
	int		part;
	_Bool	visible;
}				t_libft_test;

typedef struct	s_bench {
	char	*fun_name;
	void	(*fun_test_ptr)(void);
	_Bool	visible;
}				t_libft_bench;

typedef struct	s_errs {
	int		type;
	char	*data;
	char	*code;
	char	*diff;
}				t_err;

typedef struct	s_tests {
	char					data[128];
	void					*ptr;
	unsigned long long int	olol;
	int						flags;
}				t_test;

typedef struct	s_list {
	void			*content;
	struct s_list	*next;
}				t_list;

typedef struct	s_tdiff {
	uint64_t	begin;
	uint64_t	mid;
	uint64_t	end;
	char		state;
}				t_tdiff;

typedef struct	s_option
{
	char	*long_name;
	char	short_name;
	void	*arg;
}				t_option;

#ifdef __x86_64__
typedef struct	s_shared_mem_map
{
	char		alloc_byte_1;
	char		alloc_byte_2;
	long		:48;
	size_t		alloc_size;
	size_t		time_begin;
	size_t		time_mid;
	size_t		time_end;
}				t_map;
#else
typedef struct	s_shared_mem_map
{
	char		alloc_byte_1;
	char		alloc_byte_2;
	long		:16;
	size_t		alloc_size;
	size_t		time_begin;
	size_t		time_mid;
	size_t		time_end;
}				t_map;
#endif

enum		e_values {
	TEST_SUCCESS,
	TEST_FAILED,
	TEST_TIMEOUT,
	TEST_CRASH,
	TEST_INTERUPT,
	TEST_MISSING,
	TEST_NOCRASH,
	TEST_KO,
	TEST_PROT,
	TEST_SPEED,
	TEST_INVISIBLE,
	TEST_FINISHED,

	BENCH_FAT,
	BENCH_MEDIUM,
	BENCH_SMALL,
	BENCH_RANDOM
};

enum		e_prot {
	INVISIBLE,
	VISIBLE,
	PROTECTED,
	NOT_PROTECTED
};

# define	unused			__attribute__((unused))

# define	OFF_ALLOC_BYTE1	(g_shared_mem->alloc_byte_1)
# define	OFF_ALLOC_BYTE2	(g_shared_mem->alloc_byte_2)
# define	OFF_ALLOC_SIZE	(g_shared_mem->alloc_size)
# define	OFF_TIME_BEGIN	(g_shared_mem->time_begin)
# define	OFF_TIME_MID	(g_shared_mem->time_mid)
# define	OFF_TIME_END	(g_shared_mem->time_end)

# define	LOG_FILE		"result.log"
# define	DIFF_FILE		".fun_diff"
# define	SHARED_MEM_FILE	".shmem"
# define	BACKTRACE_FILE	"backtrace.crash"
# define	BENCH_LOG_FILE	"/sgoinfre/goinfre/Perso/lftutest/bench.txt"

# define	COLOR_SUCCESS	"\033[38;5;46m"
# define	COLOR_FAILED	"\033[38;5;160m"
# define	COLOR_TIMEOUT	"\033[38;5;166m"
# define	COLOR_INTERUPT	"\033[38;5;93m"
# define	COLOR_KO		"\033[38;5;226m"
# define	COLOR_PART1		"\033[38;5;82m"
# define	COLOR_PART2		"\033[38;5;226m"
# define	COLOR_PART3		"\033[38;5;9m"
# define	COLOR_CLEAR		"\033[0m"
# define	COLOR_PROTECTED	"\033[38;5;250m"
# define	COLOR_NPROTECTED "\033[38;5;203m"
# define	COLOR_INFO		"\033[38;5;10m"
# define	COLOR_SPEED_CRASH "\033[38;5;245m"
# define	COLOR_SPEED_10	"\033[38;5;197m"
# define	COLOR_SPEED_5	"\033[38;5;202m"
# define	COLOR_SPEED_2	"\033[38;5;220m"
# define	COLOR_SPEED_1	"\033[38;5;118m"
# define	COLOR_SPEED_05	"\033[38;5;44m"
# define	COLOR_SPEED_0	"\033[38;5;27m"
# define	COLOR_BENCH_PLAYER "\033[48;5;123m"
# define	COLOR_BENCH_VERSUS "\033[48;5;118m"
# define	COLOR_BENCH_FAT	"\033[38;5;208m"
# define	COLOR_BENCH_MEDIUM "\033[38;5;220m"
# define	COLOR_BENCH_SMALL "\033[38;5;191m"
# define	COLOR_BENCH_WINNER "\033[38;5;201m"
# define	COLOR_WARNING	"\033[38;5;9m"

# define	BSIZE			0xF00
# define	BFSIZE			0xF0000
# define	SUBTEST_SIZE	0xF00
# define	TIMEOUT_MILLIS	1500

# define	SET_EXPLANATION(x)	current_explication = x;
# define	SET_TEST_TEXT(x)	current_test = x;
# define	SET_CURRENT_TEST_CODE(x) current_test_code = x;
# define	SET_CURRENT_PROTECTED(x) current_protected = x;
# define	SET_BENCHTYPE(x)	current_benchtype = x;
# define	SET_BENCHITER(x)	current_benchiter = x;

# define	SANDBOX_STRINGIFY(x)	SET_CURRENT_TEST_CODE(#x)
# define	SANDBOX(x)			SANDBOX_STRINGIFY(x); sandbox();if (!(g_pid = fork())) {x;exit(TEST_SUCCESS);} if (g_pid > 0) { wait(&g_ret); if (WIFSIGNALED(g_ret)) _SANDBOX_RAISE(WTERMSIG(g_ret)); unsandbox(); }
# define	SANDBOX_KO(x)		SANDBOX(x); if (SANDBOX_CRASH) ft_raise(TEST_SUCCESS); else if (SANDBOX_RETURN != SIGKILL) ft_raise(TEST_KO);
# define	SANDBOX_RAISE(x)	SANDBOX(x); if (SANDBOX_CRASH) ft_raise(TEST_CRASH); else if (SANDBOX_RETURN != SIGKILL) ft_raise(SANDBOX_RESULT);
# define	SANDBOX_IRAISE(x)	SANDBOX(x); if (SANDBOX_CRASH) ft_raise(TEST_SUCCESS); else if (SANDBOX_RETURN != SIGKILL) ft_raise(TEST_NOCRASH);
# define	SANDBOX_PROT(x);	SANDBOX(x); if (SANDBOX_CRASH) current_protected = NOT_PROTECTED; else current_protected = PROTECTED; ft_raise(TEST_PROT);
//			SANDBOX_SPEED(initialization state, system function, libft function)
# define	SANDBOX_SPEED(x,y,z)SANDBOX(x; TIME_BEGIN; y; TIME_MID; z; TIME_END; TIME_SET_DATA;) if (SANDBOX_CRASH) g_time.state = TEST_CRASH; else g_time.state = VISIBLE; TIME_READ_DATA; ft_raise(TEST_SPEED);
//			SANDBOX_SPEED(initialization state, system function, libft function) but use the bench type variable to raise display
# define	SANDBOX_BENCH(x,y,z)SANDBOX(x; TIME_BEGIN; for (register int _i = 0; _i < current_benchiter; _i++) { y; } TIME_MID; for (register int _i = 0; _i < current_benchiter; _i++) { z; } TIME_END; TIME_SET_DATA;) if (SANDBOX_CRASH) g_time.state = TEST_CRASH; else g_time.state = VISIBLE; TIME_READ_DATA; ft_raise(current_benchtype);
# define	SANDBOX_RESULT		(WEXITSTATUS(g_ret))
# define	SANDBOX_RETURN		(WTERMSIG(g_ret))
# define	_SANDBOX_RAISE(x)	if (x == SIGKILL) ft_raise(TEST_TIMEOUT); if (x == SIGQUIT) ft_raise(TEST_INTERUPT);
# define	SANDBOX_CRASH		(SANDBOX_RETURN == SIGSEGV || SANDBOX_RETURN == SIGBUS || SANDBOX_RETURN == SIGABRT)

# define	_MALLOC_ENABLE		'e'
# define	_MALLOC_DISABLE		'd'
# define	_MALLOC_NULL		'N'
# define	_MALLOC_RESET		'R'
# define	_MALLOC_MEMSET		'M'
# define	_MALLOC_SIZE		'S'
# define	_MALLOC_DEBUG		'D'

# define	MALLOC_SET_BYTE(x)	OFF_ALLOC_BYTE1 = x; OFF_ALLOC_BYTE2 = _MALLOC_DISABLE;
# define	MALLOC_NULL			MALLOC_SET_BYTE(_MALLOC_NULL)
# define	MALLOC_RESET		MALLOC_SET_BYTE(_MALLOC_RESET)
# define	MALLOC_MEMSET		MALLOC_SET_BYTE(_MALLOC_MEMSET)
# define	MALLOC_DEBUG		MALLOC_SET_BYTE(_MALLOC_DEBUG)
# define	MALLOC_SIZE			MALLOC_SET_BYTE(_MALLOC_SIZE)

//# define	PRINT_BITS(x, y)	print_double_bits(x, y);
# define	SET_DIFF(x, y)		lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |%.300s|\n%12s: |%.300s|", current_fun_name + 3, (char*)x, current_fun_name, (char*)y); write(g_diff_fd, "\0", 1);
# define	SET_DIFF_INT(x, y)	lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |%i|\n%12s: |%i|", current_fun_name + 3, x, current_fun_name, y) ; write(g_diff_fd, "\0", 1);
# define	SET_DIFF_DOUBLE(x, y)	lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |%lf|\n%12s: |%lf|", current_fun_name + 3, x, current_fun_name, y) ; write(g_diff_fd, "\0", 1);
# define	SET_DIFF_PTR(x, y)	lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |%p|\n%12s: |%p|", current_fun_name + 3, x, current_fun_name, y) ; write(g_diff_fd, "\0", 1);
//# define	SET_DIFF_BIT(x, y, z)	lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |", current_fun_name + 3); PRINT_BITS(x, z); dprintf("|\n%12s: |", current_fun_name); PRINT_BITS(y, z); dprintf(g_diff_fd, "|");
# define	SET_DIFF_BYTES(x, y, max)	lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |", current_fun_name + 3); for (register int __i = 0; __i < max; ++__i) dprintf(g_diff_fd, "\\x%-2hhx ", x[__i]); dprintf(g_diff_fd, "|\n%12s: |", current_fun_name); for (register int __i = 0; __i < max; ++__i) dprintf(g_diff_fd, "\\x%-2hhx ", y[__i]); dprintf(g_diff_fd, "|");
# define	SET_DIFF_CUSTOM(f, args...) lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, f, ##args) ; write(g_diff_fd, "\0", 1);
# define	PRINT_DIFF_ASCII(x, z)	for (size_t it = 0; it < z; it++) dprintf(g_diff_fd, "\\x%02x", x[it]);
# define	SET_DIFF_ASCII(x, y, z)	lseek(g_diff_fd, 0, SEEK_SET); dprintf(g_diff_fd, "%12s: |", current_fun_name + 3); PRINT_DIFF_ASCII(x, z); dprintf(g_diff_fd, "|\n%12s: |", current_fun_name); PRINT_DIFF_ASCII(y, z); dprintf(g_diff_fd, "|");
# define	RESET_DIFF			lseek(g_diff_fd, 0, SEEK_SET); write(g_diff_fd, "\0", 1);

# define	TIME_BEGIN			g_time.begin = ft_clock(); g_time.state = VISIBLE;// printf("begin = %llu\n", g_time.begin);
# define	TIME_MID			g_time.mid = ft_clock();// printf("mid = %llu\n", g_time.mid);
# define	TIME_END			g_time.end = ft_clock();// printf("end = %llu\n", g_time.end);
# define	TIME_DIFF_SYS		(g_time.end - g_time.mid)
# define	TIME_DIFF_LIB		(g_time.mid - g_time.begin)
# define	TIME_SPEEDRATE		(((float)TIME_DIFF_SYS / (float)TIME_DIFF_LIB))
# define	TIME_SET_DATA		OFF_TIME_BEGIN = g_time.begin; OFF_TIME_MID = g_time.mid; OFF_TIME_END = g_time.end;
# define	TIME_READ_DATA		g_time.begin = OFF_TIME_BEGIN; g_time.mid = OFF_TIME_MID; g_time.end =  OFF_TIME_END;

# define	STDOUT_TO_BUFF		fd_to_buffer(STDOUT_FILENO);
# define	STDERR_TO_BUFF		fd_to_buffer(STDERR_FILENO);
# define	GET_STDOUT(y, z)	write(STDOUT_FILENO, "", 1); get_fd_buffer(STDOUT_FILENO, y, z);
# define	GET_STDERR(y, z)	write(STDERR_FILENO, "", 1); get_fd_buffer(STDERR_FILENO, y, z);
# define	VOID_STDOUT			write(STDOUT_FILENO, "", 1); get_fd_buffer(STDOUT_FILENO, NULL, 0);
# define	VOID_STDERR			write(STDERR_FILENO, "", 1); get_fd_buffer(STDERR_FILENO, NULL, 0);

extern		char			*current_fun_name;
extern		int				current_fun_visibility;
extern		int				current_test_id;
extern		t_libft_test	fun_test_table[];
extern		t_libft_bench	fun_bench_table[];
extern		t_libft_subtest	fun_subtest_table[SUBTEST_SIZE];
extern		t_libft_subbench fun_subbench_table[SUBTEST_SIZE];
extern		int				current_subtest_id;
extern		int				total_subtest;
extern		char			*current_explication;
extern		char			*current_test;
extern		pid_t			g_pid;
extern		int				g_ret;
extern		int				g_log_fd;
extern		int				g_diff_fd;
extern		t_map			*g_shared_mem;
extern		char			*current_test_code;
extern		int				current_protected;
extern		int				current_benchtype;
extern		int				current_benchiter;
extern		int				current_part;
extern		char			g_nospeed;
extern		char			g_bench;
extern		char			g_nobenchlog;
extern		char			*g_versus;
extern		t_tdiff			g_time;

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
void			test_ft_memmove(void);
void			test_ft_memchr(void);
void			test_ft_memcmp(void);
void			test_ft_strlen(void);
void			test_ft_strdup(void);
void			test_ft_calloc(void);
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
void			test_ft_substr(void);
void			test_ft_strjoin(void);
void			test_ft_strtrim(void);
void			test_ft_split(void);
void			test_ft_itoa(void);
void			test_ft_putchar(void);
void			test_ft_putstr(void);
void			test_ft_putendl(void);
void			test_ft_putnbr(void);
void			test_ft_putchar_fd(void);
void			test_ft_putstr_fd(void);
void			test_ft_putendl_fd(void);
void			test_ft_putnbr_fd(void);

void            test_ft_lstnew(void);
void            test_ft_lstdelone(void);
void            test_ft_lstclear(void);
void            test_ft_lstadd_front(void);
void            test_ft_lstadd_back(void);
void            test_ft_lstiter(void);
void            test_ft_lstsize(void);
void            test_ft_lstlast(void);
void            test_ft_lstmap(void);

void			test_ft_islower(void);
void			test_ft_isupper(void);
void			test_ft_isnumber(void);
void			test_ft_isblank(void);
void			test_ft_strtrimc(void);
void			test_ft_strndup(void);
void			test_ft_atof(void);
void			test_ft_strlcpy(void);

/*	bench functions:  */
void			bench_ft_memset(void);
void			bench_ft_bzero(void);
void			bench_ft_memcpy(void);
void			bench_ft_memmove(void);
void			bench_ft_memchr(void);
void			bench_ft_memcmp(void);
void			bench_ft_strlen(void);
void			bench_ft_strdup(void);
void			bench_ft_strcpy(void);
void			bench_ft_strncpy(void);
void			bench_ft_strcat(void);
void			bench_ft_strncat(void);
void			bench_ft_strlcat(void);
void			bench_ft_strchr(void);
void			bench_ft_strrchr(void);
void			bench_ft_strstr(void);
void			bench_ft_strnstr(void);
void			bench_ft_strcmp(void);
void			bench_ft_strncmp(void);
void			bench_ft_atoi(void);

/*  others:  */
void    		run_subtests(void *h, int start);
void			signals(void);
void			ft_raise(int s);
void			fd_to_buffer(int fd);
char			*get_fd_buffer(int fd, char *buff, size_t size);
int				get_last_malloc_size(void);
unsigned long long	ft_clock(void);
void			*electric_alloc(size_t size);
void			*electric_alloc_rev(size_t size);
int				hardcore_main(void *libft_so_handler);

/*  sanbox:  */
void			sandbox(void);
void			unsandbox(void);

/*	System's replacement functions:  */
int				simple_memcmp (const char *s1, const char *s2, size_t n);
void			putwchart(int wchar, int *len, char *buff);
int				ft_putwstr(wchar_t *wstr, char *buff);

#endif
