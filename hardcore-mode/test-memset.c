#include "utils.h"

typedef char *(*proto_t) (char *, int, size_t);
char *simple_memset (char *, int, size_t);

	char *
simple_memset (char *s, int c, size_t n)
{
  	char *r = s, *end = s + n;
  	while (r < end)
    	*r++ = c;
  	return s;
}

	static void
do_one_test (impl_t *impl, char *s, int c, size_t n)
{
  	char *res = CALL (impl, s, c, n);
  	char tstbuf[n];
  	if (res != s
      		|| simple_memset (tstbuf, c, n) != tstbuf
      		|| memcmp (s, tstbuf, n) != 0)
    {
      	error (0, 0, "Wrong result in function %s", impl->name);
      	ret = 1;
      	return;
    }
}

	static void
do_test (size_t align, int c, size_t len)
{
  	align &= 7;
  	if (align + len > page_size)
    	return;

  	FOR_EACH_IMPL (impl, 0)
    	do_one_test (impl, (char *) buf1 + align, c, len);
}

	static void
do_random_tests (void)
{
  	size_t i, j, k, n, align, len, size;
  	int c, o;
  	unsigned char *p, *res;

  	for (i = 0; i < 65536; ++i)
    	buf2[i] = random () & 255;

  	for (n = 0; n < ITERATIONS; n++)
    {
      	if ((random () & 31) == 0)
			size = 65536;
      	else
			size = 512;
      	p = buf1 + page_size - size;
      	len = random () & (size - 1);
      	align = size - len - (random () & 31);
      	if (align > size)
			align = size - len;
      	if ((random () & 7) == 0)
			align &= ~63;
      	if ((random () & 7) == 0)
			c = 0;
      	else
			c = random () & 255;
      	o = random () & 255;
      	if (o == c)
        	o = (c + 1) & 255;
      	j = len + align + 128;
      	if (j > size)
			j = size;
      	if (align >= 128)
			k = align - 128;
      	else
			k = 0;
      	for (i = k; i < align; ++i)
			p[i] = o;
      	for (i = align + len; i < j; ++i)
			p[i] = o;

      	FOR_EACH_IMPL (impl, 1)
		{
	  		for (i = 0; i < len; ++i)
	    	{
	      		p[i + align] = buf2[i];
	      		if (p[i + align] == c)
					p[i + align] = o;
	    	}
	  		res = (unsigned char *) CALL (impl, (char *) p + align, c, len);
	  		if (res != p + align)
	    	{
	      		error (0, 0, "Iteration %zd - wrong result in function %s (%zd, %d, %zd) %p != %p",
		     			n, impl->name, align, c, len, res, p + align);
	      		ret = 1;
	    	}
	  		for (i = k; i < align; ++i)
	    		if (p[i] != o)
	      		{
					error (0, 0, "Iteration %zd - garbage before %s (%zd, %d, %zd)",
		       				n, impl->name, align, c, len);
					ret = 1;
					break;
	      		}
	  		for (; i < align + len; ++i)
	    		if (p[i] != c)
	      		{
					error (0, 0, "Iteration %zd - not cleared correctly %s (%zd, %d, %zd)",
		       				n, impl->name, align, c, len);
					ret = 1;
					break;
	      		}
	  		for (; i < j; ++i)
	    		if (p[i] != o)
	      		{
					error (0, 0, "Iteration %zd - garbage after %s (%zd, %d, %zd)",
		       				n, impl->name, align, c, len);
					ret = 1;
					break;
	      		}
		}
    }
}

static void
test_memset (void *ft_memset)
{
	typeof(memset) *memset_fun = (typeof(memset) *)ft_memset;
  int i;

  it = "memset_fun";
  (void) strcpy(one, "abcdefgh");
  check(memset_fun(one+1, 'x', 3) == one+1, 1);	/* Return value. */
  equal(one, "axxxefgh", 2);		/* Basic test. */

  (void) memset_fun(one+2, 'y', 0);
  equal(one, "axxxefgh", 3);		/* Zero-length set. */

  (void) memset_fun(one+5, 0, 1);
  equal(one, "axxxe", 4);			/* Zero fill. */
  equal(one+6, "gh", 5);			/* And the leftover. */

  (void) memset_fun(one+2, 010045, 1);
  equal(one, "ax\045xe", 6);		/* Unsigned char convert. */

  /* Non-8bit fill character.  */
  memset_fun (one, 0x101, sizeof (one));
  for (i = 0; i < (int) sizeof (one); ++i)
    check (one[i] == '\01', 7);

  /* Test for more complex versions of memset_fun, for all alignments and
     lengths up to 256. This test takes a little while, perhaps it should
     be made weaker?  */
  {
    char data[512];
    int j;
    int k;
    int c;

    for (i = 0; i < 512; i++)
      data[i] = 'x';
    for (c = 0; c <= 'y'; c += 'y')  /* check for memset_fun(,0,) and
					memset_fun(,'y',) */
      for (j = 0; j < 256; j++)
	for (i = 0; i < 256; i++)
	  {
	    memset_fun (data + i, c, j);
	    for (k = 0; k < i; k++)
	      if (data[k] != 'x')
		goto fail;
	    for (k = i; k < i+j; k++)
	      {
		if (data[k] != c)
		  goto fail;
		data[k] = 'x';
	      }
	    for (k = i+j; k < 512; k++)
	      if (data[k] != 'x')
		goto fail;
	    continue;

	  fail:
	    check (0, 8 + i + j * 256 + (c != 0) * 256 * 256);
	  }
  }
}

	int
test_main_memset (void *ft_memset)
{
	INIT();
	IMPL (ft_memset, 1)
  	size_t i;
  	int c;

  	test_init (131072llu);

	test_memset(ft_memset);

  	for (c = -65; c <= 130; c += 65)
    {
      	for (i = 0; i < 18; ++i)
			do_test (0, c, 1 << i);
      	for (i = 1; i < 32; ++i)
		{
	  		do_test (i, c, i);
	  		if (i & (i - 1))
	    		do_test (0, c, i);
		}
      	do_test (1, c, 14);
      	do_test (3, c, 1024);
      	do_test (4, c, 64);
      	do_test (2, c, 25);
    }

  	do_random_tests ();
  	return ret;
}
