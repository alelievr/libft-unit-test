#include "utils.h"

typedef char *(*proto_t) (char *, const char *, size_t);

static void
do_one_test (impl_t *impl, char *dst, const char *src, size_t n)
{
  size_t k = strlen (dst);
  if (CALL (impl, dst, src, n) != dst)
    {
      error (0, 0, "Wrong result in function %s %p != %p", impl->name,
	     CALL (impl, dst, src, n), dst);
      ret = 1;
      return;
    }

  size_t len = strlen (src);
  if (memcmp (dst + k, src, len + 1 > n ? n : len + 1) != 0)
    {
      error (0, 0, "Incorrect cancatination in function %s",
	     impl->name);
      ret = 1;
      return;
    }
  if (n < len && dst[k + n] != '\0')
    {
      error (0, 0, "There is no zero in the end of output string in %s",
	     impl->name);
      ret = 1;
      return;
    }
}

static void
do_test (size_t align1, size_t align2, size_t len1, size_t len2,
	 size_t n, int max_char)
{
  size_t i;
  char *s1, *s2;

  align1 &= 7;
  if (align1 + len1 >= page_size)
    return;
  if (align1 + n > page_size)
    return;
  align2 &= 7;
  if (align2 + len1 + len2 >= page_size)
    return;
  if (align2 + len1 + n > page_size)
    return;
  s1 = (char *) (buf1 + align1);
  s2 = (char *) (buf2 + align2);

  for (i = 0; i < len1; ++i)
    s1[i] = 32 + 23 * i % (max_char - 32);
  s1[len1] = '\0';

  for (i = 0; i < len2; i++)
    s2[i] = 32 + 23 * i % (max_char - 32);

  FOR_EACH_IMPL (impl, 0)
    {
      s2[len2] = '\0';
      do_one_test (impl, s2, s1, n);
    }
}

static void
do_random_tests (void)
{
  size_t i, j, n, align1, align2, len1, len2, N;
  unsigned char *p1 = buf1 + page_size - 512;
  unsigned char *p2 = buf2 + page_size - 512;
  unsigned char *res;
  for (n = 0; n < ITERATIONS; n++)
    {
      N = random () & 255;
      align1 = random () & 31;
      if (random () & 1)
	align2 = random () & 31;
      else
	align2 = align1 + (random () & 24);
      len1 = random () & 511;
      if (len1 + align2 > 512)
	len2 = random () & 7;
      else
	len2 = (512 - len1 - align2) * (random () & (1024 * 1024 - 1))
	  / (1024 * 1024);
      j = align1;
      if (align2 + len2 > j)
	j = align2 + len2;
      if (len1 + j >= 511)
	len1 = 510 - j - (random () & 7);
      if (len1 >= 512)
	len1 = 0;
      if (align1 + len1 < 512 - 8)
	{
	  j = 510 - align1 - len1 - (random () & 31);
	  if (j > 0 && j < 512)
	    align1 += j;
	}
      j = len1 + align1 + 64;
      if (j > 512)
	j = 512;
      for (i = 0; i < j; i++)
	{
	  if (i == len1 + align1)
	    p1[i] = 0;
	  else
	    {
	      p1[i] = random () & 255;
	      if (i >= align1 && i < len1 + align1 && !p1[i])
		p1[i] = (random () & 127) + 3;
	    }
	}
      for (i = 0; i < len2; i++)
	{
	  buf1[i] = random () & 255;
	  if (!buf1[i])
	    buf1[i] = (random () & 127) + 3;
	}
      buf1[len2] = 0;

      FOR_EACH_IMPL (impl, 1)
	{
	  memset (p2 - 64, '\1', align2 + 64);
	  memset (p2 + align2 + len2 + 1, '\1', 512 - align2 - len2 - 1);
	  memcpy (p2 + align2, buf1, len2 + 1);
	  res = (unsigned char *) CALL (impl, (char *) (p2 + align2),
					(char *) (p1 + align1), N);
	  if (res != p2 + align2)
	    {
	      error (0, 0, "Iteration %zd - wrong result in function %s "
		     "(%zd, %zd, %zd, %zd, %zd) %p != %p",
		     n, impl->name, align1, align2, len1, len2, N,
		     res, p2 + align2);
	      ret = 1;
	    }
	  for (j = 0; j < align2 + 64; ++j)
	    {
	      if (p2[j - 64] != '\1')
		{
		  error (0, 0, "Iteration %zd - garbage before dst, %s "
			 "%zd, %zd, %zd, %zd, %zd)",
			 n, impl->name, align1, align2, len1, len2, N);
		  ret = 1;
		  break;
		}
	    }
	  if (memcmp (p2 + align2, buf1, len2))
	    {
	      error (0, 0, "Iteration %zd - garbage in string before, %s "
		     "(%zd, %zd, %zd, %zd, %zd)",
		     n, impl->name, align1, align2, len1, len2, N);
	      ret = 1;
	    }

	  if ((len1 + 1) > N)
	    j = align2 + N + 1 + len2;
	  else
	    j = align2 + len1 + 1 + len2;
	  for (; j < 512; ++j)
	    {
	      if (p2[j] != '\1')
		{
		  error (0, 0, "Iteration %zd - garbage after, %s "
			 "(%zd, %zd, %zd, %zd, %zd)",
			 n, impl->name, align1, align2, len1, len2, N);
		  ret = 1;
		  break;
		}
	    }
	  if (len1 + 1 > N)
	    {
	      if (p2[align2 + N + len2] != '\0')
		{
		  error (0, 0, "Iteration %zd - there is no zero at the "
			 "end of output string, %s (%zd, %zd, %zd, %zd, %zd)",
			 n, impl->name, align1, align2, len1, len2, N);
		  ret = 1;
		}
	    }
	  if (memcmp (p1 + align1, p2 + align2 + len2,
		      (len1 + 1) > N ? N : len1 + 1))
	    {
	      error (0, 0, "Iteration %zd - different strings, %s "
		     "(%zd, %zd, %zd, %zd, %zd)",
		     n, impl->name, align1, align2, len1, len2, N);
	      ret = 1;
	    }
	}
    }
}


