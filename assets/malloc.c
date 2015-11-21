#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "libft_test.h"

void *malloc(size_t size)
{
	void	*(*real_malloc)(size_t);
	int		fd;
	void	*tmp;

	real_malloc = dlsym(RTLD_NEXT, "malloc");
	if ((fd = open(TMP_FILE, O_RDONLY)) != -1) {
		char		c;
		read(fd, &c, 1);
		switch (c) {
			case _MALLOC_NULL:
				return (NULL);
				break ;
			case _MALLOC_MEMSET:
				tmp = real_malloc(size);
				memset(tmp, 'A', size);
				return tmp;
				break ;
			case _MALLOC_SIZE:
				printf("%li bytes allocated\n", size);
				break ;
		}
		close(fd);
	}
	return real_malloc(size);
}

