#CC = gcc -Wall -Wstrict-prototypes -Wnested-externs -Wno-format
CC = gcc -Wall
CFLAGS = -g -ggdb -lm
LDFLAGS = -lm
DEFS = 

DEPEND = makedepend
DEPEND_FLAGS = -Y   # suppresses shared includes
DEPEND_DEFINES = 

srcdir = 
INCLUDES = -I$(srcdir)

SRCS = heap.c model.c networknode.c price.c
OBJS = heap.o model.o networknode.o price.o
EXE = price

HDRS = heap.h model.h networknode.h util.h

all: ${EXE}

clean:
	rm -f *.o a.out core ${EXE}

depend:
	${DEPEND} -s '# DO NOT DELETE: updated by make depend'		   \
	$(DEPEND_FLAGS) -- $(INCLUDES) $(DEFS) $(DEPEND_DEFINES) $(CFLAGS) \
	-- ${SRCS}

TAGS: tags
tags:
	find $(srcdir) -name '*.[chly]' -print | xargs etags -a

.c.o:
	$(CC) -c $(INCLUDES) $(DEFS) $(CFLAGS) $<

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
# DO NOT DELETE: updated by make depend

heap.o: heap.h
model.o: heap.h model.h
networknode.o: networknode.h
price.o: util.h model.h
