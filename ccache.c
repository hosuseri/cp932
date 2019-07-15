#if CCACHE_DEBUG
#include <stdio.h>
#endif

#include "ccache.h"

void ccache_init(CCACHE cache)
{
    int i, j;

    for (i=0; i < CCACHE_NROW; i++)
	for (j=0; j < CCACHE_NCOL * 2; j++)
	    cache[i][j] = 0;
}

int ccache_lookup(CCACHE cache, unsigned short key)
{
    int i;
    unsigned short *p;

    if (key == 0)
	return 0;
    p = cache[key&(CCACHE_NROW-1)];
    for (i = CCACHE_NCOL; i; i--, p++)
	if (*p++ == key)
	    break;
    if (i) {
#if CCACHE_DEBUG
	fprintf(stderr, "HIT!(0x%02X)", (int)key);
#endif
	return (int)*p;
    }
#if CCACHE_DEBUG
    fprintf(stderr, "SKA!(0x%02X)", (int)key);
#endif
    return -1;
}

void ccache_install(CCACHE cache, unsigned short key, unsigned short val)
{
    int i;
    unsigned short *p, *q;

    p = q = cache[key&(CCACHE_NROW-1)] +
	(CCACHE_NCOL * sizeof(unsigned short) << 1);
    p--;
    q -= 3;
    for (i = CCACHE_NCOL-1; i; i--) {
	*p-- = *q--;
	*p-- = *q--;
    }
    *q-- = val;
    *q = key;
}
