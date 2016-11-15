#include "utils.h"

# define STRNCPY_RESULT(dst, len, n) dst

typedef char *(*proto_t) (char *, const char *, size_t);

	static void
do_one_test (impl_t *impl, char *dst, const char *src, size_t len, size_t n)
{
  	if (CALL (impl, dst, src, n) != STRNCPY_RESULT (dst, len, n))
    {
      	error (0, 0, "Wrong result in function %s %p %p", impl->name,
	     		CALL (impl, dst, src, n), dst);
      	ret = 1;
      	return;
    }

  	if (memcmp (dst, src, len > n ? n : len) != 0)
    {
      	error (0, 0, "Wrong result in function %s", impl->name);
      	ret = 1;
      	return;
    }

  	if (n > len)
    {
      	size_t i;

      	for (i = len; i < n; ++i)
			if (dst [i] != '\0')
	  		{
	    		error (0, 0, "Wrong result in function %s", impl->name);
	    		ret = 1;
	    		return;
	  		}
    }
}

	static void
do_test (size_t align1, size_t align2, size_t len, size_t n, int max_char)
{
  	size_t i;
  	char *s1, *s2;

  	align1 &= 7;
  	if (align1 + len >= page_size)
    	return;

  	align2 &= 7;
  	if (align2 + len >= page_size)
    	return;

  	s1 = (char *) (buf1 + align1);
  	s2 = (char *) (buf2 + align2);

  	for (i = 0; i < len; ++i)
    	s1[i] = 32 + 23 * i % (max_char - 32);
  	s1[len] = 0;
  	for (i = len + 1; i + align1 < page_size && i < len + 64; ++i)
    	s1[i] = 32 + 32 * i % (max_char - 32);

  	FOR_EACH_IMPL (impl, 0)
    	do_one_test (impl, s2, s1, len, n);
}

	static void
do_random_tests (void)
{
  	size_t i, j, n, align1, align2, len, size, mode;
  	unsigned char *p1 = buf1 + page_size - 512;
  	unsigned char *p2 = buf2 + page_size - 512;
  	unsigned char *res;

  	for (n = 0; n < ITERATIONS; n++)
    {
      	mode = random ();
      	if (mode & 1)
		{
	  		size = random () & 255;
	  		align1 = 512 - size - (random () & 15);
	  		if (mode & 2)
	    		align2 = align1 - (random () & 24);
	  		else
	    		align2 = align1 - (random () & 31);
	  		if (mode & 4)
	    	{
	      		j = align1;
	      		align1 = align2;
	      		align2 = j;
	    	}
	  		if (mode & 8)
	    		len = size - (random () & 31);
	  		else
	    		len = 512;
	  		if (len >= 512)
	    		len = random () & 511;
		}
      	else
		{
	  		align1 = random () & 31;
	  		if (mode & 2)
	    		align2 = random () & 31;
	  		else
	    		align2 = align1 + (random () & 24);
	  		len = random () & 511;
	  		j = align1;
	  		if (align2 > j)
	    		j = align2;
	  		if (mode & 4)
	    	{
	      		size = random () & 511;
	      		if (size + j > 512)
					size = 512 - j - (random() & 31);
	    	}
	  		else
	    		size = 512 - j;
	  		if ((mode & 8) && len + j >= 512)
	    		len = 512 - j - (random () & 7);
		}
      	j = len + align1 + 64;
      	if (j > 512)
			j = 512;
      	for (i = 0; i < j; i++)
		{
	  		if (i == len + align1)
	    		p1[i] = 0;
	  		else
	    	{
	      		p1[i] = random () & 255;
	      		if (i >= align1 && i < len + align1 && !p1[i])
					p1[i] = (random () & 127) + 3;
	    	}
		}

      	FOR_EACH_IMPL (impl, 1)
		{
	  		memset (p2 - 64, '\1', 512 + 64);
	  		res = (unsigned char *) CALL (impl,
					(char *) (p2 + align2),
					(char *) (p1 + align1), size);
	  		if (res != STRNCPY_RESULT (p2 + align2, len, size))
	    	{
	      		error (0, 0, "Iteration %zd - wrong result in function %s (%zd, %zd, %zd) %p != %p",
		     			n, impl->name, align1, align2, len, res,
		     			STRNCPY_RESULT (p2 + align2, len, size));
	      		ret = 1;
	    	}
	  		for (j = 0; j < align2 + 64; ++j)
	    	{
	      		if (p2[j - 64] != '\1')
				{
		  			error (0, 0, "Iteration %zd - garbage before, %s (%zd, %zd, %zd)",
			 				n, impl->name, align1, align2, len);
		  			ret = 1;
		  			break;
				}
	    	}
	  		j = align2 + len + 1;
	  		if (size + align2 > j)
	    		j = size + align2;
	  		for (; j < 512; ++j)
	    	{
	      		if (p2[j] != '\1')
				{
		  			error (0, 0, "Iteration %zd - garbage after, %s (%zd, %zd, %zd)",
			 				n, impl->name, align1, align2, len);
		  			ret = 1;
		  			break;
				}
	    	}
	  		for (j = align2 + len + 1; j < align2 + size; ++j)
	    		if (p2[j])
	      		{
					error (0, 0, "Iteration %zd - garbage after size, %s (%zd, %zd, %zd)",
		       				n, impl->name, align1, align2, len);
					ret = 1;
					break;
	      		}
	  		j = len + 1;
	  		if (size < j)
	    		j = size;
	  		if (memcmp (p1 + align1, p2 + align2, j))
	    	{
	      		error (0, 0, "Iteration %zd - different strings, %s (%zd, %zd, %zd)",
		     			n, impl->name, align1, align2, len);
	      		ret = 1;
	    	}
		}
    }
}

