#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "libft_test.h"

int     ft_convert(char *buffer, int64_t n, int b, int maj)
{
	unsigned char   base[b + 1];
	int             i;
	int             tmp;

	i = -1;
	while (++i < b)
		if (i < 10)
			base[i] = '0' + i;
		else
			base[i] = (maj ? 'A' : 'a') + i - 10;
	i = 0;
	tmp = n;
	while (tmp >= b && ((++i)))
		tmp /= b;
	n *= b;
	while (n >= b && ((n /= b)))
		buffer[i--] = base[n % b];
	buffer[i--] = base[n % b];
	return (strlen(buffer));
}

void *malloc(size_t size)
{
	static void	*(*real_malloc)(size_t) = NULL;
	int		fd;
	void	*tmp;
	char	buff[16] = {[15]=0};
	int		fd2;

	if (real_malloc == NULL)
		real_malloc = dlsym(RTLD_NEXT, "malloc");
	if ((fd = open(TMP_FILE, O_RDWR)) != -1) {
		char		c[2];

		read(fd, c, 2);
		if (c[1] == _MALLOC_DISABLE) {
			lseek(fd, 1, SEEK_SET);
			write(fd, (char[1]){_MALLOC_ENABLE}, 1);
			return real_malloc(size);
		}
		close(fd);
		switch (c[0]) {
			case _MALLOC_NULL:
				return (NULL);
				break ;
			case _MALLOC_MEMSET:
				tmp = real_malloc(size);
				memset(tmp, 'A', size);
				return tmp;
				break ;
			case _MALLOC_SIZE:
				ft_convert(buff, size, 10, 0);
				if ((fd2 = open(MALLOC_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1)
					return (real_malloc(size));

				write(fd2, buff, strlen(buff) + 1);
				close(fd2);
				break ;
			case _MALLOC_DEBUG:
				ft_convert(buff, size, 10, 0);
				write(1, buff, strlen(buff));
				write(1, " bytes allocated\n", 18);
				break ;
		}
	}
	return real_malloc(size);
}
