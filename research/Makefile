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

SRCS = alphas.c bstreap.c heap.c krapivsky.c krapivskymain.c networknode.c quickmath.c
OBJS = alphas.o bstreap.o heap.o krapivsky.o krapivskymain.o networknode.o quickmath.o
EXE = quicknet

HDRS = alphas.h bstreap.h heap.h krapivsky.h networknode.h quickmath.h

#vpath %.o build/objects/c
#vpath %.cc src/c
#vpath %.hh src/c

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

alphas.o: alphas.h networknode.h heap.h krapivsky.h
bstreap.o: networknode.h bstreap.h
heap.o: networknode.h heap.h
krapivsky.o: networknode.h heap.h bstreap.h krapivsky.h quickmath.h
krapivskymain.o: networknode.h bstreap.h heap.h krapivsky.h quickmath.h
networknode.o: networknode.h
quickmath.o: quickmath.h
