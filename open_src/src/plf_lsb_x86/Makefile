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

OBJECTS_LIB = memory.o dir.o  ngetopt.o safestr.o plf_proc.o system.o file.o timer.o


BINARIES = libns_plf.so


libns_plf.so:  $(OBJECTS_LIB)
	$(LD) -shared $(OBJECTS_LIB)   -o $@ $(LDFLAGS)


%o: %c
	$(CC) $(CFLAGS)  -c $< -o $@


build: $(BINARIES)

install:
	mkdir -p              $(DESTDIR)$(PREFIX)/lib
	cp libns_plf.so       $(DESTDIR)$(PREFIX)/lib/


clean: 
	rm -f *.o *.so 

prepare:
	mkdir -p		$(DESTDIR)$(PREFIX)/include/
	cp -r plf 		$(DESTDIR)$(PREFIX)/include/

