typedef unsigned int Rune;

Rune utf8_decode(const char *s);
char *utf8_encode(char *s, Rune rune);
char *utf8_next(char *s);
int utf8_len(char *s);
int utf8_cmp(char *s, char *t);
