/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inyancat <inyancat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/11/11 16:47:32 by inyancat          #+#    #+#             */
/*   Updated  2016/11/12 20:21:42 by inyancat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdarg.h>
extern int g_log_fd;

void error(int code, int a, const char *pattern, ...)
{
	va_list	va;

	va_start(va, pattern);
	(void)a;
	vdprintf(g_log_fd, pattern, va);
	dprintf(g_log_fd, "\n");
	va_end(va);
	exit(code);
}
