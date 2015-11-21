/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 22:09:55 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/21 01:18:01 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

void		sandbox_sig_int(int s) {
	printf("%s", "sandbox exit !\n");
	fflush(stdout);
	unsandbox();
	kill(g_pid, SIGINT);
	(void)s;
}

void		sandbox(void) {
//	signal(SIGINT, sandbox_sig_int);
//	signal(SIGQUIT, sandbox_sig_int);
}

void		unsandbox(void) {
//	signal(SIGINT, SIG_DFL);
//	signal(SIGQUIT, SIG_DFL);
}
