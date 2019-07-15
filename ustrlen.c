#include "ustring.h"

int Ustrlen(const Uchar *s)
{
    register const Uchar *p;
    for (p=s; *p; p++)
	;
    return (int)(p - s);
}
