#include "ustring.h"

int Ustrcmp(const Uchar *s, const Uchar *t)
{
    register const Uchar *p, *q;

    for (p=s, q=t; *p && *q; p++, q++)
	if (*p != *q)
	    break;
    return *p - *q;
}

