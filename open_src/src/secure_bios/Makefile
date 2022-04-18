#########################
#
## Copyright Oniteo 2007-2008 
#
##########################


PREFIX ?= /home/hans/haspoc/build


LDFLAGS =  -lc -ldl  -lns_plf  -L. -L$(PREFIX)/lib

CFLAGS  += -DPREFIX=\"$(PREFIX)\" -I. -I$(PREFIX)/include
CFLAGS  += -Wall
CFLAGS  += -fPIC
# Optimize for speed
CFLAGS  += -Os


BINARIES = libintegrity.so


OBJ = rsa_ver.o bignum.o rsa_math.o  sha2.o 

libintegrity.so:  $(OBJ)
	#  LD=gcc may solve problem with unresolved symbols
        #  __aeabi_uidiv
	$(CC_ENV) $(LD) -shared $(OBJ)  $(LDFLAGS)  -o libintegrity.so

%o: %c
	$(CC) $(CFLAGS)  -c $< -o $@


build: $(BINARIES)
	mkdir -p	$(DESTDIR)$(PREFIX)/bios/obj
	cp $(OBJ)  	$(DESTDIR)$(PREFIX)/bios/obj


install:
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp libintegrity.so $(DESTDIR)$(PREFIX)/lib


clean: 
	rm -f *.o *.so  

prepare:
	mkdir -p $(DESTDIR)$(PREFIX)/bios/include
	cp rsa_ver.h $(DESTDIR)$(PREFIX)/include
	cp sha2.h $(DESTDIR)$(PREFIX)/include

