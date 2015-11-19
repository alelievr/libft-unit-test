/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 17:48:58 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/17 23:12:19 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

void		ft_sig_success(int s) {
	(void)s;

	printf("OK !");
	display_test_result(TEST_SUCCESS, "ok");
}

void		ft_sig_missing(int s) {
	(void)s;

	display_test_result(TEST_MISSING, "function is missing");
}

void		ft_sig_user_interupt(int s) {
	(void)s;
	display_test_result(TEST_INTERUPT, "interupt by the user");
}

void		ft_sig_crash(int s) {
	(void)s;
	display_test_result(TEST_CRASH, current_explication);
}

void		ft_sig_timeout(int s) {
	(void)s;
	display_test_result(TEST_TIMEOUT, "timeout");
}

void		ft_sig_failed(int s) {
	(void)s;
	display_test_result(TEST_FAILED, current_explication);
}
