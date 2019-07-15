#include "ustring.h"

Uchar *Ustrchr(const Uchar *s, Uchar c)
{
    register const Uchar *p;

    for (p=s; *p; p++)
	if (*p == c)
	    return (Uchar *)p;
    return NULL;
}

