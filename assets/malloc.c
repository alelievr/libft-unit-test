#ifdef linux
# define _GNU_SOURCE
# define MALLOC_FUN_NAME malloc
#else
# define MALLOC_FUN_NAME lut_malloc
# define DYLD_INTERPOSE(_replacement,_replacee) \
	__attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
	__attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacement, (const void*)(unsigned long)&_replacee };
#endif

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <dlfcn.h>
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

void		*MALLOC_FUN_NAME(size_t size)
{
#ifdef linux
	static void	*(*real_malloc)(size_t) = NULL;
	if (real_malloc == NULL)
		real_malloc = dlsym(RTLD_NEXT, "malloc");
#else
	static void	*(*real_malloc)(size_t) = malloc;
#endif
	static t_map	*shmem = NULL;
	int				fd;
	void			*tmp;
	char			buff[16] = {[15]=0};
	char			c[2] = {0, 0};
	static int		once = 1;

	if (shmem == NULL)
		if ((fd = open(SHARED_MEM_FILE, O_RDONLY)) != -1)
		{
			if (!(read(fd, &shmem, 8) > 0))
				shmem = NULL;
			if (msync(shmem, 0xF00, MS_ASYNC) == -1)
				shmem = NULL;
			close(fd);
		}
	if (shmem) {
		if (once) {
			once = 0;
		}
		c[0] = shmem->alloc_byte_1;
		c[1] = shmem->alloc_byte_2;
	}
	if (c[1] == _MALLOC_DISABLE)
		shmem->alloc_byte_2 = _MALLOC_ENABLE;
	if (c[0])
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
				shmem->alloc_size = size;
				break ;
			case _MALLOC_DEBUG:
				ft_convert(buff, size, 10, 0);
				write(1, buff, strlen(buff));
				write(1, " bytes allocated\n", 18);
				break ;
		}
	return real_malloc(size);
}
#ifndef linux
 DYLD_INTERPOSE(MALLOC_FUN_NAME, malloc)
#endif
