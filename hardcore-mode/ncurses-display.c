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

#include "utils.h"
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <ncurses.h>

#define RAINBOW_SIZE	105
#define KO_SIZE			10
#define OK_SIZE			10

#define RAINBOW_INDEX	1
#define KO_INDEX		(2 + RAINBOW_SIZE)
#define OK_INDEX		(4 + RAINBOW_SIZE + KO_SIZE)

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

	//ko colors:
	for (int i = 0; i <= KO_SIZE; i++)
	{
		index = i + shift;

		short red = sin(freq * index * 10) * 250 + 750;
		short green = sin(freq * index * 10) * 250 + 350;
		short blue = 0;
		init_color(i + 2 + RAINBOW_SIZE, red, green, blue);
		init_pair(i + KO_INDEX, i + KO_INDEX, COLOR_BLACK);
	}
	
	//ok color:
	for (int i = 0; i <= KO_SIZE; i++)
	{
		index = i + shift;

		short red = 0;
		short green = sin(freq * index * 10) * 250 + 750;
		short blue = sin(freq * index * 10) * 500 + 250;
		init_color(i + OK_INDEX, red, green, blue);
		init_pair(i + OK_INDEX, i + OK_INDEX, COLOR_BLACK);
	}
}

static void	lolwrite(char *str, int start_x, int start_y, enum e_color_set set)
{
	int	x = start_x;
	int	y = start_y;

	move(start_y, start_x);
	while (*str)
	{
		if (*str == '\n')
		{
			x = start_x, y++;
			move(y, start_x);
		}
		if (isprint(*str))
		{
			if (set == COLOR_SET_RAINBOW)
				attrset(COLOR_PAIR(((x + y) % RAINBOW_SIZE) + 1));
			else if (set == COLOR_SET_KO)
				attrset(COLOR_PAIR(((x + y) % KO_SIZE) + KO_INDEX));
			else if (set == COLOR_SET_OK)
				attrset(COLOR_PAIR(((x + y) % KO_SIZE) + OK_INDEX));
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

	scrollok(stdscr, TRUE);

	//setup colors:
	setup_rainbow(0);
}

void	ncurses_deinit(void)
{
	endwin();
}

void	write_box(void)
{
	lolwrite(ASCII_HEADER, 0, 0, COLOR_SET_RAINBOW);
	lolwrite(ASCII_FOOTER, 0, 66, COLOR_SET_RAINBOW);
}

void	write_borders(void)
{
	int		start_y = 7;

	for (int i = 0; i < 15; i++)
	{
		lolwrite(ASCII_BORDER, 0, start_y + 4 * i, COLOR_SET_RAINBOW);
		lolwrite(ASCII_BORDER, 64, start_y + 4 * i, COLOR_SET_RAINBOW);
	}
}

void	ncurses_loop(void)
{
	for (int i = 0; i < 1000; i++)
	{
		setup_rainbow(i);
		usleep(1000 * 10);
	}
}

//x, y
//first box: 12, 7
//second box: 40, 7
//third box: 12, 23
//...
static void get_next_position(int *ret_x, int *ret_y)
{
	static int		x = 12;
	static int		y = 7;

	*ret_x = x;
	*ret_y = y;
	if (x == 12)
		x = 40;
	else if (x == 40)
	{
		x = 12;
		y += 7;
	}
}

void	write_result(char *fun, bool succeed)
{
	char	buff[0xF000];
	int		x;
	int		y;

	write_box();
	write_borders();
	get_next_position(&x, &y);
	if (succeed)
	{
		sprintf(buff, ASCII_FUNCTION_OK, fun);
		lolwrite(buff, x, y, COLOR_SET_OK);
	}
	else
	{
		sprintf(buff, ASCII_FUNCTION_KO, fun);
		lolwrite(buff, x, y, COLOR_SET_KO);
	}
}
