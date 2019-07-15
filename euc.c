#include <stdlib.h>
#if EUC_DEBUG
#include <stdio.h>
#endif
#include <string.h>
#include "ustring.h"

#if USE_CACHE
#include "ccache.h"
#endif

#define EUC_SS2	0x8E

#if USE_CACHE
static int inited = 0;
static CCACHE cache, icache;
#endif

int eucstr_valid_p(const Lchar *s)
{
    int c, i;
    const Lchar *p;

    for (i=0, p=s; (c=*p); p++, i++) {
	if (!(c & 0x80))
	    continue;
	if (c == EUC_SS2) {
	    c = *p++;
	    if (c < 0xA0 || c > 0xDF)
		return -i;
	} else if (c < 0x81 || c > 0xFE)
	    return -i;
	c = *p++;
	if (c < 0x81 || c > 0xFE)
	    return -i;
    }
    return i;
}

int eucstr_store(const Uchar *s, Lchar *d, int n)
{
    int c, i;
    Lchar *p;

    for (p=d, i=0; i < n && (c = *s++); i++) {
	c = utf16_to_euc(c);
	if (c & 0x8000) {
	    *p++ = c >> 8;
	    if (++i >= n) {
		p--;
		i--;
		break;
	    }
	    c &= 0xFF;
	}
	*p++ = c;
    }
    if (i < n)
	*p = EOS;
    return i;
}

Uchar *eucstr_load(const Lchar *s)
{
    int n;
    Uchar *d;

    if ((n = eucstr_valid_p(s)) <= 0)
	if (n || *s) {
	    s = (const Lchar *)"*invalid-euc-string*";
	    n = strlen((const char *)s);
	}
    n++;
    n <<= 1;
    if ((d = (Uchar *)malloc(n)) == NULL)
	return NULL;
    eucstr_nload(s, d, &n);
    return d;
}

Lchar *eucstr_nload(const Lchar *s, Uchar *d, int *n)
{
    int c, i, k;
    Uchar *p;
    const Lchar *q;
    
    k = *n;
    k >>= 1;
    for (p=d, q=s, i=0; i < k; i++) {
	if (!(c = *q))
	    break;
	q++;
	if (c & 0x80) {
	    c <<= 8;
	    c |= *q++;
	}
	c = euc_to_utf16(c);
	*p++ = c;
    }
    if (i < k)
	*p++ = EOS;
    *n = i << 1;
    return (Lchar *)q;
}

int euc_to_utf16(Uchar c)
{
    Lchar d, e, h, l;
    int t;

    d = c >> 8;
    e = c & 0xFF;
    if (!d || d == EUC_SS2)
	return e;

#if USE_CACHE
    if (!inited) {
	ccache_init(cache);
	ccache_init(icache);
	inited++;
    }
    if ((t = ccache_lookup(cache, c)) > 0)
	return t;
#endif

    d &= 0x7F;
    e &= 0x7F;
    h = ((d - 0x21)>>1) + 0x81;
    if (d >= 0x5F)
	h += 0xE0 - 0x9F - 1;
    if (d & 1) {
	l = 0x40;
	if (e >= 0x60)
	    l++;
    } else
	l = 0x9F;
    l += e - 0x21;
    t = (int)(h << 8) | l;

    t = cp932_to_utf16(t);

#if USE_CACHE
    ccache_install(cache, c, t);
#endif

    return t;
}

int utf16_to_euc(Uchar c)
{
    Lchar d, e, h, l;
    int t;

    if (c <= 0x7F)
	return c;

#if USE_CACHE
    if (!inited) {
	ccache_init(cache);
	ccache_init(icache);
	inited++;
    }
    if ((t = ccache_lookup(icache, c)) > 0)
	return t;
#endif

    t = utf16_to_cp932(c);

    d = t >> 8;
    e = t & 0xFF;
    if (!d) {
	if (e >= 0xA0 && e <= 0xDF)
	    return (EUC_SS2 << 8) | e;
	if (e <= 0x7F)
	    return e;
    }
    if (d >= 0xE0 && e < 0xEF)
	d -= 0xE0 - 0x9F - 1;
    h = ((d - 0x81) << 1) + 0x21;
    if (e < 0x40 || e > 0x7E)
	if (e >= 0x80 && e <= 0xFC)
	    e--;
    if (e >= 0x9E) {
	h++;
	e -= 0x9E - 0x40;
    }
    l = e - 0x40 + 0x21;

    t = (int)(h << 8) | l | 0x8080;

#if USE_CACHE
    ccache_install(icache, c, t);
#endif

    return t;
}

#if EUC_DEBUG
int main(int argc, char *argv[])
{
    int c, i, t;
    Uchar *p, *s;
    Lchar *q;
    static Lchar buf[512];

    if (argc < 2)
	goto pattern1;
    if (!strcmp(argv[1], "1"))
	goto pattern1;
    if (!strcmp(argv[1], "2"))
	goto pattern2;
    if (!strcmp(argv[1], "3"))
	goto pattern3;
    if (!strcmp(argv[1], "4"))
	goto pattern4;

 pattern1:
    putchar(0xFE);
    putchar(0xFF);
    while ((c = getchar()) != EOF) {
	if (c & 0x80) {
	    if ((t = getchar()) == EOF)
		break;
	    c <<= 8;
	    c |= t;
	}
	t = euc_to_utf16(c);
	c = t >> 8;
	putchar(c);
	c = t & 0xFF;
	putchar(c);
    }
    return 0;

 pattern2:
    if ((c = getchar()) != 0xFE)
	return 1;
    if ((c = getchar()) != 0xFF)
	return 1;
    while ((c = getchar()) != EOF) {
	if ((t = getchar()) == EOF)
	    break;
	c <<= 8;
	c |= t;
	t = utf16_to_euc(c);
	c = t >> 8;
	if (c)
	    putchar(c);
	c = t & 0xFF;
	putchar(c);
    }
    return 0;

 pattern3:
    s = eucstr_load("hello, world\n");
    for (p=s; *p; p++) {
	c = *p >> 8;
	putchar(c);
	c = *p & 0xFF;
	putchar(c);
    }
    s = eucstr_load("みなさん、お元気ですか?\n");
    for (p=s; *p; p++) {
	c = *p & 0xFF;
	putchar(c);
	c = *p >> 8;
	putchar(c);
    }
    free(s);
    return 0;

 pattern4:
    s = eucstr_load("みなさん、Everybody, お元気ですか？ How are you?\n");
    eucstr_store(s, buf, 512);
    for (q=buf, i=0; i < 512 && (c = *q++); i++)
	putchar(c);
    free(s);
    return 0;
}
#endif
