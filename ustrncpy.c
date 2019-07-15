#include "ustring.h"

Uchar *Ustrncpy(Uchar *d, const Uchar *s, int n)
{
    register int i;
    register Uchar *p;
    register const Uchar *q;

    for (p=d, q=s, i=0; i < n; i++)
	if (!(*p++ = *q++))
	    return d;
    if (i < n)
	*p = EOS;
    return d;
}

