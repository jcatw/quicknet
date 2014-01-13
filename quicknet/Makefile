#CC = gcc -Wall -Wstrict-prototypes -Wnested-externs -Wno-format
CC = gcc -Wall
#CFLAGS = -g -ggdb -lm
CFLAGS = -O3 -lm
LDFLAGS = -lm
DEFS = 

DEPEND = makedepend
DEPEND_FLAGS = -Y   # suppresses shared includes
DEPEND_DEFINES = 

srcdir = 
INCLUDES = -I$(srcdir)

SRCS = clparse.c heap.c model.c networknode.c price.c krapivsky.c
OBJS = clparse.o heap.o model.o networknode.o price.o krapivsky.o
EXE = price krapivsky

HDRS = clparse.h heap.h model.h networknode.h util.h

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

#ewwww
$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(filter-out $(addsuffix .o,$(filter-out $@,$(EXE))),$(OBJS)) $(LIBS)
# DO NOT DELETE: updated by make depend

clparse.o: clparse.h
heap.o: networknode.h heap.h
model.o: networknode.h heap.h model.h
networknode.o: networknode.h
price.o: clparse.h util.h networknode.h heap.h model.h
krapivsky.o: clparse.h util.h networknode.h heap.h model.h
