TARGET=libustr.a
CC=gcc
CFLAGS=-Wall -O2 -DUSE_CCACHE=0 -ggdb
OBJS=\
ccache.o \
cp932.o \
cp932lut.o \
euc.o \
ustrcat.o \
ustrncat.o \
ustrcmp.o \
ustrncmp.o \
ustrcpy.o \
ustrncpy.o \
ustrdup.o \
ustrlen.o \
ustrchr.o

all: $(TARGET) ustrtest

clean:
	rm -f $(TARGET) $(OBJS) ustrtest ustrtest.o

$(TARGET): $(OBJS)
	ar ru $(TARGET) $(OBJS)
	ranlib $(TARGET)

ustrtest: ustrtest.o $(TARGET)
	$(CC) -L . -o ustrtest ustrtest.o -lustr

ustrtest.o: ustrtest.c ustring.h

ccache.o: ccache.c ccache.h

cp932.o: cp932.c ustring.h

cp932lut.o: cp932lut.c

euc.o: euc.c ustring.h

ustrcat.o: ustrcat.c ustring.h

ustrncat.o: ustrncat.c ustring.h

ustrcmp.o: ustrcmp.c ustring.h

ustrncmp.o: ustrncmp.c ustring.h

ustrcpy.o: ustrcpy.c ustring.h

ustrncpy.o: ustrncpy.c ustring.h

ustrdup.o: ustrdup.c ustring.h

ustrlen.o: ustrlen.c ustring.h

ustrchr.o: ustrchr.c ustring.h
