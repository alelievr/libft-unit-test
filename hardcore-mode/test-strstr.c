#include "utils.h"

typedef char *(*proto_t) (const char *, const char *);


static void
do_one_test (impl_t *impl, const char *s1, const char *s2, char *exp_result)
{
  char *result = CALL (impl, s1, s2);
  if (result != exp_result)
    {
      error (0, 0, "Wrong result in function %s %s %s", impl->name,
	     result, exp_result);
      ret = 1;
      return;
    }
}


static void
do_test (size_t align1, size_t align2, size_t len1, size_t len2,
	 int fail)
{
  char *s1 = (char *) (buf1 + align1);
  char *s2 = (char *) (buf2 + align2);

  static const char d[] = "1234567890abcdef";
#define dl (sizeof (d) - 1)
  char *ss2 = s2;
  for (size_t l = len2; l > 0; l = l > dl ? l - dl : 0)
    {
      size_t t = l > dl ? dl : l;
      ss2 = memcpy (ss2, d, t);
    }
  s2[len2] = '\0';

  if (fail)
    {
      char *ss1 = s1;
      for (size_t l = len1; l > 0; l = l > dl ? l - dl : 0)
	{
	  size_t t = l > dl ? dl : l;
	  memcpy (ss1, d, t);
	  ++ss1[len2 > 7 ? 7 : len2 - 1];
	  ss1 += t;
	}
    }
  else
    {
      memset (s1, '0', len1);
      memcpy (s1 + len1 - len2, s2, len2);
    }
  s1[len1] = '\0';

  FOR_EACH_IMPL (impl, 0)
    do_one_test (impl, s1, s2, fail ? NULL : s1 + len1 - len2);
}

static void
test_strstr (void *ft)
{
	typeof(strstr) *str = (typeof(strstr) *)ft;
  it = "strstr";
  check(str("abcd", "z") == NULL, 1);	/* Not found. */
  check(str("abcd", "abx") == NULL, 2);	/* Dead end. */
  (void) strcpy(one, "abcd");
  check(str(one, "c") == one+2, 3);	/* Basic test. */
  check(str(one, "bc") == one+1, 4);	/* Multichar. */
  check(str(one, "d") == one+3, 5);	/* End of string. */
  check(str(one, "cd") == one+2, 6);	/* Tail of string. */
  check(str(one, "abc") == one, 7);	/* Beginning. */
  check(str(one, "abcd") == one, 8);	/* Exact match. */
  check(str(one, "abcde") == NULL, 9);	/* Too long. */
  check(str(one, "de") == NULL, 10);	/* Past end. */
  check(str(one, "") == one, 11);	/* Finding empty. */
  (void) strcpy(one, "ababa");
  check(str(one, "ba") == one+1, 12);	/* Finding first. */
  (void) strcpy(one, "");
  check(str(one, "b") == NULL, 13);	/* Empty string. */
  check(str(one, "") == one, 14);	/* Empty in empty string. */
  (void) strcpy(one, "bcbca");
  check(str(one, "bca") == one+2, 15);	/* False start. */
  (void) strcpy(one, "bbbcabbca");
  check(str(one, "bbca") == one+1, 16);	/* With overlap. */
}

int
test_main_strstr (void *ft_strstr)
{
	INIT();

	IMPL (ft_strstr, 1);

	test_strstr(ft_strstr);

  test_init ();

  for (size_t klen = 2; klen < 32; ++klen)
    for (size_t hlen = 2 * klen; hlen < 16 * klen; hlen += klen)
      {
	do_test (0, 0, hlen, klen, 0);
	do_test (0, 0, hlen, klen, 1);
	do_test (0, 3, hlen, klen, 0);
	do_test (0, 3, hlen, klen, 1);
	do_test (0, 9, hlen, klen, 0);
	do_test (0, 9, hlen, klen, 1);
	do_test (0, 15, hlen, klen, 0);
	do_test (0, 15, hlen, klen, 1);

	do_test (3, 0, hlen, klen, 0);
	do_test (3, 0, hlen, klen, 1);
	do_test (3, 3, hlen, klen, 0);
	do_test (3, 3, hlen, klen, 1);
	do_test (3, 9, hlen, klen, 0);
	do_test (3, 9, hlen, klen, 1);
	do_test (3, 15, hlen, klen, 0);
	do_test (3, 15, hlen, klen, 1);

	do_test (9, 0, hlen, klen, 0);
	do_test (9, 0, hlen, klen, 1);
	do_test (9, 3, hlen, klen, 0);
	do_test (9, 3, hlen, klen, 1);
	do_test (9, 9, hlen, klen, 0);
	do_test (9, 9, hlen, klen, 1);
	do_test (9, 15, hlen, klen, 0);
	do_test (9, 15, hlen, klen, 1);

	do_test (15, 0, hlen, klen, 0);
	do_test (15, 0, hlen, klen, 1);
	do_test (15, 3, hlen, klen, 0);
	do_test (15, 3, hlen, klen, 1);
	do_test (15, 9, hlen, klen, 0);
	do_test (15, 9, hlen, klen, 1);
	do_test (15, 15, hlen, klen, 0);
	do_test (15, 15, hlen, klen, 1);
      }

  do_test (0, 0, page_size - 1, 16, 0);
  do_test (0, 0, page_size - 1, 16, 1);

  return ret;
}
