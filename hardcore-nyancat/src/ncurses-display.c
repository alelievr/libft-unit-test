/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lolcat-ncurses.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inyancat <inyancat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/11/12 12:59:33 by inyancat          #+#    #+#             */
/*   Updated  2016/11/13 03:20:30 by inyancat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <ncurses.h>

#define RAINBOW_SIZE 105

//loop of 104 colors
static void		setup_rainbow(int shift)
{
	const float	freq = .3f;
	float		index;

	//first 105 colors
	for (int i = 0; i <= RAINBOW_SIZE; i++)
	{
		index = (float)(i + shift) / 5.f;

		short	red   = sin(freq * index + 0) * 500 + 500;
		short	green = sin(freq * index + 2 * M_PI / 3) * 500 + 500;
		short	blue  = sin(freq * index + 4 * M_PI / 3) * 500 + 500;
		init_color(i + 1, red, green, blue);
		init_pair(i + 1, i + 1, COLOR_BLACK);
	}

	//fire colors:
}

static void	lolwrite(char *str)
{
	static int	x = 0;
	static int	y = 0;

	while (*str)
	{
		if (*str == '\n')
		{
			x = 0, y++;
			printf("\n");
		}
		if (isprint(*str))
		{
			attrset(COLOR_PAIR(((x + y) % RAINBOW_SIZE) + 1));
			printw("%c", *str);
		}
		str++;
		x++;
	}
	refresh();
}

/*static void	switch_colors(void)
{
	char	buff[0xF000];
	char	*ptr = buff;
	int		x = 0;
	char	*color;
	int		color_index = 0;
	int		incre = 0;

	//detach process to parent but not from current terminal
	while (42)
	{
		//if terminal is closed
		if (!isatty(1))
			break ;

		for (int i = 0; i < 120; i++)
		{
			color = position_to_color(x++, 0);
			color_index++;
			ptr += sprintf(ptr, "\e]4;%i;rgb:%s\a\e[38;5;%im\e[m", color_index, color, color_index);
		}
		x = 0;
		color_index = incre++ % 256;
		write(1, buff, ptr - buff);
		usleep(1000 * 1000);
	}
}*/

void	ncurses_init(void)
{
	initscr();

	start_color();

	scrollok(stdscr,TRUE);

	//setup colors:
	setup_rainbow(0);

}

void	ncurses_deinit(void)
{
	endwin();
}

void	write_box(void)
{

}

void	write_result(char *fun, bool succeed)

	lolwrite(buff);

	for (int i = 0; i < 1000; i++)
	{
		setup_rainbow(i);
		usleep(1000);
	}
	return (0);
}
