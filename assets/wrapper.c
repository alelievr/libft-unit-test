/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/21 19:48:59 by alelievr          #+#    #+#             */
/*   Updated: 2015/11/21 19:59:45 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int		main(void) {
	char	*argv[] = {"run_test", NULL};
	char	*env[] = {"DYLD_INSERT_LIBRARIES=./assets/malloc.dylib", "DYLD_FORCE_FLAT_NAMESPACE=1", NULL};

	execve("./assets/libtests", argv, env);
	return (0);
}
