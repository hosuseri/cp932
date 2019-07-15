#ifndef __CCACHE_H__
#define __CCACHE_H__

#define CCACHE_NROW 64 /* must be 2's power */
#define CCACHE_NCOL 2

typedef unsigned short CCACHE[CCACHE_NROW][CCACHE_NCOL*2];

void ccache_init(CCACHE cache);
int ccache_lookup(CCACHE cache, unsigned short key);
void ccache_install(CCACHE cache, unsigned short key, unsigned short val);

#endif
