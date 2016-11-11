#include "utils.h"

void test_init (void)
{
  	page_size = 2 * getpagesize ();
#ifdef MIN_PAGE_SIZE
  	if (page_size < MIN_PAGE_SIZE)
    	page_size = MIN_PAGE_SIZE;
#endif
  	buf1 = mmap (0, (BUF1PAGES + 1) * page_size, PROT_READ | PROT_WRITE,
	       	MAP_PRIVATE | MAP_ANON, -1, 0);
  	if (buf1 == MAP_FAILED)
    	error (EXIT_FAILURE, errno, "mmap failed");
  	if (mprotect (buf1 + BUF1PAGES * page_size, page_size, PROT_NONE))
    	error (EXIT_FAILURE, errno, "mprotect failed");
  	buf2 = mmap (0, 2 * page_size, PROT_READ | PROT_WRITE,
	       	MAP_PRIVATE | MAP_ANON, -1, 0);
  	if (buf2 == MAP_FAILED)
    	error (EXIT_FAILURE, errno, "mmap failed");
  	if (mprotect (buf2 + page_size, page_size, PROT_NONE))
    	error (EXIT_FAILURE, errno, "mprotect failed");
  	if (do_srandom)
    {
      	printf ("Setting seed to 0x%x\n", seed);
      	srandom (seed);
    }

  	memset (buf1, 0xa5, BUF1PAGES * page_size);
  	memset (buf2, 0x5a, page_size);
}
