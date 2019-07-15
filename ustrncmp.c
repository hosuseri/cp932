#include "ustring.h"

int Ustrncmp(const Uchar *s, const Uchar *t, int n)
{
    register int i;
    register const Uchar *p, *q;

    for (p=s, q=t, i=0; i < n && *p && *q; i++, p++, q++)
	if (*p != *q)
	    break;
    if (i >= n)
	return 0;
    return *p - *q;
}

