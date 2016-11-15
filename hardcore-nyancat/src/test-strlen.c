#include "utils.h"
# define MAX_CHAR CHAR_MAX

typedef size_t (*proto_t) (const CHAR *);

	size_t
simple_STRLEN (const CHAR *s)
{
  	const CHAR *p;

  	for (p = s; *p; ++p);
  	return p - s;
}

	static void
do_one_test (impl_t *impl, const CHAR *s, size_t exp_len)
{
  	size_t len = CALL (impl, s);
  	if (len != exp_len)
    {
      	error (0, 0, "Wrong result in function %s %zd %zd", impl->name,
	     		len, exp_len);
      	ret = 1;
      	return;
    }
}

	static void
do_test (size_t align, size_t len)
{
  	size_t i;

  	align &= 63;
  	if (align + sizeof(CHAR) * len >= page_size)
    	return;

  	CHAR *buf = (CHAR *) (buf1);

  	for (i = 0; i < len; ++i)
    	buf[align + i] = 1 + 11111 * i % MAX_CHAR;
  	buf[align + len] = 0;

  	FOR_EACH_IMPL (impl, 0)
    	do_one_test (impl, (CHAR *) (buf + align), len);
}

	static void
do_random_tests (void)
{
  	size_t i, j, n, align, len;
  	CHAR *p = (CHAR *) (buf1 + page_size - 512 * sizeof(CHAR));

  	for (n = 0; n < ITERATIONS; n++)
    {
      	align = random () & 15;
      	len = random () & 511;
      	if (len + align > 510)
			len = 511 - align - (random () & 7);
      	j = len + align + 64;
      	if (j > 512)
			j = 512;

      	for (i = 0; i < j; i++)
		{
	  		if (i == len + align)
	    		p[i] = 0;
	  		else
	    	{
	      		p[i] = random () & 255;
	      		if (i >= align && i < len + align && !p[i])
					p[i] = (random () & 127) + 1;
	    	}
		}

      	FOR_EACH_IMPL (impl, 1)
			if (CALL (impl, (CHAR *) (p + align)) != len)
	  		{
	    		error (0, 0, "Iteration %zd - wrong result in function %s (%zd) %zd != %zd, p %p",
		   				n, impl->name, align, CALL (impl, (CHAR *) (p + align)),
		   				len, p);
	    		ret = 1;
	  		}
    }
}

	int
test_main_strlen (void *ft_strlen)
{
	it = "ft_strlen";
	INIT();
  	size_t i;

  	test_init ();

	IMPL (ft_strlen, 1)

  		/* Checking with only 4 * N alignments for wcslen, other alignments are wrong for wchar_t type arrays*/

  		for (i = 1; i < 8; ++i)
  		{
    		do_test (sizeof(CHAR) * i, i);
    		do_test (0, i);
  		}

  	for (i = 2; i <= 12; ++i)
    {
      	do_test (0, 1 << i);
      	do_test (sizeof(CHAR) * 7, 1 << i);
      	do_test (sizeof(CHAR) * i, 1 << i);
      	do_test (sizeof(CHAR) * i, (size_t)((1 << i) / 1.5));
    }

  	do_random_tests ();
  	return ret;
}
