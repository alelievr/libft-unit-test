/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_test_result.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/13 20:26:54 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/21 01:06:10 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"
#include <ctype.h>

char	*butify(char *code) {
	code = strdup(code);
	char	*tmp = code;

	while (*code) {
		if (*code == ';' && isspace(code[1]))
			code[1] = '\n';
		code++;
	}
	return (tmp);
}

char	*verbose_type(int type) {
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
		default:
			return "";
			break ;
	}
}

void    display_test_result(int value, char *explications)
{
	static char		*old_fun_name = NULL;
	static int		index = 0;
	static t_err	errs[0xF00] = {{0, NULL, NULL}};

	if (!old_fun_name || strcmp(old_fun_name, current_fun_name)) {
		if (index != 0) {
			printf("\n");
			for (int i = 0; i < index; i++) {
				printf("["COLOR_FAILED"%s"COLOR_CLEAR"]: %s\n", verbose_type(errs[i].type), errs[i].data);
				dprintf(g_log_fd, "[%s]: %s\n", verbose_type(errs[i].type), errs[i].data);
				dprintf(g_log_fd, "Test code:\n%s\n", butify(errs[i].code));
			}
			index = 0;
		}
		if (old_fun_name) {
			printf("\n");
			dprintf(g_log_fd, "\n");
		}
		printf(COLOR_CLEAR"testing [%s]: "COLOR_CLEAR, current_fun_name);
		dprintf(g_log_fd, "testing [%s]: ", current_fun_name);
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
			errs[index++].code = current_test_code;
			break ;
		case TEST_FAILED:
			printf(COLOR_FAILED"[FAILED] "COLOR_CLEAR);
			dprintf(g_log_fd, "[FAILED] ");
			errs[index].type = value;
			errs[index].data = explications;
			errs[index++].code = current_test_code;
			break ;
		case TEST_CRASH:
			printf(COLOR_FAILED"[CRASH] "COLOR_CLEAR);
			dprintf(g_log_fd, "[CRASH] ");
			errs[index].type = value;
			errs[index].data = explications;
			errs[index++].code = current_test_code;
			break ;
		case TEST_NOCRASH:
			printf(COLOR_FAILED"[NO CRASH] "COLOR_CLEAR);
			dprintf(g_log_fd, "[NO CRASH] ");
			errs[index].type = value;
			errs[index].data = explications;
			errs[index++].code = current_test_code;
			break ;
		case TEST_INTERUPT:
			printf(COLOR_FAILED"[INTERUPTED]"COLOR_CLEAR);
			dprintf(g_log_fd, "[INTERUPTED]");
			break ;
		case TEST_MISSING:
			printf(COLOR_FAILED"%s is missing !\n"COLOR_CLEAR, current_fun_name);
			dprintf(g_log_fd, "%s is missing !\n", current_fun_name);
			return ;
			break ;
	}
	fflush(stdout);
	old_fun_name = current_fun_name;
//	printf("current_subtest_id = %i\n", current_subtest_id);
	run_subtests(NULL, current_subtest_id);
}
