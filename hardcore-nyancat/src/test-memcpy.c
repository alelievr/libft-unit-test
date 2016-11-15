# define MIN_PAGE_SIZE 
# define MEMCPY_RESULT(dst, len) dst
# include "utils.h"

typedef char *(*proto_t) (char *, const char *, size_t);

	static void
do_one_test (impl_t *impl, char *dst, const char *src,
	    size_t len)
{
  	if (CALL (impl, dst, src, len) != MEMCPY_RESULT (dst, len))
    {
      	error (0, 0, "Wrong result in function %s %p %p", impl->name,
	     		CALL (impl, dst, src, len), MEMCPY_RESULT (dst, len));
      	ret = 1;
      	return;
    }

  	if (memcmp (dst, src, len) != 0)
    {
      	error (0, 0, "Wrong result in function %s dst \"%s\" src \"%s\"",
	     		impl->name, dst, src);
      	ret = 1;
      	return;
    }
}

	static void
do_test (size_t align1, size_t align2, size_t len)
{
  	size_t i, j;
  	char *s1, *s2;

  	align1 &= 63;
  	if (align1 + len >= page_size)
    	return;

  	align2 &= 63;
  	if (align2 + len >= page_size)
    	return;

  	s1 = (char *) (buf1 + align1);
  	s2 = (char *) (buf2 + align2);

  	for (i = 0, j = 1; i < len; i++, j += 23)
    	s1[i] = j;

  	FOR_EACH_IMPL (impl, 0)
    	do_one_test (impl, s2, s1, len);
}

	static void
do_random_tests (void)
{
  	size_t i, j, n, align1, align2, len, size1, size2, size;
  	int c;
  	unsigned char *p1, *p2;
  	unsigned char *res;

  	for (n = 0; n < ITERATIONS; n++)
    {
      	if (n == 0)
		{
	  		len = getpagesize ();
	  		size = len + 512;
	  		size1 = size;
	  		size2 = size;
	  		align1 = 512;
	  		align2 = 512;
		}
      	else
		{
	  		if ((random () & 255) == 0)
	    		size = 65536;
	  		else
	    		size = 768;
	  		if (size > page_size)
	    		size = page_size;
	  		size1 = size;
	  		size2 = size;
	  		i = random ();
	  		if (i & 3)
	    		size -= 256;
	  		if (i & 1)
	    		size1 -= 256;
	  		if (i & 2)
	    		size2 -= 256;
	  		if (i & 4)
	    	{
	      		len = random () % size;
	      		align1 = size1 - len - (random () & 31);
	      		align2 = size2 - len - (random () & 31);
	      		if (align1 > size1)
					align1 = 0;
	      		if (align2 > size2)
					align2 = 0;
	    	}
	  		else
	    	{
	      		align1 = random () & 63;
	      		align2 = random () & 63;
	      		len = random () % size;
	      		if (align1 + len > size1)
					align1 = size1 - len;
	      		if (align2 + len > size2)
					align2 = size2 - len;
	    	}
		}
      	p1 = buf1 + page_size - size1;
      	p2 = buf2 + page_size - size2;
      	c = random () & 255;
      	j = align1 + len + 256;
      	if (j > size1)
			j = size1;
      	for (i = 0; i < j; ++i)
			p1[i] = random () & 255;

      	FOR_EACH_IMPL (impl, 1)
		{
	  		j = align2 + len + 256;
	  		if (j > size2)
	    		j = size2;
	  		memset (p2, c, j);
	  		res = (unsigned char *) CALL (impl,
					(char *) (p2 + align2),
					(char *) (p1 + align1), len);
	  		if (res != MEMCPY_RESULT (p2 + align2, len))
	    	{
	      		error (0, 0, "Iteration %zd - wrong result in function %s (%zd, %zd, %zd) %p != %p",
		     			n, impl->name, align1, align2, len, res,
		     			MEMCPY_RESULT (p2 + align2, len));
	      		ret = 1;
	    	}
	  		for (i = 0; i < align2; ++i)
	    	{
	      		if (p2[i] != c)
				{
		  			error (0, 0, "Iteration %zd - garbage before, %s (%zd, %zd, %zd)",
			 				n, impl->name, align1, align2, len);
		  			ret = 1;
		  			break;
				}
	    	}
	  		for (i = align2 + len; i < j; ++i)
	    	{
	      		if (p2[i] != c)
				{
		  			error (0, 0, "Iteration %zd - garbage after, %s (%zd, %zd, %zd)",
			 				n, impl->name, align1, align2, len);
		  			ret = 1;
		  			break;
				}
	    	}
	  		if (memcmp (p1 + align1, p2 + align2, len))
	    	{
	      		error (0, 0, "Iteration %zd - different strings, %s (%zd, %zd, %zd)",
		     			n, impl->name, align1, align2, len);
	      		ret = 1;
	    	}
		}
    }
}

	static void
test_memcpy (void *ft_memcpy)
{
	typeof(memcpy) *memcpy_fun = (typeof(memcpy) *)ft_memcpy;
  	int i;
  	it = "memcpy";
  	check(memcpy_fun(one, "abc", 4) == one, 1);	/* Returned value. */
  	equal(one, "abc", 2);			/* Did the copy go right? */

  	(void) strcpy(one, "abcdefgh");
  	(void) memcpy_fun(one+1, "xyz", 2);
  	equal(one, "axydefgh", 3);		/* Basic test. */

  	(void) strcpy(one, "abc");
  	(void) memcpy_fun(one, "xyz", 0);
  	equal(one, "abc", 4);			/* Zero-length copy. */

  	(void) strcpy(one, "hi there");
  	(void) strcpy(two, "foo");
  	(void) memcpy_fun(two, one, 9);
  	equal(two, "hi there", 5);		/* Just paranoia. */
  	equal(one, "hi there", 6);		/* Stomped on source? */

  	for (i = 0; i < 16; i++)
    {
      	const char *x = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
      	strcpy (one, x);
      	check (memcpy_fun (one + i, "hi there", 9) == one + i,
	     		7 + (i * 6));		/* Unaligned destination. */
      	check (memcmp (one, x, i) == 0, 8 + (i * 6));  /* Wrote under? */
      	equal (one + i, "hi there", 9 + (i * 6));
      	check (one[i + 9] == 'x', 10 + (i * 6));       /* Wrote over? */
      	check (memcpy_fun (two, one + i, 9) == two,
	     		11 + (i * 6));		/* Unaligned source. */
      	equal (two, "hi there", 12 + (i * 6));
    }
}

	int
test_main_memcpy (void *ft_memcpy)
{
  	size_t i;

  	INIT();

	IMPL (ft_memcpy, 1)

  	test_init (131072llu);

	test_memcpy(ft_memcpy);

  	for (i = 0; i < 18; ++i)
    {
      	do_test (0, 0, 1 << i);
      	do_test (i, 0, 1 << i);
      	do_test (0, i, 1 << i);
      	do_test (i, i, 1 << i);
    }

  	for (i = 0; i < 32; ++i)
    {
      	do_test (0, 0, i);
      	do_test (i, 0, i);
      	do_test (0, i, i);
      	do_test (i, i, i);
    }

  	for (i = 3; i < 32; ++i)
    {
      	if ((i & (i - 1)) == 0)
			continue;
      	do_test (0, 0, 16 * i);
      	do_test (i, 0, 16 * i);
      	do_test (0, i, 16 * i);
      	do_test (i, i, 16 * i);
    }

  	do_test (0, 0, getpagesize ());

  	do_random_tests ();
  	return ret;
}
