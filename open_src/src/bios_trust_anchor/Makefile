#########################
#
## Copyright Oniteo 2007-2008 
#
##########################


PREFIX ?= /home/hans/haspoc/build


LDFLAGS =  -lc -ldl -lpthread

CFLAGS  += -DPREFIX=\"$(PREFIX)\" -I. -I$(PREFIX)/include
CFLAGS  += -fno-builtin
CFLAGS  += -fkeep-inline-functions
CFLAGS  += -fno-common
CFLAGS  += -ffreestanding
CFLAGS  += -Wall
CFLAGS  += -fPIC

OBJECTS_LIB = trust.o

BINARIES = libtrust.so


libtrust.so:  $(OBJECTS_LIB)
	$(LD) -shared $(OBJECTS_LIB)   -o $@ $(LDFLAGS)


%o: %c
	$(CC) $(CFLAGS)  -c $< -o $@


build: $(BINARIES)
	mkdir -p           $(DESTDIR)$(PREFIX)/bios/obj
	cp $(OBJECTS_LIB)  $(DESTDIR)$(PREFIX)/bios/obj

install:
	mkdir -p              $(DESTDIR)$(PREFIX)/lib
	cp libtrust.so       $(DESTDIR)$(PREFIX)/lib/


clean: 
	rm -f *.o *.so 

prepare:
	mkdir -p		$(DESTDIR)$(PREFIX)/include/
	cp trust.h  		$(DESTDIR)$(PREFIX)/include/

