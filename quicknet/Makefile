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

SRCS = clparse.c hash.c heap.c model.c networknode.c price.c pricerecip.c krapivsky.c krapivskyrecip.c krapivskysc.c scc.c
OBJS = clparse.o hash.o heap.o model.o networknode.o price.o pricerecip.o krapivsky.o krapivskyrecip.o krapivskysc.o scc.o
EXE = price pricerecip krapivsky krapivskyrecip krapivskysc

HDRS = clparse.h hash.h heap.h model.h networknode.h scc.h types.h util.h

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

clparse.o: clparse.h types.h
hash.o: hash.h types.h
heap.o: networknode.h types.h heap.h hash.h
model.o: networknode.h types.h heap.h model.h
networknode.o: networknode.h types.h
price.o: clparse.h types.h util.h networknode.h heap.h model.h
pricerecip.o: clparse.h types.h util.h networknode.h heap.h model.h
krapivsky.o: clparse.h types.h util.h networknode.h heap.h model.h
krapivskyrecip.o: clparse.h types.h util.h networknode.h heap.h model.h
krapivskyrecip.o: hash.h
krapivskysc.o: clparse.h types.h util.h networknode.h heap.h model.h scc.h
scc.o: networknode.h types.h model.h scc.h
