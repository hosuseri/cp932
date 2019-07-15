#include "ustring.h"

Uchar *Ustrcpy(Uchar *d, const Uchar *s)
{
    register Uchar *p;
    register const Uchar *q;

    for (p=d, q=s; (*p++ = *q++); )
	;
    return d;
}

