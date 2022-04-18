#########################
#
## Copyright Oniteo 2007-2008 
#
##########################


PREFIX ?= /home/hans/sb/open_src/build

TEST_DEBUG_LEVEL = 10

LDFLAGS =  -lspd -lintegrity -ltrust -lns_plf -lc  -L. -L$(PREFIX)/lib

CFLAGS  += -DPREFIX=\"$(PREFIX)\" -I. -I$(PREFIX)/include -I$(PREFIX)/bios/include
CFLAGS  += -Wall
CFLAGS  += -fPIC
CFLAGS  += -DTESTMODE=\"nocopy\"
CFLAGS  += -DDEBUG_LEVEL=$(TEST_DEBUG_LEVEL)
CFLAGS  += -DSUITE_BUILD_ID=0
CFLAGS  += -DARM64=1



OBJECTS_LIB = main.o

BINARIES = libbios_high.so main_test


libbios_high.so:  $(OBJECTS_LIB)
	$(LD) -shared $(OBJECTS_LIB) $(LDFLAGS)   -o $@ 
	mkdir -p           $(DESTDIR)$(PREFIX)/lib
	cp libbios_high.so       $(DESTDIR)$(PREFIX)/lib/


%o: %c
	$(CC) $(CFLAGS)  -c $< -o $@


build: $(BINARIES)

install:
	mkdir -p           $(DESTDIR)$(PREFIX)/bios/obj
	cp $(OBJECTS_LIB) 	$(DESTDIR)$(PREFIX)/bios/obj


clean: 
	rm -f *.o *.so $(BINARIES) 

prepare:
	mkdir -p       $(DESTDIR_STATE)$(PREFIX)/include/spd
	cp bios_high.h       $(DESTDIR_STATE)$(PREFIX)/include/spd


main_test:
	$(CC) $(CFLAGS) main_test.c  -lbios_high   $(LDFLAGS) -o main_test

run: 
	LD_LIBRARY_PATH=$(PREFIX)/lib ./main_test ../../hsbf/hsbf_image

debug:
	LD_LIBRARY_PATH=$(PREFIX)/lib gdb --args main_test ../../hsbf/hsbf_image

