/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test-utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inyancat <inyancat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/11/12 21:04:36 by inyancat          #+#    #+#             */
/*   Updated  2016/11/12 21:04:44 by inyancat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
extern int g_log_fd;

	void
check (int thing, int number)
{
	if (!thing)
	{
		//dprintf (g_log_fd, "%s flunked test %d\n", it, number);
		++errors;
	}
	(void)number;
}

/* Complain if first two args don't strcmp as equal.  */
	void
equal (const char *a, const char *b, int number)
{
	check (a != NULL && b != NULL && STREQ (a, b), number);
}
