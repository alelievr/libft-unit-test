/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_test_result.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/13 20:26:54 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/18 23:35:53 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

void    display_test_result(int value, char *explications)
{
	static char	*old_fun_name = NULL;

	if (!old_fun_name || strcmp(old_fun_name, current_fun_name)) {
		if (old_fun_name)
			printf("\n");
		printf(COLOR_CLEAR"testing [%s]: "COLOR_CLEAR, current_fun_name);
	}
	switch (value) {
		case TEST_SUCCESS:
			printf("%s ", COLOR_SUCCESS"[OK]"COLOR_CLEAR);
			break ;
		case TEST_TIMEOUT:
			printf(COLOR_TIMEOUT"[TIMEOUT] "COLOR_CLEAR);
			break ;
		case TEST_FAILED:
			printf("%s have failed !\nTest information: %s\n"COLOR_CLEAR, current_fun_name, explications);
			break ;
		case TEST_CRASH:
			printf("%s have crash !\nTest information: %s\n"COLOR_CLEAR, current_fun_name, explications);
			break ;
		case TEST_NOCRASH:
			printf("%s\n"COLOR_CLEAR, explications);
			break ;
		case TEST_INTERUPT:
			printf("%s%s have been interupt by user !\n"COLOR_CLEAR, COLOR_FAILED, current_fun_name);
			break ;
		case TEST_MISSING:
			printf("%s%s is missing !\n"COLOR_CLEAR, COLOR_FAILED, current_fun_name);
			break ;
	}
	fflush(stdout);
	old_fun_name = current_fun_name;
//	printf("current_subtest_id = %i\n", current_subtest_id);
	run_subtests(NULL, current_subtest_id);
}
