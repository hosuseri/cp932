#include "ustring.h"

Uchar *Ustrncat(Uchar *s, const Uchar *t, int n)
{
    register int i;
    register Uchar *p;
    register const Uchar *q;

    for (p=s; *p; p++)
	;
    for (q=t, i=0; i < n; i++)
	if (!(*p++ = *q++))
	    return s;
    if (i < n)
	*p = EOS;
    return s;
}
