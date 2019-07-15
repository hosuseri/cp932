#!/bin/sh

hfname=cp932lut.h.auto
cfname=cp932lut.c.auto
cp932lut=cp932lut.$$
utf16lut=utf16lut.$$
awk '
NF < 2 { next; }
$1 ~ /0x[0-7][0-9A-F]$/ { next; }
$1 ~ /0x[0-9A-F]+/ && $2 ~ /0x[0-9A-F]+/ {
    printf "%s\t%s\n", $1, $2;
}
' CP932.TXT >$cp932lut

awk '
NF < 2 { next; }
$1 ~ /0x[0-7][0-9A-F]$/ { next; }
$1 ~ /0x[0-9A-F]+/ && $2 ~ /0x[0-9A-F]+/ {
    printf "%s\t%s\n", $1, $2;
}
' CP932.TXT | sort +1 +0 | uniq -f1 >$utf16lut

cat <<EOF >$hfname
#ifndef __CP932LUT_H__
#define __CP932LUT_H__

EOF
set `wc $cp932lut`
echo \#define CP932_NMEMB $1 >>$hfname
set `wc $utf16lut`
echo \#define UTF16_NMEMB $1 >>$hfname
cat <<EOF >>$hfname

extern unsigned short cp932_lut[];
extern unsigned short cp932_i_lut[];

#endif
EOF

cat <<EOF >$cfname
unsigned short cp932_lut[] = {
EOF

awk '
NR > 1 { printf ",\n"; }
{ printf "    %s, %s", $1, $2; }
' $cp932lut >>$cfname

cat <<EOF >>$cfname

};

unsigned short cp932_i_lut[] = {
EOF

awk '
NR > 1 { printf ",\n"; }
{ printf "    %s, %s", $2, $1; }
' $utf16lut >>$cfname

cat <<EOF >>$cfname

};
EOF

rm $cp932lut $utf16lut
