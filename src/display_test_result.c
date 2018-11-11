/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_test_result.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2015/11/13 20:26:54 by alelievr          #+#    #+#             */
/*   Updated  2015/12/23 20:28:34 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

const char		*forbidden_functions[] = {
	"memset",
	"bzero",
	"memcpy",
	"memchr",
	"memcmp",
	"strlen",
	"strdup",
	"strcpy",
	"strncpy",
	"strcat",
	"strncat",
	"strlcat",
	"strchr",
	"strrchr",
	"strstr",
	"strnstr",
	"strcmp",
	"strncmp",
	"atoi",
	NULL
};

#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#define SKIP_SPACE(x) while (isspace(**x)) (*x)++;
#define SKIP_SEMICOLON(x) while (**x == ';') (*x)++;
#define	CODELINE_BUFFSIZE 0xF0000

bool		single_bracket_on_line(char *line, int n)
{
	while (n--)
		if (!isspace(line[n]) && (line[n] != '}' || line[n] != '{'))
			return false;
	return true;
}

char		*get_next_code_line(char **code, bool reset)
{
	static char		ret[CODELINE_BUFFSIZE + 1];
	static size_t	indent = 1;
	static bool		next_line_indent = false;
	bool			new_block_statement = false;
	bool			set_now = false;
	size_t			parent = 0;
	size_t			i = 0;
	size_t			shift = 0;

	if (reset)
	{
		indent = 1;
		return NULL;
	}

	if (next_line_indent)
		indent++;
	while (i < indent && i < CODELINE_BUFFSIZE)
		ret[i++] = '\t';
	while (**code)
	{
		if (**code == '(')
			parent++;
		if (**code == ')')
		{
			parent--;
			if (parent == 0 && new_block_statement)
			{
				set_now = true;
				if (i >= CODELINE_BUFFSIZE)
					goto end;
				ret[i++] = *(*code)++;
				SKIP_SPACE(code);
				next_line_indent = true;
				break ;
			}
		}
		if (**code == '{')
		{
			indent++;
			if (single_bracket_on_line(ret, i))
			{
				if (next_line_indent)
					shift = 1;
				if (i >= CODELINE_BUFFSIZE)
					goto end;
				ret[i++] = *(*code)++;
				SKIP_SPACE(code);
				break ;
			}
		}
		if (**code == '}')
		{
			indent--;
			if (single_bracket_on_line(ret, i))
			{
				shift = 1;
				if (i >= CODELINE_BUFFSIZE)
					goto end;
				ret[i++] = *(*code)++;
				SKIP_SPACE(code);
				break ;
			}
		}
		if (!strncmp(*code, "if ", 3) || !strncmp(*code, "while ", 6) || !strncmp(*code, "for ", 4))
			new_block_statement = true;
		if (i >= CODELINE_BUFFSIZE)
			goto end;
		ret[i++] = **code;
		if (**code == ';' && parent == 0)
		{
			SKIP_SEMICOLON(code);
			SKIP_SPACE(code);
			break ;
		}
		(*code)++;
	}
	end:
	ret[i] = 0;
	if (next_line_indent && set_now == false)
	{
		indent--;
		next_line_indent = false;
	}
	return ret + shift;
}

static bool	is_var_declaration(const char *code_line)
{
	const char	* const *identifiers = (const char * const *)(const char * const []){
		"const",
		"int",
		"char",
		"wchar_t",
		"float",
		"double",
		"size_t",
		"t_list",
		NULL};

	while (*identifiers)
	{
		while (isspace(*code_line))
			code_line++;
		if (!strncmp(code_line, *identifiers, strlen(*identifiers)))
			return true;
		identifiers++;
	}
	return false;
}

static bool	contains_forbidden_content(const char *code_line)
{
	const char	* const *identifiers = (const char * const *)(const char * const []){
		"write(g_diff_fd, \"\\0\", 1)",
		"dprintf",
		"lseek",
		"g_shared_mem",
		"mprotect",
		NULL};

	while (*identifiers)
	{
		if (strstr(code_line, *identifiers))
			return true;
		identifiers++;
	}
	return false;
}

static char	*butify(char *code) {
	const size_t	size = (strlen(code) + 0xF00);
	char			*tmp;
	char			*ret;
	char			*code_line;
	bool			decl_blank = true;
	bool			first = true;

	if (!(tmp = (char*)malloc(sizeof(char) * size)))
		return ("//BUFFER ERROR\\\\");
	ret = tmp;
	*tmp = 0;
	get_next_code_line(NULL, true);
	while (*code)
	{
		code_line = get_next_code_line(&code, false);
		if (decl_blank == true && !first && !is_var_declaration(code_line))
		{
			decl_blank = false;
			tmp = strcat(tmp, "\n");
		}
		if (!contains_forbidden_content(code_line))
		{
			tmp = strcat(tmp, code_line);
			tmp = strcat(tmp, "\n");
		}
		first = false;
	}
	return (ret);
}

static char	*get_diff(void) {
	char	buff[0xF000];
	lseek(g_diff_fd, 0, SEEK_SET);
	buff[read(g_diff_fd, buff, sizeof(buff))] = 0;
	return (strdup(buff));
}

static char	*verbose_type(int type) {
	switch (type) {
		case TEST_FAILED:
			return "fail";
			break ;
		case TEST_CRASH:
			return "crash";
			break ;
		case TEST_NOCRASH:
			return "no crash";
			break ;
		case TEST_TIMEOUT:
			return "timeout";
			break ;
		case TEST_KO:
			return "KO";
			break ;
		default:
			return "";
			break ;
	}
}

static char	*verbose_color(int type) {
	switch(type) {
		case TEST_KO:
			return (COLOR_KO);
			break ;
		case TEST_TIMEOUT:
			return (COLOR_TIMEOUT);
			break ;
		default:
			return (COLOR_FAILED);
			break ;
	}
}

static void	display_part(void) {
	static int last_part = -2;

	if (last_part != current_part && current_part == 1) {
		if (!g_nospeed)
			printf(COLOR_CLEAR"speed scale: > x10:"COLOR_SPEED_10"\u25CF"COLOR_CLEAR
					" > x5:"COLOR_SPEED_5"\u25CF"COLOR_CLEAR
					" > x2:"COLOR_SPEED_2"\u25CF"COLOR_CLEAR
					" > x1:"COLOR_SPEED_1"\u25CF"COLOR_CLEAR
					" > x0.5:"COLOR_SPEED_05"\u25CF"COLOR_CLEAR
					" < x0.5:"COLOR_SPEED_0"\u25CF"COLOR_CLEAR
					"\n");
		printf(COLOR_PART1"                      First part\n");
		printf("%s\n", ".-\"-.     .-\"-.     .-\"-.     .-\"-.     .-\"-.     .-\"-.\n"
				"     \"-.-\"     \"-.-\"     \"-.-\"     \"-.-\"     \"-.-\"    "COLOR_CLEAR);
	}

	if (last_part != current_part && current_part == 2) {
		printf(COLOR_INFO"\n%s"COLOR_CLEAR, "In this part, you can choose to protect "
				"your function or not to,\na color code will tell you if your "
				"function is protected/not BUT stay coherent !\n"COLOR_PROTECTED"[\U0001F6E1 ]"COLOR_INFO
				" --> protected\n"COLOR_NPROTECTED"[\U0001F4A5 ]"COLOR_INFO" --> not protected"COLOR_CLEAR);
		printf(COLOR_PART2"\n                     Second part\n");
		printf("%s\n", " __)(__  __)(__  __)(__  __)(__  __)(__  __)(__  __)(__  __)\n"
				"(______)(______)(______)(______)(______)(______)(______)(___\n");
	}

	if (last_part != current_part && current_part == 3) {
		printf(COLOR_PART3"\n%s\n",
			    " /~~~\\/~~\\/~~~\\/~~~\\/~~\\/~~~\\                    /~~~\\/~~\\/~~~\\/~~~\\/~~\\/~~~\\\n"
				" | /\\/ /\\/ /\\ || /\\/ /\\/ /\\ |                    | /\\ \\/\\ \\/\\ || /\\ \\/\\ \\/\\ |\n"
				"  \\ \\/ /\\/ /\\/ /\\ \\/ /\\/ /\\/ /     Bonus part     \\ \\/\\ \\/\\ \\/ /\\ \\/\\ \\/\\ \\/ /\n"
				"   \\ \\/\\ \\/\\ \\/  \\ \\/\\ \\/\\ \\/                      \\/ /\\/ /\\/ /  \\/ /\\/ /\\/ /\n"
				",_/\\ \\/\\ \\/\\ \\__/\\ \\/\\ \\/\\ \\______________________/ /\\/ /\\/ /\\__/ /\\/ /\\/ /\\_,\n"
				"(__/\\__/\\__/\\____/\\__/\\__/\\________________________/\\__/\\__/\\____/\\__/\\__/\\__)\n");
	}

	//bench mode header
	if (last_part == -2 && (g_bench || g_versus))
	{
		char	*opponent = (g_versus) ? g_versus : "system's libc";
		printf(COLOR_PART1"                               STARTING BENCH MODE\n");
		printf("   _.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._\n"
				".-'---      - ---     --     ---   -----   - --       ----  ----   -     ---`-.\n"
				" )  pts   time used by your libft      VS     %-25s pts  ( \n"COLOR_CLEAR, opponent);
	}

	last_part = current_part;
}

static char	*get_speed_color(_Bool inverted) {
	if (!TIME_DIFF_LIB) {
		dprintf(g_log_fd, " x0 (function time is 0, this is not possible)");
		return (COLOR_SPEED_CRASH);
	}
	float	diff = 0;
	if (inverted)
		diff = ((float)(TIME_DIFF_LIB) / (float)(TIME_DIFF_SYS));
	else
		diff = ((float)(TIME_DIFF_SYS) / (float)(TIME_DIFF_LIB));

	if (g_bench == 0 && g_versus == NULL)
		dprintf(g_log_fd, "x%.2f exec time", diff);
	else if (!inverted)
	{
		float	vdiff = diff;
		char	*opponent = (g_versus) ? g_versus : "system";
		char	*speeder = (vdiff > 1) ? "slower" : "faster";
		//if (vdiff < 1)
		//	vdiff = 1 / vdiff;
		dprintf(g_log_fd, "%s: x%.2f %s than %s's function (%llu cpu ticks vs %llu)\n",
				current_fun_name, vdiff, speeder, opponent, (unsigned long long)TIME_DIFF_LIB, (unsigned long long)TIME_DIFF_SYS);
	}
	if (diff > 10)
		return (COLOR_SPEED_10);
	else if (diff > 5)
		return (COLOR_SPEED_5);
	else if (diff > 2)
		return (COLOR_SPEED_2);
	else if (diff > 1)
		return (COLOR_SPEED_1);
	else if (diff > 0.5)
		return (COLOR_SPEED_05);
	else
		return (COLOR_SPEED_0);
}

static char *fg_color_to_bg(char *color, int buffer)
{
	static char	buff[2][0xF0];
	int		c = atoi(color + 7);

	sprintf(buff[buffer], "\033[48;5;%im", c);
	return buff[buffer];
}

static _Bool login_cmp(const char *l1, char *l2)
{
	while (*l1 && *l2 && *l1 == *l2 && *l2 != ' ')
		l1++, l2++;
	if (!*l1 && (!*l2 || *l2 == ' '))
		return true;
	return false;
}

static bool isCheater(void)
{
	char	buff[0xF00];
	int		i = 0;

	STDOUT_TO_BUFF;
	system("nm -u libft.so");
	GET_STDOUT(buff, sizeof(buff));
	while (forbidden_functions[i])
		if (strstr(buff, forbidden_functions[i++]))
			return true;
	return false;
}

static void updateRankingFile(int total_player_points)
{
	static struct {char name[9]; int points;}	users[0xF000];
	const char *login = getlogin();
	int			fd;
	struct stat	st;
	char		*file;
	char		*fstart;
	int			i = 1;

	if (!login)
		return ;
	if ((fd = open(BENCH_LOG_FILE, O_RDONLY)) != -1 && !fstat(fd, &st))
	{
		if (st.st_size != 0)
		{
			if ((fstart = file = mmap(NULL, st.st_size + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) != MAP_FAILED)
			{
				file[st.st_size] = 0;
				while (*file != '\0' && i < 0xF000 - 2)
				{
					if (fstart - file + st.st_size < 10)
						break ;
					strlcpy(users[i].name, file, 9);
					users[i].points = atoi(file + 10);
					if (!(strsep(&file, "\n")))
						break ;
					i += 2;
				}
			}
			else
			{
				printf("can't read bench file: %s", strerror(errno));
				return ;
			}
		}
		for (int j = 1; j < i; j += 2)
		{
			if (login_cmp(login, users[j].name))
				users[j].name[0] = 0;
		}
		for (int j = 1; j < i + 2 || i == 1; j += 2)
		{
			if (users[j].points < total_player_points || users[j].points == 0 || i == 1)
			{
				strlcpy(users[j - 1].name, login, 9);
				users[j - 1].points = total_player_points;
				break ;
			}
		}
		close(fd);
		if ((fd = open(BENCH_LOG_FILE, O_WRONLY | O_TRUNC)) != -1)
		{
			for (int j = 0; j < i; j++)
			{
				if (!users[j].name[0])
					continue ;
				dprintf(fd, "%-8s: %i pts\n", users[j].name, users[j].points);
			}
		}
		if (!close(fd))
			printf(COLOR_INFO "\nyour score [%i] have been added to the ranking file, you can view it here:\n"
					BENCH_LOG_FILE "\n" COLOR_CLEAR, total_player_points);
		munmap(fstart, st.st_size);
	}
}

void    display_test_result(int value, char *explanations)
{
	static char		*old_fun_name = NULL;
	static int		index = 0;
	static int		once = 0;
	static int		first = 0;
	static int		count = 0;
	static int		total_player_points = 0;
	static int		total_versus_points = 0;
	static t_err	errs[0xF00] = {{0, NULL, NULL, NULL}};

	MALLOC_RESET;

	count++;
	if (!old_fun_name || strcmp(old_fun_name, current_fun_name)) {
		first = 1;
		if (index != 0) {
			if (g_bench == 0 && g_versus == NULL)
				printf("\n");
			for (int i = 0; i < index && i < 0xF00 - 1; i++)
			{
				if (g_bench == 0 && g_versus == NULL)
					printf("[%s%s"COLOR_CLEAR"]: %s\n",
							verbose_color(errs[i].type),
							verbose_type(errs[i].type),
							errs[i].data);
				if (errs[i].type != TEST_TIMEOUT)
					dprintf(g_log_fd, "\n[%s]: %s\n", verbose_type(errs[i].type), errs[i].data);
				char *butified_code = butify(errs[i].code);
				dprintf(g_log_fd, "Test code:\n%s\n", butified_code);
				free(butified_code);
				if (errs[i].diff != NULL) {
					dprintf(g_log_fd, "Diffs:\n%s\n", errs[i].diff);
					free(errs[i].diff);
				}
			}
			index = 0;
		}
		if (value == TEST_FINISHED)
		{
			if (g_bench != 0 || g_versus != NULL)
			{
				char	*winner;
				int		winner_len;
				char	*sep1 = (count % 2) ? "( " : " )";
				char	*sep2 = !(count % 2) ? "( " : " )";

				winner = (total_player_points > total_versus_points) ? "WINNER: local libft" : "WINNER: system's libc";
				if (g_versus != NULL && total_versus_points > total_player_points)
					winner = "WINNER: john cena !";
 	 	 	 	 winner_len = strlen(winner);
				printf(COLOR_PART1"%s"COLOR_CLEAR"  | | %2i pts %*s "COLOR_BENCH_WINNER"%s"COLOR_CLEAR" %*s %2i pts | |  "COLOR_PART1"%s\n"COLOR_CLEAR,
						sep1,
						total_player_points,
						24 - winner_len / 2 - 1, "",
						winner,
						24 - winner_len / 2 - !(winner_len % 2) - 1, "",
						total_versus_points,
						sep2);
				printf(COLOR_PART1
						"(___       _       _       _       _       _       _       _       _       ___)\n"
    					"    (__  _) ( __ _) (__  _) (__ _ ) `-._.-' ( _ __) (_  __) (_ __ ) (_  __)\n"
    					"    ( _ __) (_  __) (__ __) `-._.-'         `-._.-' (__ __) (__  _) (__ _ )\n"
    					"    (__  _) (_ _ _) `-._.-'                         `-._.-' (_ _ _) (_  __)\n"
    					"    (_ ___) `-._.-'                                         `-._.-' (___ _)\n"
    					"    `-._.-'                                                         `-._.-'\n"
					COLOR_CLEAR);

				//log file:
				if (g_bench != 0 && g_versus == NULL && g_nobenchlog == 0)
				{
					if (isCheater())
						printf(COLOR_WARNING"\n\u26A0 :cheaters will not be added to the ranking file !\n"COLOR_CLEAR);
					else
						updateRankingFile(total_player_points);
				}
			}
			else if (isCheater())
			{
				printf(COLOR_WARNING"\n\n\u26A0 : your libft is using forbidden functions so the result may not be relevant !\n"COLOR_CLEAR);
			}
			return ;
		}
		if (old_fun_name) {
			if (g_bench == 0 && g_versus == NULL)
				printf("\n");
			dprintf(g_log_fd, "\n");
			once = 1;
		}
		display_part();
		if (g_bench == 0 && g_versus == NULL)
		{
			printf(COLOR_CLEAR"%s:%*s"COLOR_CLEAR,
					current_fun_name,
					14 - (int)strlen(current_fun_name),
					"");
			dprintf(g_log_fd, "%s:%*s", current_fun_name, 14 - (int)strlen(current_fun_name), "");
		}
		else
		{
			int		fun_len = strlen(current_fun_name);
			int		fun_padd = (14 - fun_len) / 2;
			char	*sep1 = (count % 2) ? "( " : " )";
			char	*sep2 = !(count++ % 2) ? "( " : " )";
			printf(COLOR_PART1"%s"COLOR_CLEAR"  | | %*s|%*s"COLOR_PART1"%s"COLOR_CLEAR"%*s|%*s | |  "COLOR_PART1"%s\n"COLOR_CLEAR, sep1, 24, "========================", fun_padd, "", current_fun_name, fun_padd - !(fun_len % 2), "", 24, "========================", sep2);
		}
	}
	char	*bench_name = NULL;
	int		bench_name_len = 0;
	bool	bench_timeout = false;
	switch (value) {
		case TEST_SUCCESS:
			printf(COLOR_SUCCESS"[OK] "COLOR_CLEAR);
			dprintf(g_log_fd, "[OK] ");
			break ;
		case TEST_TIMEOUT:
			if (g_bench != 0 || g_versus != NULL)
			{
				value = BENCH_FAT;
				bench_timeout = true;
				break ;
			}
			printf(COLOR_TIMEOUT"[TIMEOUT] "COLOR_CLEAR);
			dprintf(g_log_fd, "[TIMEOUT] ");
			errs[index].type = value;
			errs[index].data = "see the log file for informations";
			errs[index].diff = NULL;
			errs[index++].code = current_test_code;
			break ;
		case TEST_FAILED:
			printf(COLOR_FAILED"[FAILED] "COLOR_CLEAR);
			dprintf(g_log_fd, "[FAILED] ");
			errs[index].type = value;
			errs[index].data = explanations;
			errs[index].diff = get_diff();
			errs[index++].code = current_test_code;
			break ;
		case TEST_CRASH:
			printf(COLOR_FAILED"[CRASH] "COLOR_CLEAR);
			dprintf(g_log_fd, "[CRASH] ");
			errs[index].type = value;
			errs[index].data = explanations;
			errs[index].diff = NULL;
			errs[index++].code = current_test_code;
			break ;
		case TEST_NOCRASH:
			printf(COLOR_FAILED"[NO CRASH] "COLOR_CLEAR);
			dprintf(g_log_fd, "[NO CRASH] ");
			errs[index].type = value;
			errs[index].data = explanations;
			errs[index].diff = NULL;
			errs[index++].code = current_test_code;
			break ;
		case TEST_KO:
			printf(COLOR_KO"[KO] "COLOR_CLEAR);
			dprintf(g_log_fd, "[KO] ");
			errs[index].type = value;
			errs[index].data = explanations;
			errs[index].diff = get_diff();
			errs[index++].code = current_test_code;
			break ;
		case TEST_INTERUPT:
			printf(COLOR_FAILED"[INTERUPTED]"COLOR_CLEAR);
			dprintf(g_log_fd, "[INTERUPTED]");
			break ;
		case TEST_MISSING:
			if (once) {
				printf(COLOR_FAILED"[MISSING]"COLOR_CLEAR);
				dprintf(g_log_fd, "[MISSING]");
			}
			break ;
		case TEST_PROT:
			if (current_protected != INVISIBLE && first) {
				printf("%s] %s", (current_protected == PROTECTED) ? COLOR_PROTECTED"[\U0001F6E1 ": COLOR_NPROTECTED"[\U0001F4A5 ", COLOR_CLEAR);
				dprintf(g_log_fd, "%s", (current_protected == PROTECTED) ? "{protected}" : "{not protected}");
				first = 0;
			}
			break ;
		case TEST_SPEED:
			if (g_time.state == INVISIBLE)
				break ;
			printf("%s\u25CF%s", (g_time.state == TEST_CRASH) ? COLOR_SPEED_CRASH : get_speed_color(false), "\033[38;0m");
			if (g_time.state == TEST_CRASH) {
				dprintf(g_log_fd, "x??? the test has crash ...");
				errs[index].type = TEST_CRASH;
				errs[index].data = "speed test";
				errs[index].diff = NULL;
				errs[index++].code = current_test_code;
			}
			break ;
		case BENCH_FAT:
			bench_name = COLOR_BENCH_FAT"fat"COLOR_CLEAR;
			bench_name_len = 3;
		case BENCH_MEDIUM:
			if (!bench_name)
				bench_name = COLOR_BENCH_MEDIUM"medium"COLOR_CLEAR, bench_name_len = 6;
		case BENCH_SMALL:
			if (!bench_name)
				bench_name = COLOR_BENCH_SMALL"small"COLOR_CLEAR, bench_name_len = 5;
		case BENCH_RANDOM:
			if (!bench_name)
				bench_name = "random"COLOR_CLEAR, bench_name_len = 6;
			clock_t	global_ticks = TIME_DIFF_SYS + TIME_DIFF_LIB;
			float	p1, p2;
			if (global_ticks == 0 || bench_timeout)
			{
				p1 = 0;
				p2 = 1;
			}
			else
			{
				p1 = (float)TIME_DIFF_LIB / (float)global_ticks;
				p2 = (float)TIME_DIFF_SYS / (float)global_ticks;
			}
			if (p1 > p2 && g_time.state != TEST_CRASH)
				total_player_points++;
			else
				total_versus_points++;
			int		fun_padd = (14 - bench_name_len) / 2;
			int		nc1 = round(p1 * 20);
			int		nc2 = round(p2 * 20);
			char	*c1 = (p1 < p2) ? "\u25CF" : " ";
			char	*c2 = (p2 < p1) ? "\u25CF" : " ";
			char	*sep1 = (count % 2) ? "( " : " )";
			char	*sep2 = !(count % 2) ? "( " : " )";
			char	*color1 = (g_time.state == TEST_CRASH) ? COLOR_SPEED_CRASH : get_speed_color(false);
			char	*color2 = (g_time.state == TEST_CRASH) ? COLOR_SPEED_CRASH : get_speed_color(true);
			printf(COLOR_PART1"%s  "COLOR_CLEAR"|%s%s"COLOR_CLEAR"|%s%3.0f%% %*s%s%*s"COLOR_CLEAR
						"|%*s%s%*s|"
						"%s%*s"COLOR_CLEAR"%*s%s%3.0f%% "COLOR_CLEAR"|%s%s"COLOR_CLEAR"|  "COLOR_PART1"%s\n"COLOR_CLEAR,
						sep1,									// left decoration
						color1, c2,								// left color and point
						color1, p2 * 100,						// left color and percent
						20 - nc2, "",							// padding for colored spaces
						fg_color_to_bg(color1, 0),				// right colored spaces color
						nc2, "",								// colored spaces
						fun_padd, "",							// left padding for function name
						bench_name,								// function name / bench type
						fun_padd - !(bench_name_len % 2), "",	// right padding for function name
						fg_color_to_bg(color2, 1),				// right colored spaces color
						nc1, "",								// colored spaces
						20 - nc1, "",							// padding for colored spaces
						color2, p1 * 100,						// right color and percent
						color2, c1,								// right color and point
						sep2);									// right decoration
			/*					printf("%s\u25CF%s x%.2f (%7llu tick vs %-7llu)",
								(g_time.state == TEST_CRASH) ? COLOR_SPEED_CRASH : get_speed_color(),
								"\033[38;0m",
								((float)(TIME_DIFF_SYS) / (float)(TIME_DIFF_LIB)),
								TIME_DIFF_SYS,
								TIME_DIFF_LIB);*/
			if (g_time.state == TEST_CRASH) {
				dprintf(g_log_fd, "x??? the test has crash ...");
				errs[index].type = TEST_CRASH;
				errs[index].data = "bench test";
				errs[index].diff = NULL;
				errs[index++].code = current_test_code;
			}
			break ;
	}
	fflush(stdout);
	old_fun_name = current_fun_name;
	once = 0;
	//	printf("current_subtest_id = %i\n", current_subtest_id);
}
