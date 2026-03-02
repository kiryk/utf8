#include "utf8.h"

/* number of bytes in UTF-8 code started by given byte */
#define UTF8Size(b) ( \
	((b) & 0x80) == 0x00? 1 : \
	((b) & 0xe0) == 0xc0? 2 : \
	((b) & 0xf0) == 0xe0? 3 : \
	((b) & 0xf8) == 0xf0? 4 : -1)

/* number of bytes needed in UTF-8 to encode given rune */
#define RuneSize(r) ( \
	(r) < 0x80?     1 : \
	(r) < 0x800?    2 : \
	(r) < 0x10000?  3 : \
	(r) < 0x110000? 4 : -1)

/* decode the first rune in UTF-8 string */
Rune utf8_decode(const char *s)
{
	int i, n = UTF8Size(*s);
	Rune rune;

	/* read value from the header */
	if (n == 1)
		rune = *s & 0x7f;
	else if (n > 1)
		rune = *s & (0x3f >> n);
	else
		return -1;

	/* read values from the extra bytes */
	for (i = 1; i < n; i++) {
		if ((*++s & 0xc0) != 0x80)
			return -1;
		rune = (rune << 6) | (*s & 0x3f);
	}

	/* check if not overlong */
	if (RuneSize(rune) < UTF8Size(*s))
		rune = 0xfffd; /* decode as malformed */
	return rune;
}

/* encode rune as UTF-8 string */
char *utf8_encode(char *s, Rune rune)
{
	int n = RuneSize(rune);
	int i;

	if (n <= 0) {
		n = 0;
	} else if (n == 1) {
		s[0] = rune;
	} else {
		for (i = n-1; i > 0; i--) {
			s[i] = 0x80 | (rune & 0x3f); /* 0x10XXXXXX */
			rune >>= 6;
		}
		s[0] = 0;
		for (i = 0; i < n; i++)
			s[0] = (s[0] >> 1) | 0x80;
		s[0] |= (rune & 0xff);
	}

	s[n] = '\0';
	return s;
}

/* find next rune in UTF-8 string, never jump over NUL */
char *utf8_next(char *s)
{
	char *t = s+UTF8Size(*s);

	while (*s && s < t)
		s++;
	return s;
}

/* find length of UTF-8 string */
int utf8_len(char *s)
{
	int i = 0;

	for (; *s; s = utf8_next(s))
		i++;
	return i;
}

/* compare UTF-8 strings */
int utf8_cmp(char *s, char *t)
{
	Rune rs = 0, rt = 0;

	while (*s && *t) {
		rs = utf8_decode(s);
		rt = utf8_decode(t);
		if (rs != rt)
			break;
		s = utf8_next(s);
		t = utf8_next(t);
	}
	return rs - rt;
}
