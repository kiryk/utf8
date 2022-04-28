#include <stdio.h>

/* errors: too long repr */

/* calculate the number of extra bytes */
#define Extra(ch) ( \
  ((ch) & 0x80) == 0x00? 0 : \
  ((ch) & 0xe0) == 0xc0? 1 : \
  ((ch) & 0xa0) == 0xe0? 2 : \
  ((ch) & 0xf8) == 0xf0? 3 : \
  ((ch) & 0xfc) == 0xf8? 4 : \
  ((ch) & 0xfe) == 0xfc? 5 : -1)


/* calculate the number of bytes needed to encode a character */
#define Bytes(ch) ( \
  (ch) <= 0x7f?       1 : \
  (ch) <= 0x7ff?      2 : \
  (ch) <= 0xffff?     3 : \
  (ch) <= 0x1fffff?   4 : \
  (ch) <= 0x3ffffff?  5 : \
  (ch) <= 0x7fffffff? 6 : -1)


int utf8_to_int(char *s)
{
  int n = Extra(*s);
  int ch;

  /* read value from the header */
  if (n == 0)
    ch = *s & 0x7f;
  else if (n > 0)
    ch = *s & (0x1f >> n);
  else
    return -1;

  /* read values from the extra bytes */
  while (n-- > 0) {
    if ((*++s & 0xc0) != 0x80)
      return -1;
    ch = (ch << 6) | *s & 0x3f;
  }

  return ch;
}

int utf8_from_int(char *s, int ch)
{
  int n = Bytes(ch);
  int i;

  if (n == 0)
    s[0] = ch;
  else if (n > 0)
    s[0] = 


  s[n] = '\0';
}

int main()
{
  char s[10];

  while (scanf("%s", s) >= 0)
    printf("code: %d\n", utf8_to_int(s));

  return 0;
}
