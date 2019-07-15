#if CP932_DEBUG
#include <stdio.h>
#endif
#include <string.h>
#include "ustring.h"
#include "cp932lut.h"
#if USE_CACHE
#include "ccache.h"
#endif

#if USE_CACHE
static int inited = 0;
static CCACHE cache, icache;
#endif

int cp932str_valid_p(const Lchar *s)
{
    int c, i;
    const Lchar *p;

    for (i=0, p=s; (c=*p); p++, i++) {
	if (!(c & 0x80))
	    continue;
	if (c >= 0xA0 && c <= 0xDF)
	    continue;
	if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) {
	    c = *++p;
	    if ((c < 0x40 || c > 0x7E) && (c < 0x80 || c > 0xFC))
		return -i;
	    i++;
	    continue;
	}
	return -i;
    }
    i++;
    return i;
}

int cp932str_store(const Uchar *s, Lchar *d, int n)
{
    int c, i;
    Lchar *p;

    for (p=d, i=0; i < n && (c = *s++); i++) {
	c = utf16_to_cp932(c);
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

Uchar *cp932str_load(const Lchar *s)
{
    int n;
    Uchar *d;

    if ((n = cp932str_valid_p(s)) <= 0)
	if (n || *s) {
	    s = (const Lchar *)"*invalid-shift-jis-string*";
	    n = strlen((const char *)s);
	}
    n++;
    n <<= 1;
    if ((d = (Uchar *)malloc(n)) == NULL)
	return NULL;
    cp932str_nload(s, d, &n);
    return d;
}

Lchar *cp932str_nload(const Lchar *s, Uchar *d, int *n)
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
	if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) {
	    c <<= 8;
	    c |= *q++;
	}
	c = cp932_to_utf16(c);
	*p++ = c;
    }
    if (i < k)
	*p++ = EOS;
    *n = i << 1;
    return (Lchar *)q;
}

static int compar(const void *p, const void *q)
{
    return (int)*(Uchar *)p - *(Uchar *)q;
}

Uchar cp932_to_utf16(Uchar c)
{
#if USE_CACHE
    int t;
#endif

    Uchar *p;

    if (c <= 0x7F)
	return c;

#if USE_CACHE
    if (!inited) {
	ccache_init(cache);
	ccache_init(icache);
	inited++;
    }
    if ((t = ccache_lookup(cache, c)) > 0)
	return t;
#endif

    p = (Uchar *)bsearch(&c,cp932_lut,CP932_NMEMB,sizeof(Uchar)<<1,compar);
    if (p == NULL)
	return 0;

    p++;

#if USE_CACHE
    ccache_install(cache, c, *p);
#endif

    return *p;
}

Uchar utf16_to_cp932(Uchar c)
{
#if USE_CACHE
    int t;
#endif

    Uchar *p;

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

    p = (Uchar *)bsearch(&c,cp932_i_lut,UTF16_NMEMB,sizeof(Uchar)<<1,compar);
    if (p == NULL)
	return 0;

    p++;

#if USE_CACHE
    ccache_install(icache, c, *p);
#endif

    return *p;
}

#if CP932_DEBUG
int main(int argc, char *argv[])
{
    int c, i, n, t;
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
    if (!strcmp(argv[1], "9"))
	goto pattern9;

 pattern1:
    for (n=i=0; i < CP932_NMEMB; i++) {
        c = cp932_to_utf16(cp932_lut[i*2]);
        if (c != cp932_lut[i*2+1])
	    break;
        c = utf16_to_cp932(c);
        if (c != cp932_lut[i*2]) {
	    n++;
	    fprintf(stderr, "0x%04X: dup.\n", cp932_lut[i*2+1]);
	}
    }
    if (i >= CP932_NMEMB && i-n == UTF16_NMEMB)
	fprintf(stderr, "OK\n");
    else
	fprintf(stderr, "NG\n");
    if (n > 0) {
	fprintf(stderr, "%d chars are duplicated.\n", n);
    }

    putchar(0xFE);
    putchar(0xFF);
    while ((c = getchar()) != EOF) {
	if ((c >= 0x81 && c <= 0x9f) || (c >= 0xE0 && c <= 0xEF)) {
	    if ((t = getchar()) == EOF)
		break;
	    c <<= 8;
	    c |= t;
	}
	c = cp932_to_utf16(c);
	t = (c >> 8) & 0xFF;
	putchar(t);
	t = c & 0xFF;
	putchar(t);
    }

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
	t = utf16_to_cp932(c);
	c = t >> 8;
	if (c)
	    putchar(c);
	c = t & 0xFF;
	putchar(c);
    }
    return 0;

 pattern3:
    s = cp932str_load("hello, world\n");
    for (p=s; *p; p++) {
	c = *p >> 8;
	putchar(c);
	c = *p & 0xFF;
	putchar(c);
    }
    s = cp932str_load("\x82\xDD\x82\xC8\x82\xB3\x82\xF1\x81\x41\x82\xA8\x8C\xB3\x8B\x43\x82\xC5\x82\xB7\x82\xA9?\n");
    for (p=s; *p; p++) {
	c = *p >> 8;
	putchar(c);
	c = *p & 0xFF;
	putchar(c);
    }
    free(s);
    return 0;

 pattern4:
    s = cp932str_load("\x82\xDD\x82\xC8\x82\xB3\x82\xF1\x81\x41""Everybody,\x82\xA8\x8C\xB3\x8B\x43\x82\xC5\x82\xB7\x82\xA9\x81\x48 How are you?\n");
    cp932str_store(s, buf, 512);
    for (q=buf, i=0; i < 512 && (c = *q++); i++)
	putchar(c);
    free(s);
    return 0;

 pattern9:
    putchar(0xFF);
    putchar(0xFE);
    while ((c = getchar()) != EOF) {
	if ((c >= 0x81 && c <= 0x9f) || (c >= 0xE0 && c <= 0xEF)) {
	    if ((t = getchar()) == EOF)
		break;
	    c <<= 8;
	    c |= t;
	}
	c = cp932_to_utf16(c);
	t = c & 0xFF;
	putchar(t);
	t = (c >> 8) & 0xFF;
	putchar(t);
    }
    return 0;
}
#endif
