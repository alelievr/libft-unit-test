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

static char	*butify(char *code) {
	char	*tmp;
	size_t	size = (strlen(code) + 0xF00);
	if (!(tmp = (char*)malloc(sizeof(char) * size)))
		return ("//BUFFER ERROR\\\\");
	char	*ret = tmp;
	int		indent = 1;

	*tmp++ = '\t';
	while (*code) {
		if (*code == ';' && (isspace(code[1]) || (code[1] == ';' && isspace(code[2])))) {
			while (*code && (isspace(*code) || *code == ';'))
				code++;
			*tmp++ = ';';
			*tmp++ = '\n';
			for (int i = 0; i < indent; i++)
				*tmp++ = '\t';
		}
		else
			*tmp++ = *code++;
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

static void	display_part(char *s) {
	if (!strcmp(s, "ft_memset")) {
		printf(COLOR_CLEAR"speed scale: > x10:"COLOR_SPEED_10"\u25CF"COLOR_CLEAR
				" > x5:"COLOR_SPEED_5"\u25CF"COLOR_CLEAR
				" > x2:"COLOR_SPEED_2"\u25CF"COLOR_CLEAR
				" > x1:"COLOR_SPEED_1"\u25CF"COLOR_CLEAR
				" > x0.5:"COLOR_SPEED_05"\u25CF"COLOR_CLEAR
				" < x0.5:"COLOR_SPEED_0"\u25CF"COLOR_CLEAR
				"\n"
				"\033[38;5;244mto disable this, run \"make f NOSPEED=1\" or \"./run_test nospeed\"\033[0m\n");
		printf(COLOR_PART1"                      First part\n");
		printf("%s\n", ".-\"-.     .-\"-.     .-\"-.     .-\"-.     .-\"-.     .-\"-.\n"
				       "     \"-.-\"     \"-.-\"     \"-.-\"     \"-.-\"     \"-.-\"    "COLOR_CLEAR);
	}
	if (!strcmp(s, "ft_memalloc")) {
		printf(COLOR_INFO"\n%s"COLOR_CLEAR, "In this part, you can choose to protect "
				"your function or not protect them,\na color code will tell you if your "
				"function is protected/not But stay coherent !\n"COLOR_PROTECTED"[||]"COLOR_INFO
				" --> protected\n"COLOR_NPROTECTED"[||]"COLOR_INFO" --> not protected"COLOR_CLEAR);
		printf(COLOR_PART2"\n                     Second part\n");
		printf("%s\n", " __)(__  __)(__  __)(__  __)(__  __)(__  __)(__  __)(__  __)\n"
					   "(______)(______)(______)(______)(______)(______)(______)(___\n");
	}

	if (!strcmp(s, "ft_lstnew")) {
		printf(COLOR_PART3"\n%s\n",
			     "	/~~~\\/~~\\/~~~\\/~~~\\/~~\\/~~~\\                    /~~~\\/~~\\/~~~\\/~~~\\/~~\\/~~~\\\n"
				 "| /\\/ /\\/ /\\ || /\\/ /\\/ /\\ |                    | /\\ \\/\\ \\/\\ || /\\ \\/\\ \\/\\ |\n"
				 " \\ \\/ /\\/ /\\/ /\\ \\/ /\\/ /\\/ /     Bonus part     \\ \\/\\ \\/\\ \\/ /\\ \\/\\ \\/\\ \\/ /\n"
				 "   \\ \\/\\ \\/\\ \\/  \\ \\/\\ \\/\\ \\/                      \\/ /\\/ /\\/ /  \\/ /\\/ /\\/ /\n"
				 ",_/\\ \\/\\ \\/\\ \\__/\\ \\/\\ \\/\\ \\______________________/ /\\/ /\\/ /\\__/ /\\/ /\\/ /\\_,\n"
				 "(__/\\__/\\__/\\____/\\__/\\__/\\________________________/\\__/\\__/\\____/\\__/\\__/\\__)\n");
	}
}

static char	*get_speed_color(void) {
	if (!TIME_DIFF_LIB) {
		dprintf(g_log_fd, " x0 (function time is 0, this is not possible)");
		return (COLOR_SPEED_CRASH);
	}
	float	diff = ((float)(TIME_DIFF_SYS) / (float)(TIME_DIFF_LIB));

	dprintf(g_log_fd, "x%.2f exec time", diff);
//	printf("%llu - %llu\n", TIME_DIFF_SYS, TIME_DIFF_LIB);
//	printf("%f\n", diff);
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

void    display_test_result(int value, char *explications)
{
	static char		*old_fun_name = NULL;
	static int		index = 0;
	static int		once = 0;
	static int		first = 0;
	static t_err	errs[0xF00] = {{0, NULL, NULL, NULL}};

	if (!old_fun_name || strcmp(old_fun_name, current_fun_name)) {
		first = 1;
		if (index != 0) {
			printf("\n");
			for (int i = 0; i < index && i < 0xF00 - 1; i++) {
				printf("[%s%s"COLOR_CLEAR"]: %s\n", verbose_color(errs[i].type), verbose_type(errs[i].type), errs[i].data);
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
			return ;
		if (old_fun_name) {
			printf("\n");
			dprintf(g_log_fd, "\n");
			once = 1;
		}
		display_part(current_fun_name);
		printf(COLOR_CLEAR"%s:%*s"COLOR_CLEAR, current_fun_name, 14 - (int)strlen(current_fun_name), "");
		dprintf(g_log_fd, "%s:%*s", current_fun_name, 14 - (int)strlen(current_fun_name), "");
	}
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
			printf("%s\u25CF%s", (g_time.state == TEST_CRASH) ? COLOR_SPEED_CRASH : get_speed_color(), "\033[38;0m");
			if (g_time.state == TEST_CRASH)
				dprintf(g_log_fd, "x??? the test has crash ...");
			break ;
	}
	fflush(stdout);
	old_fun_name = current_fun_name;
	once = 0;
//	printf("current_subtest_id = %i\n", current_subtest_id);
}
