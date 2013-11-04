#CC = gcc -Wall -Wstrict-prototypes -Wnested-externs -Wno-format
CC = gcc -Wall
CFLAGS = -g
DEFS = 

DEPEND = makedepend
DEPEND_FLAGS = 
DEPEND_DEFINES = 

srcdir = 
INCLUDES = -I$(srcdir)

SRCS = bstreap.c krapivsky.c krapivskymain.c networknode.c
OBJS = bstreap.o krapivsky.o krapivskymain.o networknode.o
EXE = quicknet

HDRS = bstreap.h krapivsky.h networknode.h

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

bstreap.o: bstreap.h networknode.h
krapivsky.o: /usr/include/stdlib.h /usr/include/Availability.h
krapivsky.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
krapivsky.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
krapivsky.o: /usr/include/sys/_symbol_aliasing.h
krapivsky.o: /usr/include/sys/_posix_availability.h
krapivsky.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
krapivsky.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
krapivsky.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
krapivsky.o: /usr/include/i386/signal.h /usr/include/i386/_structs.h
krapivsky.o: /usr/include/sys/_structs.h /usr/include/machine/_structs.h
krapivsky.o: /usr/include/sys/resource.h /usr/include/machine/endian.h
krapivsky.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
krapivsky.o: /usr/include/libkern/_OSByteOrder.h
krapivsky.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
krapivsky.o: /usr/include/machine/types.h /usr/include/i386/types.h
krapivsky.o: /usr/include/stdint.h krapivsky.h networknode.h bstreap.h
krapivskymain.o: networknode.h bstreap.h krapivsky.h
networknode.o: /usr/include/stdlib.h /usr/include/Availability.h
networknode.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
networknode.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
networknode.o: /usr/include/sys/_symbol_aliasing.h
networknode.o: /usr/include/sys/_posix_availability.h
networknode.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
networknode.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
networknode.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
networknode.o: /usr/include/i386/signal.h /usr/include/i386/_structs.h
networknode.o: /usr/include/sys/_structs.h /usr/include/machine/_structs.h
networknode.o: /usr/include/sys/resource.h /usr/include/machine/endian.h
networknode.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
networknode.o: /usr/include/libkern/_OSByteOrder.h
networknode.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
networknode.o: /usr/include/machine/types.h /usr/include/i386/types.h
networknode.o: /usr/include/stdint.h networknode.h
