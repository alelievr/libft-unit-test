/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 16:54:54 by alelievr          #+#    #+#             */
/*   Updated: 2016/11/11 17:47:40 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
impl_t			*__start_impls;
unsigned char	*buf1, *buf2;
int				ret, do_srandom = 1;
unsigned int	seed = 0x42;
size_t			page_size;
int				__i = 0;

int		main(void)
{
	__start_impls = calloc(sizeof(impl_t), 0xF00);
	test_main_strcpy();
	test_main_strcmp();
	return (0);
}
