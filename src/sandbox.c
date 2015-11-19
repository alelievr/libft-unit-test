/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/17 22:09:55 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/18 01:13:00 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_test.h"

void		sandbox_sig_crash(int s) {
	pid_t	pid = getpid();
	printf("%s", "sandbox exit !");
	signal(SIGQUIT, SIG_DFL);
	kill(pid, SIGQUIT);
	(void)s;
}

void		sandbox(void) {
//	signal(SIGINT, sandbox_sig_crash);
//	signal(SIGQUIT, sandbox_sig_crash);
}
