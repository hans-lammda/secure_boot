# Secure Boot #

## For the impatient

If you want to build the generic modules for ARM on your X86 laptop.

```
$ make 
```



## General ##
This project is related to the HASPOC project sponsored by Vinnova
High Assurance Security Products On COTS platforms. 



## Referenses
I watch this area, maybe I pick up the work in the future. 
### Overview of existing bootloaders
* [Comparison](https://en.wikipedia.org/wiki/Comparison_of_bootloaders)
### Attacks on Trustzone
[TEE](https://github.com/enovella/TEE-reversing)
### Business impact
* [Apple](https://9to5mac.com/2023/11/18/idc-report-highlights-how-security-has-become-apples-trojan-horse-in-the-enterprise/)
### Encryption 
* [Encryption](https://github.com/ANSSI-FR/SecAESSTM32)
* [Quantum Safe](https://openquantumsafe.org)
* [Hashboot](https://eprint.iacr.org/2022/1198.pdf)
* [BL1_2](https://docs.nordicsemi.com/bundle/ncs-2.3.0/page/tfm/technical_references/design_docs/index.html)
* [Wolfiboot](https://www.wolfssl.com/wolfboot-secure-boot-and-more-unique-features-to-assist-and-optimize-firmware-updates/)
### Uboot 
* [Update on uboot](https://www.youtube.com/watch?v=YlJBsVZJkDI)
### RISE 
* [Assist](https://assist-project.github.io/events/)
### Root of trust 
* [OpenTitan](https://opentitan.org/)
### TPM 
[TPM and Linux](https://stackoverflow.com/questions/59951735/using-tpm-under-embedded-linux-on-arm)
[TPM and Linaro](https://static.linaro.org/connect/bkk19/presentations/bkk19-215.pdf) 

## Hypervisors

[Krata](https://github.com/edera-dev/krata/blob/main/FAQ.md)
[ACRN](https://projectacrn.github.io/latest/index.html) 

### Security issues 
* [Black Louts](https://www.theregister.com/2023/03/01/blacklotus_malware_eset/) malware can bypass Secure boot Mar 2023
### Lifecycle management
* [RAUC](https://rauc.readthedocs.io/en/latest/using.html)
* [Axis](https://www.axis.com/developer-community/open-source)
* [Axis-OS](https://www.axis.com/support/axis-os) 
### Hardware and tools 
* [RISCV](https://github.com/RISCV-MYTH-WORKSHOP/)
* [Veripool](https://www.veripool.org/verilator/)
* [Chipyard](https://chipyard.readthedocs.io/en/stable/index.html)
* [Write our own loader](https://dev.to/frosnerd/writing-my-own-boot-loader-3mld)
* [Hardware without closed microcode](https://www.phoronix.com/).
### Memory safe languages 
#### RUST
*"Write once run everywhere"*
[Rust](https://www.rust-lang.org/) and [WASM](https://webassembly.org/) seems to play well together. 

There is also some projects related to bootloaders. 
* [rust-os](https://github.com/rust-osdev/bootloader) 
* [rustBoot](https://github.com/nihalpasham/rustBoot) 
* [Loadstore](https://www.reddit.com/r/rust/comments/ojccg2/announcing_loadstone_a_secure_baremetal_rust/) 
Linux kernel 
* [Kernel module](https://wusyong.github.io/posts/rust-kernel-module-01/)
Microservices
* [Rust example](https://medium.com/tenable-techblog/building-a-microservice-with-rust-23a4de6e5e14)
#### C and C++ 
[White house about C and memory safety](https://www-infoworld-com.cdn.ampproject.org/c/s/www.infoworld.com/article/3713203/white-house-urges-developers-to-dump-c-and-c.amp.html)

## Maintainer ##
hans ( at ) lammda.se 

## Features ##
* Modular designed adapted for  [Common Criteria](https://en.wikipedia.org/wiki/Common_Criteria)
* Compliant with [ARM Trusted Firmware](https://github.com/ARM-software/arm-trusted-firmware)
* Adapted for [Formal Verification](https://en.wikipedia.org/wiki/Formal_verification)

## Keep it simple ##
The reference implementation for Trusted Firmware has been studied as well as several documents covering boot. 

[Boot Requirements for ARMv8]
(http://infocenter.arm.com/help/topic/com.arm.doc.den0044b/DEN0044B_Server_Base_Boot_Requirements.pdf)

To satisfy the requirement of Common Criteria and Formal Verification the design is simplified. 

* Warm boot eliminated 
* Static configuration of memory management ( TCR , MAIR , TBBR ) 
* ROM resident code locked to exception level 3 
* Minimalistic design of boot format


The minimal requirement for booting is 

* Load image from media into memory 
* Transfer control to loaded image


The design is balanced between simplicity and required functionality. Another aspect of security is understanding, instead of building generic framework with configuration tools, Secure boot is designed for understanding. 

The very first instructions is kept in **bl1_entrypoint.S** without any includes to simplify security audits. 


Modern hardware used in smartphones are often multicore systems, with 
functionality for low energy consumption. Secure boot must therefore manage power on/off for secondary cores
after the actual boot sequence. 


*"just load the OS
and get the hell out of there"*

Linus Thorvalds(http://yarchive.net/comp/linux/efi.html)

## Adoption for Common Criteria ##
The product must be described following CC standard. 

* TOE       ( Target Of Evaluation ) 
* TSF       ( TOE Security Functionality )
* TSFI      ( TSF Interface ) Security functions that are external for the TOE  
* Subsystem ( The HASPOC TOE consists of two subsystems ( Secure boot and the Hypervisor ) 
* Modules   ( Each subsystem should be divied into modules ) 



The process of CC evaluation ( EAL-6)  is basicly how the documents relates to each other. 


* SFR       ( Security Functional Requirements ) 
* FSP       ( Function specification ) 
* TDS       ( Design specification ) 

***CC requirements for the TDS***

* Classification of modules toward the security functionalities defined for the TOE: enforcing, supporting or non-interfering
* Semi-formal description of each module, interaction between modules, interfaces of a module. 


The SFR Document is excluded here since most of the requirements are related to the other subsystem. The related requirements are 

* Cold boot (TSFI_COLD_BOOT) 
* Load and execute a signed boot image (TSFI_STORAGE) 

This two interfaces are the only external attack surfaces, attacks against TSFI_COLD_BOOT is remove/insert power cord to the TOE during boot. attack against TSFI_STORAGE is corrupt of badly configured boot images. 

The FSP Document describes functionality, the TDS document is more detailed about how the modules are related to each TSFI. 

### Secure boot modules ###
The Boot subsystem contains the modules below

Generic and portable modules 

* bios_high         ( *Enforcing*       Boot logic ) 
* spdlib            ( *Supporting*      Process boot image) 
* bios_trust_anchor ( *Supporting*      Hash of public key, should reside in ROM or eFuse) 
* bios_fip          ( *Supporting*      Interface to Arm Trusted firmware runtime module bl31 ) 
* secure_bios       ( *Supporting*      SHA256 and RSA library) 

The modules below are hardware dependent 

* bios_memory       ( *Supporting*      MMU,Cache, DRAM configuration) 
* bios_output       ( *Non Interfering* Output via serialport) 
* plf_bios_arm      ( *Supporting*      Platform runtime for ARM )
* bios_low          ( *Supporting*      Configuration of hardware, non portable) 

Each module contains two Makefiles. One for building for unit tests on the build server, 
and one Makefile.arm required for the ARMv8 target. Development and test of generic modules 
( about 50 % of the source code ) could be executed on a Workstation with native debuggers etc.
Some additional modules required for compilation and test 

* plf_lsb_x86       ( Platform runtime for Linux/X86 ) 
* linaro            ( Cross compiler for ARMv8 ) 
* bios_arm_build    ( linker script ) 

### Waterfall versus agile ###
Combining CC and Open source offers challenges, since CC is document centric and Open Source is code centric. 
Any change by a external contributor may break the relation with the document. The problem is the integration of content extracted from sourcecode/tools with content stored in OpenOffice/Words. The complete TDS document is found in the source directory, here is just three examples which illustrates how content from different source code be integrated in the same document. 


***Sequence diagram manually composed***

This picture describes integration with ARM Trusted Firmware


![Screenshot from 2016-09-21 11-08-11.png](https://bitbucket.org/repo/yLG48x/images/4220868894-Screenshot%20from%202016-09-21%2011-08-11.png)

***Dependencies between modules***

Each external interface to security functions ( TSFI ) must be described regarding dependencies to other modules. The build system for Secure Boot is implemented so symbols from the ELF-images could be postprocessed for generating dependency tables. 


![Screenshot from 2016-09-21 10-59-35.png](https://bitbucket.org/repo/yLG48x/images/2737672638-Screenshot%20from%202016-09-21%2010-59-35.png)


***Module Interface***

By adding information inside comments in the source code, tools used for agile projects could be applied for CC. 

![Screenshot from 2016-09-21 11-04-44.png](https://bitbucket.org/repo/yLG48x/images/3505302465-Screenshot%20from%202016-09-21%2011-04-44.png)

The complete TDS document could be found found in the source directory. 



In the document directory there is some examples of tests that assure that the functionality is correct. Tools for generating document from source code and test results are part of T2Data commercial offering and therefore excluded.

# Building Secure Boot #

## For the impatient 
```
$ git clone https://github.com/hans-lammda/secure_boot.git
$ cd secure_boot
$ make 
```


## ARM Build
The crosscompiler for ARMv8 is included in the repository, but other compilers may work. 


```
#!sh

$ git clone https://github.com/hans-lammda/secure_boot.git

$ cd secure_boot/open_src

$ make -f Makefile.img create # Create signed HASPOC boot image 

$ make -f Makefile.arm build  # Create first stage boot loader ( bl1.bin ) 

$ make -f Makefile.gen build  # Build portable code on your Linux system

$ make -f Makefile.gen test   # Unit test of HASPOC boot image
```


## Prepare for Boot on Hikey board ##
![hikey.JPG](https://bitbucket.org/repo/yLG48x/images/3812312777-hikey.JPG)
For development on the Hikey board, USB boot is recommended. Since Secure Boot initially was 
developed for the Juno Development board, which enable boot from 64 bits mode. The boot ROM in 
the Hikey board handover the platform in 32 mode, therefore a wrapper is required to change from 
32 to 64 bit mode. 


[Hikey Board](http://www.lenovator.com/product/90.html)



[Hikey loader](http://github.com/96boards/l-loader)



The same site also contain tools for generate the USB boot image. 

The output from Secure Boot is found in src/bios_arm_build/bl1.bin. A Python tool generates 
the format required by Hikey Boot. The format starts with a header followed by the wrapper and 
bl1.bin. 



$ python gen_loader.py -o l-loader.bin  --img_loader=l_loader_start.bin --img_bl1=bl1.bin

$ python hisi-idt.py l-loader.bin 

*Switch to aarch64 mode. CPU0 executes at 0xf9801000!*



# Create HASPOC Boot Image  #

The image is generated by the signer tool, based on a manifest. Since Secure Boot lacks file system, the size of the file MUST be stored inside the image, this is managed by signing in two pass. The boot logic supports file system, but due to simplicity it has been excluded. The image must therefore be stored in a dedicated partition on the flash. The first sector of that partition is hardcoded into the ROM-resident part of Secure Boot ( bl1.bin ). 


![Screenshot from 2016-09-21 14-59-07.png](https://bitbucket.org/repo/yLG48x/images/1155554219-Screenshot%20from%202016-09-21%2014-59-07.png)

## Calculate the size of HSBF Image ##
*size=`tools/bin/signer  --rootpk  root_pk/pub.pem --priv root_pk/priv.pem  --out hsbf/hsbf_image --config hsbf/hikey2.ini*


## Final sign with size included ##
*tools/bin/signer  --rootpk  root_pk/pub.pem --priv root_pk/priv.pem   --out hsbf/hsbf_image  --config hsbf/hikey2.ini --size $$size*

## Sign manifest ##
It is important to inspect the manifest, prior to signing, since Secure Boot complete trust the HSBF image. Each section in the manifest corresponds to a object in the resulting image. 

***Note:*** The two first object MUST be the public key and DRAM configuration. The reason is that Secure boot initially executes in a context with limited amount of SRAM, The ROOT_PK object is verified with the ROOT_PK_HASH ( stored in module bios_trust_anchor). The ROOT_PK is then used to verify the SDRAM object. When mass memory is configured, the entire image is retrieved and stored on memory for later processing. 

This manifest loads the hypervisor and the payload object ( loaded as ramdisk type). The controll is passed by the final object, that creates a area in secure memory that controls which exception level the payload should use. 

 

 

```
#!ini

[rootpk]
type=rootpk
id=Divided objects

[sdram]

type=sdram
image=binaries/mcuimage.bin
heap_size=50000000
heap_start=0x4000



[bl31]
type=atf_runtime
image=binaries/bl31.bin
bl31_load_address=0xf9858000

;
; Compability with ATF
;
[bl33]
type=atf_payload
image=binaries/sth
; BL33_BASE
bl33_load_address=0x03800000

;
; Hypervisor config and guests
;
[payload]
type=ramdisk
load_address=0x03820000
image=binaries/payload


;
; Compability with ATF
;
[fip.bin]
type=fip
; BL31_BASE
bl31_load_address=0xf9858000
; BL32_BASE , not used
bl32_load_address=0x0
;  NS_IMAGE_OFFSET
bl33_load_address=0x03800000
; Exception level for bl33.bin
e_level=2
;
bl33_X0=0x03820000
bl33_X1=0x0
```

This manifest is used to invoke Uboot, note that the exception_level is 1 instead of 2. 


```
#!ini
[rootpk]
type=rootpk
id=Uboot

[sdram]
type=sdram
image=/opt/haspoc/hikey_fip/mcuimage.bin
heap_size=50000000
heap_start=0x1024
id=Uboot


;
; Compability with ATF
;
[bl31]
type=atf_runtime
image=/opt/haspoc/hikey_fip/bl31.bin
; BL31_BASE
bl31_load_address=0xf9858000

;
; Compability with ATF
;
[bl33]
type=atf_payload
image=/opt/haspoc/hikey_fip/u_boot_bl33.bin
; BL33_BASE
bl33_load_address=0x37000000


[fip.bin]
type=fip
bl31_load_address=0xf9858000
bl32_load_address=0x0
bl33_load_address=0x37000000
e_level=1
;
bl33_X0=0x35000000
bl33_X1=0x0

```

# Test #
The ultimate goal is to apply formal verification for the entire subsystem, but the model only cover memory and cryptographic library. Therefore traditional tests is required as a complement. 

## Unit Test ##

The functionality for the boot logic is based on generic modules. The design allows that the HSBF image used for the actual target also could be used for unit tests. 

```
#!c
#include <plf/platform.h>

/* Dependency on Linux */

#include <sys/stat.h>
#include <fcntl.h>
#include <bios_high.h>
#include <bios_low.h>
#include <bios_memory.h>
#include <fip.h>

int bl2main(uint32_t e_level, uint64_t *bl31_load_address, int bl31_size,  uint64_t *bl33_load_address,  int bl33_size, uint64_t  bl33_X0, uint64_t  bl33_X1)
{
  fprintf(stdout,"e_level\t%d\n", e_level);
  fprintf(stdout,"bl31_load_address\t0x%x\n", bl31_load_address);
  fprintf(stdout,"bl33_load_address\t0x%x\n", bl33_load_address);
  fprintf(stdout,"bl33_size\t%d\n", bl33_size);
  fprintf(stdout,"bl31_size\t%d\n", bl31_size);
  return FIP_ERROR_BOOT_FAILED;
}
int main(int argc, char *argv[])
{
        int  rc,i;
        int fp;
        struct stat stat_p;

        if ( argc != 2 ) {
          // trace_dl(DBG_LEVEL,"Usage: %s: hsbf_image \n",argv[0]);
          return 1;
        }


        fp = open(argv[1],O_RDONLY);
        if (fp <  0 )
         return 1;

        if ( fstat(fp, &stat_p) == -1 ) {
          return 1;
        }
        image_size = (stat_p.st_size / BLOCK_SIZE ) + 1 ;
        trace_dl(DBG_LEVEL,"%d\n", image_size);


        image = mmap(NULL, stat_p.st_size, PROT_READ, MAP_PRIVATE, fp, 0);

        if (image  ==  NULL )
          return 1;


        rc = _main(BOOT_START_SECTOR,INITIAL_READ);
        fprintf(stdout,"rc\t%d\n", rc);
        return 0;
}

```

The application above is invoked with the HSBF image generated by the signer, the log below is generated and parsed. 

```
#!
main.c:_main:51: SUITE_BUILD_ID 761055
ROOT_PK_HASH retreved
ROOT_PK_HASH installed
ROOT_PK found in HSBF (pass1)
main.c:_main:96:

ROOT_PK verified

ROOT_PK installed
HSBF Image ID=[Divided objects]
SDRAM found in HSBF (pass1)
main.c:_main:122: About to assign 50000000 bytes starting at 0x4000 for heap
SDRAM Verified image_size = 0/15243296
main.c:_main:152: 15243296 bytes allocated, about to read from eMMC into 0x94970010
main.c:_main:164: HSBF processing node of type 1 len=15243296
main.c:_main:168:

ROOT_PK Ignored 5

main.c:_main:164: HSBF processing node of type 4 len=15242904
main.c:_main:172:

SDRAM Ignored 1

main.c:_main:164: HSBF processing node of type 8 len=15094512
ATF_RUNTIME found in HSBF
main.c:_main:214: verify ok 1
main.c:_main:164: HSBF processing node of type 9 len=15055944
ATF_PAYLOAD found in HSBF
main.c:_main:227: verify ok 1
main.c:_main:164: HSBF processing node of type 5 len=15005792
RAMDISK found in HSBF
main.c:_main:202: verify ok 1
main.c:_main:164: HSBF processing node of type 3 len=1728
ATF_BOOT found in HSBF
main.c:_main:182: verify ok 1
e_level 2
bl31_load_address       0xf9858000
bl33_load_address       0x3800000
bl33_size       48464
bl31_size       36880
main.c:_main:190: Failed to boot -1
rc      16

```
The logfile is processed and compared against the test specification, finally a test report is 
generated. 
![Screenshot from 2016-09-21 16-58-55.png](https://bitbucket.org/repo/yLG48x/images/975660153-Screenshot%20from%202016-09-21%2016-58-55.png)

## Integration test ##
The infrastructure required to automate the test cycle involves hardware. 

### Boot log ###

```
#!


SARA: Capturing data from localhost:2001 Connection timeout 2 Session timeout 30
SARA: Rejected Input , nonascii
debug EMMC boot: print init OK
debug EMMC boot: send RST_N .
debug EMMC boot: start eMMC boot......
load fastboot1!
Switch to aarch64 mode. CPU0 executes at 0xf9801000!
mmu/h_mmu.c:bios_memory_mmu_setup:24: SUITE_BUILD_ID 761055
TCR_EL3 = 80803520
Start address of l1_xlation_table=f980f000
Start address of l2_xlation_table=f980d000
Sample values before assignment: l1_xlat[1]: 40000725 l2_xlat[460]: F9800721, l2_xlat[511]: FFE00725
Sample values  after assignment: l1_xlat[1]: 40000725 l2_xlat[460]: F9800721, l2_xlat[511]: FFE00725
Enabling MMU & D-Cache with sctlr= c5383d
Secure Boot VMA
SRAM
__SRAM_BASE__             = 0xf9800000
__RO_START__              = 0xf9801000
__RO_END__                = 0xf980efb8
__DATA_RAM_START__        = 0xf980efb8
__DATA_RAM_END__          = 0xf980f120
__BSS_START__             = 0xf9810000
__BSS_END__               = 0xf9810100
__PARAMS_BASE__           = 0xf9811000
_LOW_LEVEL_SRAM_STACK_END = 0xf9812000
_LOW_LEVEL_SRAM_STACK     = 0xf98125e0
__SRAM_TOP__              = 0xf9c00000
Size
SRAM_SIZE                 = 4194304
__RO_SIZE__               = 57272
__BSS_SIZE__              = 256
__DATA_SIZE__             = 360
STACK_SIZE                = 1504
SRAM_HEAP_SIZE            = 262144
Config Elapsed time(ms): 1920
main.c:_main:51: SUITE_BUILD_ID 761055
ROOT_PK_HASH retreved
ROOT_PK_HASH installed
ROOT_PK found in HSBF (pass1)
main.c:_main:96:
ROOT_PK verified
ROOT_PK installed
HSBF Image ID=[Divided objects]
SDRAM found in HSBF (pass1)
main.c:_main:122: About to assign 50000000 bytes starting at 0x4000 for heap
SDRAM Verified image_size = 0/15243296
main.c:_main:152: 15243296 bytes allocated, about to read from eMMC into 0x4000
main.c:_main:164: HSBF processing node of type 1 len=15243296
main.c:_main:168:
ROOT_PK Ignored 5
main.c:_main:164: HSBF processing node of type 4 len=15242904
main.c:_main:172:
SDRAM Ignored 1
main.c:_main:164: HSBF processing node of type 8 len=15094512
ATF_RUNTIME found in HSBF
main.c:_main:214: verify ok 1
main.c:_main:164: HSBF processing node of type 9 len=15055944
ATF_PAYLOAD found in HSBF
main.c:_main:227: verify ok 1
main.c:_main:164: HSBF processing node of type 5 len=15005792
RAMDISK found in HSBF
main.c:_main:202: verify ok 1
main.c:_main:164: HSBF processing node of type 3 len=1728
ATF_BOOT found in HSBF
main.c:_main:182: verify ok 1
Parameter block written at 0xf9811000
Flushed start 0xf9858000 36880 bytes
bl33_X0 0x3820000
bl33_X1 0x0
Flushed start 0x3800000 48464 bytes
Post Platform config
About to boot
Secure boot
BL1:
BL1: BL3-1 address  = 0xf9858000
BL1: BL3-1 spsr     = 0x3cd
BL1: BL3-1 E-level  = 3
BL1: BL3-1 X0       = 0xf9811000
BL1: BL3-1 X1       = 0x4b5a6978
BL33
BL1: BL3-3 address  = 0x3800000
BL1: BL3-3 spsr     = 0x3c9
BL1: BL3-3 E-level  = 2
BL1: BL3-3 X0       = 0x3820000
BL1: BL3-3 X1       = 0x0
Exit Elapsed time(ms): 5208


SARA: Capturing data from localhost:2002 Connection timeout 2 Session timeout 30
SARA: Rejected Input , nonascii
NOTICE:  BL3-1: v1.1(debug):ca9f7ed
NOTICE:  BL3-1: Built : 11:24:57, Sep 19 2016
INFO:    BL3-1: Initializing runtime services
INFO:    BL3-1: Preparing for EL3 exit to normal world
INFO:    BL3-1: Next image address = 0x3800000
INFO:    BL3-1: Next image spsr = 0x3c9
INFO:    Elapsed time at exit(ms): 5199

Booting STH ARMv8
Created Jun 29 2016 22:14:13 (DEBUG=2) #059BC8B8
[C0] Performance monitor; Time at boot(ms): 5203
HYP RAM: 0000000003810000-0000000003820000
HYP ROM: 0000000003800000-0000000003810000
ID_AA64AFR  = (0000000000000000 0000000000000000)
ID_AA64DFR  = (0000000010305106 0000000000000000)
ID_AA64ISAR = (0000000000011120 0000000000000000)
ID_AA64MMFR = (0000000000001122 0000000000000000)
ID_AA64PFR  = (0000000000002222 0000000000000000)
ID_MMFR     = (10101105 40000000 01260000 02102211)
MAIR = 0100000000000000, TCR = 80800000, SCTLR=30CD183C
MIDR_EL1=410FD033 MPIDR_EL1=0000000080000000 VMPIDR_EL2=0000000080000000
CACHES: CLIDR = 0A200023, CTR = 84448004
L1 (type 3)  D/U CCSIDR=700FE01A I CCSIDR=201FE00A
L2 (type 4)  D/U CCSIDR=703FE07A
GICD=F6801000 GICC=F6802000 GICH=F6804000 GICV=F6806000
GICD CTLR=00000000 TYPER=0000FCE4 IIDR=0200143B
GICC CTLR=00000060 PMR=000000F0 IIDR=0202143B
GICH HCR=00000000 VTR=90000003 VMCR=004C0000 MISR=00000000
vGIC configuration: 256 pIRQ -> 172 vIRQ
[C0] PAYLOAD at 0000000003820000
...

ALSA device list:
No soundcards found.
uart-pl011 c0004000.uart: no DMA platform data
Freeing unused kernel memory: 3592K (ffffffc000aaa000 - ffffffc000e2c000)
Freeing alternatives memory: 60K (ffffffc000e2c000 - ffffffc000e3b000)
Starting logging: OK
SICS Thin Hypervisor Ethernet driver
STH IGC network interface eth0 created with 256 producer buffers and 256 consumer buffers
SICS Thin Hypervisor: driver for making hypercalls
Initializing random number generator... random: dd urandom read with 1 bits of entropy available
done.
Starting network...
Guest 3 setting up networking
Welcome to Buildroot
buildroot login:


```
The boot log is parsed against a lexical parser, and a test report is generated. 

![Screenshot from 2016-09-21 17-15-34.png](https://bitbucket.org/repo/yLG48x/images/720247480-Screenshot%20from%202016-09-21%2017-15-34.png)

# Licenses #

Each module used in this subsystem have licenses attached. As a result of the build process 
all distributed modules are listed in the legal report. The table below indicates that some modules 
are under dual license. The legal document contains all licenses. 


![Screenshot from 2016-09-21 18-16-18.png](https://bitbucket.org/repo/yLG48x/images/3742652376-Screenshot%20from%202016-09-21%2018-16-18.png)
