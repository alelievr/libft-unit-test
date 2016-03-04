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

void		*malloc(size_t size)
{
	static void	*(*real_malloc)(size_t) = NULL;
	static unsigned long *shmem = NULL;
	int		fd;
	void	*tmp;
	char	buff[16] = {[15]=0};
	char		c[2] = {0, 0};
	int		fd2;
	static int	once = 1;

	if (shmem == NULL)
		if ((fd = open(SHARED_MEM_FILE, O_RDONLY)) != -1)
		{
			if (!(read(fd, &shmem, 8) > 0))
				shmem = NULL;
			close(fd);
		}

	if (real_malloc == NULL) {
		real_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	if (shmem) {
		if (once) {
			once = 0;
		//	printf("addr = %p\n", shmem);
		}
		c[0] = shmem[O_ALLOC_BYTE1];
		c[1] = shmem[O_ALLOC_BYTE2];
	}
	if (c[1] == _MALLOC_DISABLE)
		shmem[O_ALLOC_BYTE2] = _MALLOC_ENABLE;
	if (c[0] && c[1] == _MALLOC_ENABLE)
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
				shmem[O_ALLOC_SIZE] = size;
				break ;
			case _MALLOC_DEBUG:
				ft_convert(buff, size, 10, 0);
				write(1, buff, strlen(buff));
				write(1, " bytes allocated\n", 18);
				break ;
		}
	return real_malloc(size);
}
