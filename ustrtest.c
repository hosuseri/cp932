#include <stdio.h>
#include <string.h>
#include "ustring.h"

int main()
{
    int n;
    Uchar *s, *t, *p;
    static Uchar b[2048];
    static Lchar cb[2048];

    s = eucstr_load((const Lchar *)"あいうえお");
    t = eucstr_load((const Lchar *)"abc");
    p = Ustrcpy(b, s);
    p = Ustrcat(p, t);
    n = Ustrlen(p);
    printf("%d\n", n);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    n = Ustrncmp(s, p, 5);
    printf("%d\n", n);
    n = Ustrncmp(s, p, 4);
    printf("%d\n", n);
    n = Ustrncmp(s, s, 5);
    printf("%d\n", n);
    n = Ustrncmp(s, p, 6);
    printf("%d\n", n);
    *p = EOS;
    n = Ustrlen(p);
    printf("%d\n", n);
    n = Ustrcmp(s, t);
    printf("%d\n", n);
    n = Ustrcmp(t, s);
    printf("%d\n", n);
    n = Ustrcmp(t, t);
    printf("%d\n", n);
    p = Ustrdup(s);
    n = Ustrcmp(p, s);
    printf("%d\n", n);
    n = Ustrcmp(p, t);
    printf("%d\n", n);
    Ufree(s);
    s = cp932str_load((const Lchar *)"\x82\xA9\x82\xAB\x82\xAD\x82\xAF\x82\xB1");
    Ustrcpy(b, s);
    Ustrcat(b, t);
    Ustrcat(b, p);
    Ustrcat(b, s);
    Ustrcat(b, t);
    eucstr_store(b, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    Ufree(p);
    memset(b, EOS, sizeof(b));
    p = Ustrncpy(b, s, 3);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    p = Ustrncpy(b, s, 4);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    p = Ustrncpy(b, s, 5);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    p = Ustrncpy(b, s, 6);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    p = Ustrncat(b, s, 3);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    p = Ustrncat(b, s, 5);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    p = Ustrncat(b, s, 6);
    eucstr_store(p, cb, sizeof(cb));
    printf("%s\n", (const char *)cb);
    Ufree(s);
    Ufree(t);
    printf("END\n");

    return 0;
}