static void
test_strncpy (void *ft)
{
	typeof(strncpy) *cc = (typeof(strncpy) *)ft;
  /* Testing is a bit different because of odd semantics.  */
  it = "ft_strncpy";
  check (cc (one, "abc", 4) == one, 1);	/* Returned value. */
  equal (one, "abc", 2);			/* Did the copy go right? */

  (void) strcpy (one, "abcdefgh");
  (void) cc (one, "xyz", 2);
  equal (one, "xycdefgh", 3);			/* Copy cut by count. */

  (void) strcpy (one, "abcdefgh");
  (void) cc (one, "xyz", 3);		/* Copy cut just before NUL. */
  equal (one, "xyzdefgh", 4);

  (void) strcpy (one, "abcdefgh");
  (void) cc (one, "xyz", 4);		/* Copy just includes NUL. */
  equal (one, "xyz", 5);
  equal (one+4, "efgh", 6);			/* Wrote too much? */

  (void) strcpy (one, "abcdefgh");
  (void) cc (one, "xyz", 5);		/* Copy includes padding. */
  equal (one, "xyz", 7);
  equal (one+4, "", 8);
  equal (one+5, "fgh", 9);

  (void) strcpy (one, "abc");
  (void) cc (one, "xyz", 0);		/* Zero-length copy. */
  equal (one, "abc", 10);

  (void) cc (one, "", 2);		/* Zero-length source. */
  equal (one, "", 11);
  equal (one+1, "", 12);
  equal (one+2, "c", 13);

  (void) strcpy (one, "hi there");
  (void) cc (two, one, 9);
  equal (two, "hi there", 14);		/* Just paranoia. */
  equal (one, "hi there", 15);		/* Stomped on source? */
}

	int
test_main_strncpy (void *ft_strncpy)
{
	INIT();
  	size_t i;

	test_strncpy(ft_strncpy);

	IMPL (ft_strncpy, 1);

  	test_init ();

  	for (i = 1; i < 8; ++i)
    {
      	do_test (i, i, 16, 16, 127);
      	do_test (i, i, 16, 16, 255);
      	do_test (i, 2 * i, 16, 16, 127);
      	do_test (2 * i, i, 16, 16, 255);
      	do_test (8 - i, 2 * i, 1 << i, 2 << i, 127);
      	do_test (2 * i, 8 - i, 2 << i, 1 << i, 127);
      	do_test (8 - i, 2 * i, 1 << i, 2 << i, 255);
      	do_test (2 * i, 8 - i, 2 << i, 1 << i, 255);
    }

  	for (i = 1; i < 8; ++i)
    {
      	do_test (0, 0, 4 << i, 8 << i, 127);
      	do_test (0, 0, 16 << i, 8 << i, 127);
      	do_test (8 - i, 2 * i, 4 << i, 8 << i, 127);
      	do_test (8 - i, 2 * i, 16 << i, 8 << i, 127);
    }

  	do_random_tests ();
  	return ret;
}
