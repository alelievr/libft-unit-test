#include "utils.h"

typedef char *(*proto_t) (char *, const char *, size_t);

static void
do_one_test (impl_t *impl, char *dst, char *src, const char *orig_src,
	     size_t len)
{
  char *res;

  memcpy (src, orig_src, len);
  res = CALL (impl, dst, src, len);
  if (res != dst)
    {
      error (0, 0, "Wrong result in function %s %p %p", impl->name,
	     res, dst);
      ret = 1;
      return;
    }

  if (memcmp (dst, orig_src, len) != 0)
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
    do_one_test (impl, s2, (char *) (buf2 + align1), s1, len);
}

static void
do_random_tests (void)
{
  size_t i, n, align1, align2, len, size;
  size_t srcstart, srcend, dststart, dstend;
  int c;
  unsigned char *p1, *p2;
  unsigned char *res;

  for (n = 0; n < ITERATIONS; n++)
    {
      if ((random () & 255) == 0)
	size = 65536;
      else
	size = 512;
      if (size > page_size)
	size = page_size;
      if ((random () & 3) == 0)
	{
	  len = random () & (size - 1);
	  align1 = size - len - (random () & 31);
	  align2 = size - len - (random () & 31);
	  if (align1 > size)
	    align1 = 0;
	  if (align2 > size)
	    align2 = 0;
	}
      else
	{
	  align1 = random () & (size / 2 - 1);
	  align2 = random () & (size / 2 - 1);
	  len = random () & (size - 1);
	  if (align1 + len > size)
	    align1 = size - len;
	  if (align2 + len > size)
	    align2 = size - len;
	}

      p1 = buf1 + page_size - size;
      p2 = buf2 + page_size - size;
      c = random () & 255;
      srcend = align1 + len + 256;
      if (srcend > size)
	srcend = size;
      if (align1 > 256)
	srcstart = align1 - 256;
      else
	srcstart = 0;
      for (i = srcstart; i < srcend; ++i)
	p1[i] = random () & 255;
      dstend = align2 + len + 256;
      if (dstend > size)
	dstend = size;
      if (align2 > 256)
	dststart = align2 - 256;
      else
	dststart = 0;

      FOR_EACH_IMPL (impl, 1)
	{
	  memset (p2 + dststart, c, dstend - dststart);
	  memcpy (p2 + srcstart, p1 + srcstart, srcend - srcstart);
	  res = (unsigned char *) CALL (impl,
					(char *) (p2 + align2),
					(char *) (p2 + align1), len);
	  if (res != p2 + align2)
	    {
	      error (0, 0, "Iteration %zd - wrong result in function %s (%zd, %zd, %zd) %p != %p",
		     n, impl->name, align1, align2, len, res, p2 + align2);
	      ret = 1;
	    }
	  if (memcmp (p1 + align1, p2 + align2, len))
	    {
	      error (0, 0, "Iteration %zd - different strings, %s (%zd, %zd, %zd)",
		     n, impl->name, align1, align2, len);
	      ret = 1;
	    }
	  for (i = dststart; i < dstend; ++i)
	    {
	      if (i >= align2 && i < align2 + len)
		{
		  i = align2 + len - 1;
		  continue;
		}
	      if (i >= srcstart && i < srcend)
		{
		  i = srcend - 1;
		  continue;
		}
	      if (p2[i] != c)
		{
		  error (0, 0, "Iteration %zd - garbage in memset area, %s (%zd, %zd, %zd)",
			 n, impl->name, align1, align2, len);
		  ret = 1;
		  break;
		}
	    }

	  if (srcstart < align2
	      && memcmp (p2 + srcstart, p1 + srcstart,
			 (srcend > align2 ? align2 : srcend) - srcstart))
	    {
	      error (0, 0, "Iteration %zd - garbage before dst, %s (%zd, %zd, %zd)",
		     n, impl->name, align1, align2, len);
	      ret = 1;
	      break;
	    }

	  i = srcstart > align2 + len ? srcstart : align2 + len;
	  if (srcend > align2 + len
	      && memcmp (p2 + i, p1 + i, srcend - i))
	    {
	      error (0, 0, "Iteration %zd - garbage after dst, %s (%zd, %zd, %zd)",
		     n, impl->name, align1, align2, len);
	      ret = 1;
	      break;
	    }
	}
    }
}

static void
test_memmove (void *ft_memmove)
{
	typeof(memmove) *memmove_fun = (typeof(memmove) *)ft_memmove;
  it = "memmove";
  check(memmove_fun(one, "abc", 4) == one, 1);	/* Returned value. */
  equal(one, "abc", 2);			/* Did the copy go right? */

  (void) strcpy(one, "abcdefgh");
  (void) memmove_fun(one+1, "xyz", 2);
  equal(one, "axydefgh", 3);		/* Basic test. */

  (void) strcpy(one, "abc");
  (void) memmove_fun(one, "xyz", 0);
  equal(one, "abc", 4);			/* Zero-length copy. */

  (void) strcpy(one, "hi there");
  (void) strcpy(two, "foo");
  (void) memmove_fun(two, one, 9);
  equal(two, "hi there", 5);		/* Just paranoia. */
  equal(one, "hi there", 6);		/* Stomped on source? */

  (void) strcpy(one, "abcdefgh");
  (void) memmove_fun(one+1, one, 9);
  equal(one, "aabcdefgh", 7);		/* Overlap, right-to-left. */

  (void) strcpy(one, "abcdefgh");
  (void) memmove_fun(one+1, one+2, 7);
  equal(one, "acdefgh", 8);		/* Overlap, left-to-right. */

  (void) strcpy(one, "abcdefgh");
  (void) memmove_fun(one, one, 9);
  equal(one, "abcdefgh", 9);		/* 100% overlap. */
}

int
test_main_memmove (void *ft_memmove)
{
	INIT();
  size_t i;

	IMPL (ft_memmove, 1);

  test_init ();

  test_memmove(ft_memmove);

  for (i = 0; i < 14; ++i)
    {
      do_test (0, 32, 1 << i);
      do_test (32, 0, 1 << i);
      do_test (0, i, 1 << i);
      do_test (i, 0, 1 << i);
    }

  for (i = 0; i < 32; ++i)
    {
      do_test (0, 32, i);
      do_test (32, 0, i);
      do_test (0, i, i);
      do_test (i, 0, i);
    }

  for (i = 3; i < 32; ++i)
    {
      if ((i & (i - 1)) == 0)
	continue;
      do_test (0, 32, 16 * i);
      do_test (32, 0, 16 * i);
      do_test (0, i, 16 * i);
      do_test (i, 0, 16 * i);
    }

  do_random_tests ();
  return ret;
}
