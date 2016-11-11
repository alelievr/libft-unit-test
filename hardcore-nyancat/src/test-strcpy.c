#include "utils.h"

# define STRCPY_RESULT(dst, len) dst

CHAR *SIMPLE_STRCPY (CHAR *, const CHAR *);

	CHAR *
SIMPLE_STRCPY (CHAR *dst, const CHAR *src)
{
  	CHAR *ret = dst;
  	while ((*dst++ = *src++) != '\0');
  	return ret;
}

typedef CHAR *(*proto_t) (CHAR *, const CHAR *);

	static void
do_one_test (impl_t *impl, CHAR *dst, const CHAR *src,
	    size_t len __attribute__((unused)))
{
  	if (CALL (impl, dst, src) != STRCPY_RESULT (dst, len))
    {
      	error (0, 0, "Wrong result in function %s %p %p", impl->name,
	     		CALL (impl, dst, src), STRCPY_RESULT (dst, len));
      	ret = 1;
      	return;
    }

  	if (STRCMP (dst, src) != 0)
    {
      	error (0, 0,
	     		"Wrong result in function %s dst \"%" sfmt "\" src \"%" sfmt "\"",
	     		impl->name, dst, src);
      	ret = 1;
      	return;
    }
}

	static void
do_test (size_t align1, size_t align2, size_t len, int max_char)
{
  	size_t i;
  	CHAR *s1, *s2;
	/* For wcscpy: align1 and align2 here mean alignment not in bytes,
   	   but in wchar_ts, in bytes it will equal to align * (sizeof (wchar_t))
   	   len for wcschr here isn't in bytes but it's number of wchar_t symbols.  */
  	align1 &= 7;
  	if ((align1 + len) * sizeof(CHAR) >= page_size)
    	return;

  	align2 &= 7;
  	if ((align2 + len) * sizeof(CHAR) >= page_size)
    	return;

  	s1 = (CHAR *) (buf1) + align1;
  	s2 = (CHAR *) (buf2) + align2;

  	for (i = 0; i < len; i++)
    	s1[i] = 32 + 23 * i % (max_char - 32);
  	s1[len] = 0;

  	FOR_EACH_IMPL (impl, 0)
    	do_one_test (impl, s2, s1, len);
}

	static void
do_random_tests (void)
{
  	size_t i, j, n, align1, align2, len;
  	UCHAR *p1 = (UCHAR *) (buf1 + page_size) - 512;
  	UCHAR *p2 = (UCHAR *) (buf2 + page_size) - 512;
  	UCHAR *res;

  	for (n = 0; n < ITERATIONS; n++)
    {
      	/* For wcsrchr: align1 and align2 here mean align not in bytes,
	 	   but in wchar_ts, in bytes it will equal to align * (sizeof
	 	   (wchar_t)).  For strrchr we need to check all alignments from
	 	   0 to 63 since some assembly implementations have separate
	 	   prolog for alignments more 48. */

      	align1 = random () & (63 / sizeof(CHAR));
      	if (random () & 1)
			align2 = random () & (63 / sizeof(CHAR));
      	else
			align2 = align1 + (random () & 24);
      	len = random () & 511;
      	j = align1;
      	if (align2 > j)
			j = align2;
      	if (len + j >= 511)
			len = 510 - j - (random () & 7);
      	j = len + align1 + 64;
      	if (j > 512)
			j = 512;
      	for (i = 0; i < j; i++)
		{
	  		if (i == len + align1)
	    		p1[i] = 0;
	  		else
	    	{
	      		p1[i] = random () & BIG_CHAR;
	      		if (i >= align1 && i < len + align1 && !p1[i])
					p1[i] = (random () & SMALL_CHAR) + 3;
	    	}
		}

      	FOR_EACH_IMPL (impl, 1)
		{
	  		MEMSET (p2 - 64, '\1', 512 + 64);
	  		res = (UCHAR *) CALL (impl, (CHAR *) (p2 + align2), (CHAR *) (p1 + align1));
	  		if (res != STRCPY_RESULT (p2 + align2, len))
	    	{
	      		error (0, 0, "Iteration %zd - wrong result in function %s (%zd, %zd, %zd) %p != %p",
		     			n, impl->name, align1, align2, len, res,
		     			STRCPY_RESULT (p2 + align2, len));
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
	  		for (j = align2 + len + 1; j < 512; ++j)
	    	{
	      		if (p2[j] != '\1')
				{
		  			error (0, 0, "Iteration %zd - garbage after, %s (%zd, %zd, %zd)",
			 				n, impl->name, align1, align2, len);
		  			ret = 1;
		  			break;
				}
	    	}
	  		if (MEMCMP (p1 + align1, p2 + align2, len + 1))
	    	{
	      		error (0, 0, "Iteration %zd - different strings, %s (%zd, %zd, %zd)",
		     			n, impl->name, align1, align2, len);
	      		ret = 1;
	    	}
		}
    }
}

	int
test_main_strcpy(void)
{
  	size_t i;

	IMPL (SIMPLE_STRCPY, 0)
	IMPL (STRCPY, 1)

  	test_init ();

  	printf ("%23s", "");
  	FOR_EACH_IMPL (impl, 0)
    	printf ("\t%s", impl->name);
  	putchar ('\n');

  	for (i = 0; i < 16; ++i)
    {
      	do_test (0, 0, i, SMALL_CHAR);
      	do_test (0, 0, i, BIG_CHAR);
      	do_test (0, i, i, SMALL_CHAR);
      	do_test (i, 0, i, BIG_CHAR);
    }

  	for (i = 1; i < 8; ++i)
    {
      	do_test (0, 0, 8 << i, SMALL_CHAR);
      	do_test (8 - i, 2 * i, 8 << i, SMALL_CHAR);
    }

  	for (i = 1; i < 8; ++i)
    {
      	do_test (i, 2 * i, 8 << i, SMALL_CHAR);
      	do_test (2 * i, i, 8 << i, BIG_CHAR);
      	do_test (i, i, 8 << i, SMALL_CHAR);
      	do_test (i, i, 8 << i, BIG_CHAR);
    }

  	do_random_tests ();
  	return ret;
}
