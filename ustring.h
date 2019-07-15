#ifndef __USTRING_H__
#define __USTRING_H__

#include <stdlib.h>

#ifndef YES
#define YES	1
#endif
#ifndef NO
#define NO	0
#endif
#ifndef EOS
#define EOS	0
#endif

#define BLANK	' '

#define Ualloc(x) malloc(x)
#define Urealloc(x, y) realloc((x), (y))
#define Ufree(x) free((void *)(x))

typedef unsigned short Uchar;
typedef unsigned char Lchar;

int cp932str_valid_p(const Lchar *s);
int cp932str_store(const Uchar *s, Lchar *d, int n);
Uchar *cp932str_load(const Lchar *s);
Lchar *cp932str_nload(const Lchar *s, Uchar *d, int *n);
Uchar cp932_to_utf16(Uchar c);
Uchar utf16_to_cp932(Uchar c);

int eucstr_valid_p(const Lchar *s);
int eucstr_store(const Uchar *s, Lchar *d, int n);
Uchar *eucstr_load(const Lchar *s);
Lchar *eucstr_nload(const Lchar *s, Uchar *d, int *n);

int euc_to_utf16(Uchar c);
int utf16_to_euc(Uchar c);
int Ustrcmp(const Uchar *s, const Uchar *t);
int Ustrncmp(const Uchar *s, const Uchar *t, int n);
Uchar *Ustrcpy(Uchar *d, const Uchar *s);
Uchar *Ustrncpy(Uchar *d, const Uchar *s, int n);
int Ustrlen(const Uchar *s);
Uchar *Ustrcat(Uchar *s, const Uchar *t);
Uchar *Ustrncat(Uchar *s, const Uchar *t, int n);
Uchar *Ustrdup(const Uchar *s);
Uchar *Ustrchr(const Uchar *s, Uchar c);

#endif
