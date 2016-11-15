#include "utils.h"
# define MIDDLE_CHAR 127
# define NULLRET(endptr) NULL

typedef CHAR *(*proto_t) (const CHAR *, int);

	static void
do_one_test (impl_t *impl, const CHAR *s, int c, const CHAR *exp_res)
{
  	CHAR *res = CALL (impl, s, c);
  	if (res != exp_res)
    {
      	error (0, 0, "Wrong result in function %s %#x %p %p", impl->name,
	     		c, res, exp_res);
      	ret = 1;
      	return;
    }
}

	static void
do_test (size_t align, size_t pos, size_t len, int seek_char, int max_char)
	/* For wcschr: align here means align not in bytes,
   	   but in wchar_ts, in bytes it will equal to align * (sizeof (wchar_t))
   	   len for wcschr here isn't in bytes but it's number of wchar_t symbols.  */
{
  	size_t i;
  	CHAR *result;
  	CHAR *buf = (CHAR *) buf1;
  	align &= 15;
  	if ((align + len) * sizeof (CHAR) >= page_size)
    	return;

  	for (i = 0; i < len; ++i)
    {
      	buf[align + i] = 32 + 23 * i % max_char;
      	if (buf[align + i] == seek_char)
			buf[align + i] = seek_char + 1;
      	else if (buf[align + i] == 0)
			buf[align + i] = 1;
    }
  	buf[align + len] = 0;

  	if (pos < len)
    {
      	buf[align + pos] = seek_char;
      	result = buf + align + pos;
    }
  	else if (seek_char == 0)
    	result = buf + align + len;
  	else
    	result = NULLRET (buf + align + len);

  	FOR_EACH_IMPL (impl, 0)
    	do_one_test (impl, buf + align, seek_char, result);
}

	static void
do_random_tests (void)
{
  	size_t i, j, n, align, pos, len;
  	int seek_char;
  	CHAR *result;
  	UCHAR *p = (UCHAR *) (buf1 + page_size - 512 * sizeof (CHAR));

  	for (n = 0; n < ITERATIONS; n++)
    {
      	/* For wcschr: align here means align not in bytes, but in wchar_ts,
	 	   in bytes it will equal to align * (sizeof (wchar_t)).  */
      	align = random () & 15;
      	pos = random () & 511;
      	seek_char = random () & 255;
      	if (pos + align >= 511)
			pos = 510 - align - (random () & 7);
      	/* len for wcschr here isn't in bytes but it's number of wchar_t
	 	   symbols.  */
      	len = random () & 511;
      	if ((pos == len && seek_char)
	  			|| (pos > len && (random () & 1)))
			len = pos + 1 + (random () & 7);
      	if (len + align >= 512)
			len = 511 - align - (random () & 7);
      	if (pos == len && seek_char)
			len = pos + 1;
      	j = (pos > len ? pos : len) + align + 64;
      	if (j > 512)
			j = 512;

      	for (i = 0; i < j; i++)
		{
	  		if (i == pos + align)
	    		p[i] = seek_char;
	  		else if (i == len + align)
	    		p[i] = 0;
	  		else
	    	{
	      		p[i] = random () & 255;
	      		if (i < pos + align && p[i] == seek_char)
					p[i] = seek_char + 13;
	      		if (i < len + align && !p[i])
				{
		  			p[i] = seek_char - 13;
		  			if (!p[i])
		    			p[i] = 140;
				}
	    	}
		}

      	if (pos <= len)
			result = (CHAR *) (p + pos + align);
      	else if (seek_char == 0)
			result = (CHAR *) (p + len + align);
      	else
			result = NULLRET ((CHAR *) (p + len + align));

      	FOR_EACH_IMPL (impl, 1)
			if (CALL (impl, (CHAR *) (p + align), seek_char) != result)
	  		{
	    		error (0, 0, "Iteration %zd - wrong result in function \
		   				%s (align in bytes: %zd, seek_char: %d, len: %zd, pos: %zd) %p != %p, p %p",
		   				n, impl->name, align * sizeof (CHAR), seek_char, len, pos,
		   				CALL (impl, (CHAR *) (p + align), seek_char), result, p);
	    		ret = 1;
	  		}
    }
}

	static void
test_strchr (void *ft)
{
	typeof(strchr) *chr = (typeof(strchr) *)ft;
  	it = "ft_strchr";
  	check (chr ("abcd", 'z') == NULL, 1);	/* Not found. */
  	(void) strcpy (one, "abcd");
  	check (chr (one, 'c') == one+2, 2);	/* Basic test. */
  	check (chr (one, 'd') == one+3, 3);	/* End of string. */
  	check (chr (one, 'a') == one, 4);		/* Beginning. */
  	check (chr (one, '\0') == one+4, 5);	/* Finding NUL. */
  	(void) strcpy (one, "ababa");
  	check (chr (one, 'b') == one+1, 6);	/* Finding first. */
  	(void) strcpy (one, "");
  	check (chr (one, 'b') == NULL, 7);		/* Empty string. */
  	check (chr (one, '\0') == one, 8);		/* NUL in empty string. */
  	{
    	char buf[4096];
    	int i;
    	char *p;
    	for (i=0; i < 0x100; i++)
      	{
			p = (char *) ((unsigned long int) (buf + 0xff) & ~0xff) + i;
			strcpy (p, "OK");
			strcpy (p+3, "BAD/WRONG");
			check (chr (p, '/') == NULL, 9+i);
      	}
   	}
}

	int
test_main_strchr (void *ft_strchr)
{
	INIT();
  	size_t i;

	IMPL (ft_strchr, 1);

	test_strchr(ft_strchr);

  		test_init ();

  	for (i = 1; i < 8; ++i)
    {
      	do_test (0, 16 << i, 2048, SMALL_CHAR, MIDDLE_CHAR);
      	do_test (i, 16 << i, 2048, SMALL_CHAR, MIDDLE_CHAR);
    }

  	for (i = 1; i < 8; ++i)
    {
      	do_test (i, 64, 256, SMALL_CHAR, MIDDLE_CHAR);
      	do_test (i, 64, 256, SMALL_CHAR, BIG_CHAR);
    }

  	for (i = 0; i < 32; ++i)
    {
      	do_test (0, i, i + 1, SMALL_CHAR, MIDDLE_CHAR);
      	do_test (0, i, i + 1, SMALL_CHAR, BIG_CHAR);
    }

  	for (i = 1; i < 8; ++i)
    {
      	do_test (0, 16 << i, 2048, 0, MIDDLE_CHAR);
      	do_test (i, 16 << i, 2048, 0, MIDDLE_CHAR);
    }

  	for (i = 1; i < 8; ++i)
    {
      	do_test (i, 64, 256, 0, MIDDLE_CHAR);
      	do_test (i, 64, 256, 0, BIG_CHAR);
    }

  	for (i = 0; i < 32; ++i)
    {
      	do_test (0, i, i + 1, 0, MIDDLE_CHAR);
      	do_test (0, i, i + 1, 0, BIG_CHAR);
    }

  	do_random_tests ();
  	return ret;
}
