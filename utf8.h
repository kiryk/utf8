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

typedef unsigned int Rune;

Rune utf8_decode(const char *s);
char *utf8_encode(char *s, Rune rune);
char *utf8_next(char *s);
int utf8_len(char *s);
int utf8_cmp(char *s, char *t);
