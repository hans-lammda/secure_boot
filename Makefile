#
#  1. Creates and sign the boot_image 
#  2. Build generic modules on X86 
#  3. Verify that boot_image is loaded, verified and parsed. It will stop with error 
#      main.c:_main:190 Failed to boot -1 


all: create_image x86_build x86_test

create_image: 
	( cd open_src && make -f Makefile.img create ) 

x86_build: 
	( cd open_src && make -f Makefile.gen build ) 

x86_test: 
	( cd open_src && make -f Makefile.gen test ) 


keywords:
	find . -name '*.o' | xargs ident

