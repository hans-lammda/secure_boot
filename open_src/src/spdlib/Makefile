#########################
#
## Copyright Oniteo 2007-2008 
#
##########################


PREFIX ?= /home/hans/sb/build


LDFLAGS =  -lc -ldl -lintegrity -ltrust -lns_plf  -L. -L$(PREFIX)/lib

CFLAGS  += -DPREFIX=\"$(PREFIX)\" -I. -I$(PREFIX)/include
CFLAGS  += -Wall
CFLAGS  += -fPIC

OBJECTS_LIB = spd_verify.o spd_install.o

BINARIES = libspd.so spd_test


libspd.so:  $(OBJECTS_LIB)
	$(LD) -shared $(OBJECTS_LIB)   -o $@ $(LDFLAGS)

spd_test:
	$(CC) $(CFLAGS) spd_test.c  -lspd   $(LDFLAGS) -o spd_test



%o: %c
	$(CC) $(CFLAGS)  -c $< -o $@


build: $(BINARIES)
	mkdir -p           $(DESTDIR)$(PREFIX)/lib
	cp libspd.so       $(DESTDIR)$(PREFIX)/lib/

install:
	mkdir -p           $(DESTDIR)$(PREFIX)/bios/obj
	cp $(OBJECTS_LIB)  $(DESTDIR)$(PREFIX)/bios/obj


run:
	LD_LIBRARY_PATH=$(PREFIX)/lib ./spd_test ../../loop/exec/flash/hsbf.bin 3 

clean: 
	rm -f *.o *.so 

prepare:
	mkdir -p       $(DESTDIR_STATE)$(PREFIX)/include/spd
	cp spd.h       $(DESTDIR_STATE)$(PREFIX)/include/spd

