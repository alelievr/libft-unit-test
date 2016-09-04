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

#define NEXT_LINE_CONTAINS(x) strnstr(code, x, code - strchr(code + 1, ';'))

static char	*butify(char *code) {
	char	*tmp;
	size_t	size = (strlen(code) + 0xF00);
	if (!(tmp = (char*)malloc(sizeof(char) * size)))
		return ("//BUFFER ERROR\\\\");
	char	*ret = tmp;
	int		indent = 1;
	int		bracket = 0;

	*tmp++ = '\t';
	while (*code) {
		if (bracket == 0 && *code == ';' && strchr(code + 1, ';'))
			while (NEXT_LINE_CONTAINS("g_shared_mem"))
				code = strchr(code + 1, ';');
		if (bracket == 0 && *code == ';' && (isspace(code[1]) || (code[1] == ';' && isspace(code[2]))))
		{
			while (*code && (isspace(*code) || *code == ';'))
				code++;
			*tmp++ = ';';
			*tmp++ = '\n';
			for (int i = 0; i < indent; i++)
				*tmp++ = '\t';
		}
		else
			*tmp++ = *code++;
		if (*code == '(')
			bracket++;
		if (*code == ')')
			bracket--;
	}
	*tmp = 0;
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
				"your function or not protect them,\na color code will tell you if your "
				"function is protected/not But stay coherent !\n"COLOR_PROTECTED"[||]"COLOR_INFO
				" --> protected\n"COLOR_NPROTECTED"[||]"COLOR_INFO" --> not protected"COLOR_CLEAR);
		printf(COLOR_PART2"\n                     Second part\n");
		printf("%s\n", " __)(__  __)(__  __)(__  __)(__  __)(__  __)(__  __)(__  __)\n"
				"(______)(______)(______)(______)(______)(______)(______)(___\n");
	}

	if (last_part != current_part && current_part == 3) {
		printf(COLOR_PART3"\n%s\n",
			    "/~~~\\/~~\\/~~~\\/~~~\\/~~\\/~~~\\                    /~~~\\/~~\\/~~~\\/~~~\\/~~\\/~~~\\\n"
				"| /\\/ /\\/ /\\ || /\\/ /\\/ /\\ |                    | /\\ \\/\\ \\/\\ || /\\ \\/\\ \\/\\ |\n"
				" \\ \\/ /\\/ /\\/ /\\ \\/ /\\/ /\\/ /     Bonus part     \\ \\/\\ \\/\\ \\/ /\\ \\/\\ \\/\\ \\/ /\n"
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
				current_fun_name, vdiff, speeder, opponent, TIME_DIFF_LIB, TIME_DIFF_SYS);
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

void    display_test_result(int value, char *explications)
{
	static char		*old_fun_name = NULL;
	static int		index = 0;
	static int		once = 0;
	static int		first = 0;
	static int		count = 0;
	static int		total_player_points = 0;
	static int		total_versus_points = 0;
	static t_err	errs[0xF00] = {{0, NULL, NULL, NULL}};

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
				dprintf(g_log_fd, "Test code:\n%s\n", butify(errs[i].code));
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
				char	*winner = (total_player_points > total_versus_points) ? "WINNER: local libft" : "WINNER: john cena !";
				if (g_versus == NULL)
					winner = "WINNER: system's libc";
				int		winner_len = strlen(winner);
				char	*sep1 = (count % 2) ? "( " : " )";
				char	*sep2 = !(count % 2) ? "( " : " )";
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
	switch (value) {
		case TEST_SUCCESS:
			printf(COLOR_SUCCESS"[OK] "COLOR_CLEAR);
			dprintf(g_log_fd, "[OK] ");
			break ;
		case TEST_TIMEOUT:
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
			errs[index].data = explications;
			errs[index].diff = get_diff();
			errs[index++].code = current_test_code;
			break ;
		case TEST_CRASH:
			printf(COLOR_FAILED"[CRASH] "COLOR_CLEAR);
			dprintf(g_log_fd, "[CRASH] ");
			errs[index].type = value;
			errs[index].data = explications;
			errs[index].diff = NULL;
			errs[index++].code = current_test_code;
			break ;
		case TEST_NOCRASH:
			printf(COLOR_FAILED"[NO CRASH] "COLOR_CLEAR);
			dprintf(g_log_fd, "[NO CRASH] ");
			errs[index].type = value;
			errs[index].data = explications;
			errs[index].diff = NULL;
			errs[index++].code = current_test_code;
			break ;
		case TEST_KO:
			printf(COLOR_KO"[KO] "COLOR_CLEAR);
			dprintf(g_log_fd, "[KO] ");
			errs[index].type = value;
			errs[index].data = explications;
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
				printf("%s[||] %s", (current_protected == PROTECTED) ? COLOR_PROTECTED : COLOR_NPROTECTED, COLOR_CLEAR);
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
			float	p1 = (float)TIME_DIFF_LIB / (float)global_ticks;
			float	p2 = (float)TIME_DIFF_SYS / (float)global_ticks;
			if (p1 > p2)
				total_player_points++;
			else
				total_versus_points++;
			int		fun_padd = (14 - bench_name_len) / 2;
			int		nc1 = round(p1 * 20);
			int		nc2 = round(p2 * 20);
			char	*c1 = (nc1 < nc2) ? "\u25CF" : " ";
			char	*c2 = (nc2 < nc1) ? "\u25CF" : " ";
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
