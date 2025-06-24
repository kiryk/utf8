/* number of bytes following the first byte in a UTF-8 char */
#define UTF8Extra(h) ( \
	((h) & 0x80) == 0x00? 0 : \
	((h) & 0xe0) == 0xc0? 1 : \
	((h) & 0xf0) == 0xe0? 2 : \
	((h) & 0xf8) == 0xf0? 3 : \
	((h) & 0xfc) == 0xf8? 4 : \
	((h) & 0xfe) == 0xfc? 5 : -1)

/* number of bytes needed to encode given unicode in UTF-8 */
#define UTF8Bytes(r) ( \
	(r) <= 0x7f?       1 : \
	(r) <= 0x7ff?      2 : \
	(r) <= 0xffff?     3 : \
	(r) <= 0x1fffff?   4 : \
	(r) <= 0x3ffffff?  5 : \
	(r) <= 0x7fffffff? 6 : -1)

/* decodes the first UTF-8 char in the string */
int utf8_to_int(const char *s)
{
	int n = UTF8Extra(*s);
	int rune;

	/* read value from the header */
	if (n == 0)
		rune = *s & 0x7f;
	else if (n > 0)
		rune = *s & (0x1f >> n);
	else
		return -1;

	/* read values from the extra bytes */
	while (n-- > 0) {
		if ((*++s & 0xc0) != 0x80)
			return -1;
		rune = (rune << 6) | (*s & 0x3f);
	}

	return rune;
}

/* encodes unicode char as a UTF-8 string */
char *utf8_from_int(char *s, int rune)
{
	int n = UTF8Bytes(rune);
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

/* decodes a UTF-8 string into an array of unicode ids */
void utf8_decode(int *runes, char *s)
{
	int i, j;

	for (i = j = 0; s[i]; i += 1+UTF8Extra(s[i]), j++)
		runes[j] = utf8_to_int(s+i);
	runes[j] = 0;
}

/* compares two UTF-8 strings */
int utf8_cmp(char *a, char *b)
{
	int ra = 0, rb = 0;

	while (*a && *b) {
		ra = utf8_to_int(a);
		rb = utf8_to_int(b);
		if (ra != rb)
			return ra - rb;
		a += 1+UTF8Extra(*a);
		b += 1+UTF8Extra(*b);
	}

	return *(unsigned char*)a - *(unsigned char*)b;
}
