#include "utils.h"

typedef char *(*proto_t) (const char *, int, size_t);

static void
do_one_test (impl_t *impl, const char *s, int c, size_t n, char *exp_res)
{
  char *res = CALL (impl, s, c, n);
  if (res != exp_res)
    {
      error (0, 0, "Wrong result in function %s %p %p", impl->name,
	     res, exp_res);
      ret = 1;
      return;
    }
}

static void
do_test (size_t align, size_t pos, size_t len, int seek_char)
{
  size_t i;
  char *result;

  align &= 7;
  if (align + len >= page_size)
    return;

  for (i = 0; i < len; ++i)
    {
      buf1[align + i] = 1 + 23 * i % 127;
      if (buf1[align + i] == seek_char)
        buf1[align + i] = seek_char + 1;
    }
  buf1[align + len] = 0;

  if (pos < len)
    {
      buf1[align + pos] = seek_char;
      buf1[align + len] = -seek_char;
      result = (char *) (buf1 + align + pos);
    }
  else
    {
      result = NULL;
      buf1[align + len] = seek_char;
    }

  FOR_EACH_IMPL (impl, 0)
    do_one_test (impl, (char *) (buf1 + align), seek_char, len, result);
}

static void
do_random_tests (void)
{
  size_t i, j, n, align, pos, len;
  int seek_char;
  char *result;
  unsigned char *p = buf1 + page_size - 512;

  for (n = 0; n < ITERATIONS; n++)
    {
      align = random () & 15;
      pos = random () & 511;
      if (pos + align >= 512)
	pos = 511 - align - (random () & 7);
      len = random () & 511;
      if (pos >= len)
	len = pos + (random () & 7);
      if (len + align >= 512)
        len = 512 - align - (random () & 7);
      seek_char = random () & 255;
      j = len + align + 64;
      if (j > 512)
        j = 512;

      for (i = 0; i < j; i++)
	{
	  if (i == pos + align)
	    p[i] = seek_char;
	  else
	    {
	      p[i] = random () & 255;
	      if (i < pos + align && p[i] == seek_char)
		p[i] = seek_char + 13;
	    }
	}

      if (pos < len)
	{
	  size_t r = random ();
	  if ((r & 31) == 0)
	    len = ~(uintptr_t) (p + align) - ((r >> 5) & 31);
	  result = (char *) (p + pos + align);
	}
      else
	result = NULL;

      FOR_EACH_IMPL (impl, 1)
	if (CALL (impl, (char *) (p + align), seek_char, len) != result)
	  {
	    error (0, 0, "Iteration %zd - wrong result in function %s (%zd, %d, %zd, %zd) %p != %p, p %p",
		   n, impl->name, align, seek_char, len, pos,
		   CALL (impl, (char *) (p + align), seek_char, len),
		   result, p);
	    ret = 1;
	  }
    }
}

static void
test_memchr (void *ft_memchr)
{
	typeof(memchr) *memchr_fun = (typeof(memchr) *)ft_memchr;
  it = "memchr";
  check(memchr_fun("abcd", 'z', 4) == NULL, 1);	/* Not found. */
  (void) strcpy(one, "abcd");
  check(memchr_fun(one, 'c', 4) == one+2, 2);	/* Basic test. */
  check(memchr_fun(one, ~0xff|'c', 4) == one+2, 2);	/* ignore highorder bits. */
  check(memchr_fun(one, 'd', 4) == one+3, 3);	/* End of string. */
  check(memchr_fun(one, 'a', 4) == one, 4);	/* Beginning. */
  check(memchr_fun(one, '\0', 5) == one+4, 5);	/* Finding NUL. */
  (void) strcpy(one, "ababa");
  check(memchr_fun(one, 'b', 5) == one+1, 6);	/* Finding first. */
  check(memchr_fun(one, 'b', 0) == NULL, 7);	/* Zero count. */
  check(memchr_fun(one, 'a', 1) == one, 8);	/* Singleton case. */
  (void) strcpy(one, "a\203b");
  check(memchr_fun(one, 0203, 3) == one+1, 9);	/* Unsignedness. */

  /* now test all possible alignment and length combinations to catch
     bugs due to unrolled loops (assuming unrolling is limited to no
     more than 128 byte chunks: */
  {
    char buf[128 + sizeof(long)];
    long align, len, i, pos;

    for (align = 0; align < (long) sizeof(long); ++align) {
      for (len = 0; len < (long) (sizeof(buf) - align); ++len) {
	for (i = 0; i < len; ++i) {
	  buf[align + i] = 'x';		/* don't depend on memset... */
	}
	for (pos = 0; pos < len; ++pos) {
#if 0
	  printf("align %d, len %d, pos %d\n", align, len, pos);
#endif
	  check(memchr_fun(buf + align, 'x', len) == buf + align + pos, 10);
	  check(memchr_fun(buf + align, 'x', pos) == NULL, 11);
	  buf[align + pos] = '-';
	}
      }
    }
  }
}


int
test_main_memchr (void*ft_memchr)
{
	INIT();
  size_t i;

	IMPL (ft_memchr, 1);

  test_init ();

  test_memchr(ft_memchr);

  for (i = 1; i < 8; ++i)
    {
      do_test (0, 16 << i, 2048, 23);
      do_test (i, 64, 256, 23);
      do_test (0, 16 << i, 2048, 0);
      do_test (i, 64, 256, 0);
    }
  for (i = 1; i < 32; ++i)
    {
      do_test (0, i, i + 1, 23);
      do_test (0, i, i + 1, 0);
    }

  do_random_tests ();
  return ret;
}

