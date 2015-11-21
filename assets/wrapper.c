/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/21 19:48:59 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/22 00:32:25 by bciss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "libft_test.h"

int		main(void) {
	char	*argv[] = {"run_test", NULL};
	char	*env[] = {"DYLD_INSERT_LIBRARIES=./assets/malloc.dylib", "DYLD_FORCE_FLAT_NAMESPACE=1", NULL};
	pid_t	pid;

	if ((pid = fork()) == 0)
		exit(execve("./assets/libtests", argv, env));
	else {
		wait(NULL);
		unlink(TMP_FILE);
	}
	return (0);
}
