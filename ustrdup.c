#include "ustring.h"

Uchar *Ustrdup(const Uchar *s)
{
    register Uchar *p;
    register const Uchar *q;
    register int n;
    Uchar *a;

    for (q=s; *q; q++)
	;
    n = (int)(q - s);
    n++;
    n *= sizeof(Uchar);
    a = p = (Uchar *)malloc(n);
    q = s;
    while ((*p++ = *q++))
	;
    return a;
}
