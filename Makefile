#CC = gcc -Wall -Wstrict-prototypes -Wnested-externs -Wno-format
CC = gcc -Wall
CFLAGS = -g -ggdb
DEFS = 

DEPEND = makedepend
DEPEND_FLAGS = 
DEPEND_DEFINES = 

srcdir = 
INCLUDES = -I$(srcdir)

SRCS = bstreap.c heap.c krapivsky.c krapivskymain.c networknode.c 
OBJS = bstreap.o heap.o krapivsky.o krapivskymain.o networknode.o 
EXE = quicknet

HDRS = bstreap.h heap.h krapivsky.h networknode.h

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

bstreap.o: /usr/include/stdlib.h /usr/include/Availability.h
bstreap.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
bstreap.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
bstreap.o: /usr/include/sys/_symbol_aliasing.h
bstreap.o: /usr/include/sys/_posix_availability.h
bstreap.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
bstreap.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
bstreap.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
bstreap.o: /usr/include/i386/signal.h /usr/include/i386/_structs.h
bstreap.o: /usr/include/sys/_structs.h /usr/include/machine/_structs.h
bstreap.o: /usr/include/sys/resource.h /usr/include/machine/endian.h
bstreap.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
bstreap.o: /usr/include/libkern/_OSByteOrder.h
bstreap.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
bstreap.o: /usr/include/machine/types.h /usr/include/i386/types.h
bstreap.o: /usr/include/stdint.h /usr/include/stdio.h
bstreap.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
bstreap.o: networknode.h bstreap.h
heap.o: /usr/include/stdlib.h /usr/include/Availability.h
heap.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
heap.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
heap.o: /usr/include/sys/_symbol_aliasing.h
heap.o: /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h
heap.o: /usr/include/i386/_types.h /usr/include/sys/wait.h
heap.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
heap.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
heap.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
heap.o: /usr/include/machine/_structs.h /usr/include/sys/resource.h
heap.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
heap.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
heap.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
heap.o: /usr/include/machine/types.h /usr/include/i386/types.h
heap.o: /usr/include/stdio.h /usr/include/secure/_stdio.h
heap.o: /usr/include/secure/_common.h heap.h
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
krapivsky.o: /usr/include/stdint.h /usr/include/stdio.h
krapivsky.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
krapivsky.o: /usr/include/string.h /usr/include/strings.h
krapivsky.o: /usr/include/secure/_string.h networknode.h heap.h bstreap.h
krapivsky.o: krapivsky.h
krapivskymain.o: /usr/include/stdlib.h /usr/include/Availability.h
krapivskymain.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
krapivskymain.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
krapivskymain.o: /usr/include/sys/_symbol_aliasing.h
krapivskymain.o: /usr/include/sys/_posix_availability.h
krapivskymain.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
krapivskymain.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
krapivskymain.o: /usr/include/sys/appleapiopts.h
krapivskymain.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
krapivskymain.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
krapivskymain.o: /usr/include/machine/_structs.h /usr/include/sys/resource.h
krapivskymain.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
krapivskymain.o: /usr/include/sys/_endian.h
krapivskymain.o: /usr/include/libkern/_OSByteOrder.h
krapivskymain.o: /usr/include/libkern/i386/_OSByteOrder.h
krapivskymain.o: /usr/include/alloca.h /usr/include/machine/types.h
krapivskymain.o: /usr/include/i386/types.h /usr/include/stdint.h
krapivskymain.o: /usr/include/stdio.h /usr/include/secure/_stdio.h
krapivskymain.o: /usr/include/secure/_common.h /usr/include/string.h
krapivskymain.o: /usr/include/strings.h /usr/include/secure/_string.h
krapivskymain.o: networknode.h bstreap.h heap.h krapivsky.h
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