static void
test_strncat (void *ft)
{
	typeof(strncat) *ncat = (typeof(strncat) *)ft;
  /* First test it as strcat, with big counts, then test the count
     mechanism.  */
  it = "ft_strncat";
  (void) strcpy (one, "ijk");
  check (ncat (one, "lmn", 99) == one, 1);	/* Returned value. */
  equal (one, "ijklmn", 2);		/* Basic test. */

  (void) strcpy (one, "x");
  (void) ncat (one, "yz", 99);
  equal (one, "xyz", 3);		/* Writeover. */
  equal (one+4, "mn", 4);		/* Wrote too much? */

  (void) strcpy (one, "gh");
  (void) strcpy (two, "ef");
  (void) ncat (one, two, 99);
  equal (one, "ghef", 5);			/* Basic test encore. */
  equal (two, "ef", 6);			/* Stomped on source? */

  (void) strcpy (one, "");
  (void) ncat (one, "", 99);
  equal (one, "", 7);			/* Boundary conditions. */
  (void) strcpy (one, "ab");
  (void) ncat (one, "", 99);
  equal (one, "ab", 8);
  (void) strcpy (one, "");
  (void) ncat (one, "cd", 99);
  equal (one, "cd", 9);

  (void) strcpy (one, "ab");
  (void) ncat (one, "cdef", 2);
  equal (one, "abcd", 10);			/* Count-limited. */

  (void) ncat (one, "gh", 0);
  equal (one, "abcd", 11);			/* Zero count. */

  (void) ncat (one, "gh", 2);
  equal (one, "abcdgh", 12);		/* Count and length equal. */

  (void) ncat (one, "ij", (size_t)-1);	/* set sign bit in count */
  equal (one, "abcdghij", 13);

  int ntest = 14;
  char buf1[80] __attribute__ ((aligned (16)));
  char buf2[32] __attribute__ ((aligned (16)));
  for (size_t n1 = 0; n1 < 16; ++n1)
    for (size_t n2 = 0; n2 < 16; ++n2)
      for (size_t n3 = 0; n3 < 32; ++n3)
	for (size_t n4 = 0; n4 < 16; ++n4)
	  {
	    int olderrors = errors;

	    memset (buf1, 'b', sizeof (buf1));

	    memset (buf1 + n2, 'a', n3);
	    buf1[n2 + n3] = '\0';
	    strcpy (buf2 + n1, "123");

	    check (ncat (buf1 + n2, buf2 + n1, ~((size_t) 0) - n4)
		   == buf1 + n2, ntest);
	    if (errors == olderrors)
	      for (size_t i = 0; i < sizeof (buf1); ++i)
		{
		  if (i < n2)
		    check (buf1[i] == 'b', ntest);
		  else if (i < n2 + n3)
		    check (buf1[i] == 'a', ntest);
		  else if (i < n2 + n3 + 3)
		    check (buf1[i] == "123"[i - (n2 + n3)], ntest);
		  else if (i == n2 + n3 + 3)
		    check (buf1[i] == '\0', ntest);
		  else
		    check (buf1[i] == 'b', ntest);

		  if (errors != olderrors)
		    {
		      printf ("n1=%zu, n2=%zu, n3=%zu, n4=%zu, buf1=%02hhx",
			      n1, n2, n3, n4, buf1[0]);
		      for (size_t j = 1; j < sizeof (buf1); ++j)
			printf (",%02hhx", buf1[j]);
		      putchar_unlocked ('\n');
		      break;
		    }
		}
	  }
}

int
test_main_strncat (void *ft_strncat)
{
	INIT();
  size_t i, n;


	IMPL (ft_strncat, 2);

	test_strncat(ft_strncat);

  test_init ();

  for (n = 2; n <= 2048; n*=4)
    {
      do_test (0, 2, 2, 2, n, 127);
      do_test (0, 0, 4, 4, n, 127);
      do_test (4, 0, 4, 4, n, 255);
      do_test (0, 0, 8, 8, n, 127);
      do_test (0, 8, 8, 8, n, 127);

      for (i = 1; i < 8; ++i)
	{
	  do_test (0, 0, 8 << i, 8 << i, n, 127);
	  do_test (8 - i, 2 * i, 8 << i, 8 << i, n, 127);
	  do_test (0, 0, 8 << i, 2 << i, n, 127);
	  do_test (8 - i, 2 * i, 8 << i, 2 << i, n, 127);
	}

      for (i = 1; i < 8; ++i)
	{
	  do_test (i, 2 * i, 8 << i, 1, n, 127);
	  do_test (2 * i, i, 8 << i, 1, n, 255);
	  do_test (i, i, 8 << i, 10, n, 127);
	}
    }

  do_random_tests ();
  return ret;
}
