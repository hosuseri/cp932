#include "ustring.h"

Uchar *Ustrcat(Uchar *s, const Uchar *t)
{
    register Uchar *p;
    register const Uchar *q;

    for (p=s; *p; p++)
	;
    for (q=t; (*p++ = *q++);)
	;
    return s;
}
