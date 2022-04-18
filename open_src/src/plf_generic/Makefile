
prepare: install

build:

install:
	mkdir -p		$(DESTDIR)$(PREFIX)/include/plf
	cp *.h 			$(DESTDIR)$(PREFIX)/include/plf
	mkdir -p              $(DESTDIR)$(PREFIX)/man
	( cd man && tar cf - * | ( cd $(DESTDIR)$(PREFIX)/man && tar xf - ))

clean:

