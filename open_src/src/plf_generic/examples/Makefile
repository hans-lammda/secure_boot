PREFIX ?= /opt/nano

CFLAGS  += -D PREFIX=\"$(PREFIX)\"

CFLAGS  +=  -I. -I$(PREFIX)/include
LDFLAGS  =  -lns_plf -L$(PREFIX)/lib

BINARIES = dir safestr dynload memory file plf_proc ngetopt foo

all: $(BINARIES)
build: $(BINARIES)

run: $(BINARIES)
	@for p in $(BINARIES) ; do \
           LD_LIBRARY_PATH=$(PREFIX)/lib ./$$p ; \
	done 
clean:	
	$(RM) *~ *.o  $(BINARIES)
