# $HeadURL: https://cm-ext.dev.oniteo.com/svn/ir/oniteo.com/suites/trusted_arm/branches/haspoc/hikey/extra/simple_build/import.mk $
# $Revision: 2737 $


# Build directory for Secure boot 
CC_TOP = /home/hans/sb



PREFIX = $(CC_TOP)/build

CROSS_COMP_PATH        = $(CC_TOP)/tools/crosscompiler/gcc-linaro-aarch64-none-elf-4.9-2014.07_linux
CROSS_COMP_PREFIX      = aarch64-none-elf-
CROSS_COMPILE          = $(CROSS_COMP_PATH)/bin/$(CROSS_COMP_PREFIX)
CROSS_COMP_DIR         = $(CROSS_COMP_PATH)/lib/gcc/aarch64-none-elf/4.9.1
CROSS_COMPILER_CFLAGS  = -I$(CROSS_COMP_DIR)/include  -ffreestanding -mgeneral-regs-only -nostdinc  -ffunction-sections -fdata-sections  -fno-stack-protector --no-common 
CROSS_COMPILER_ASFLAGS = -Wa,--gdwarf-2 -nostdinc  -Wa,--fatal-warnings -Werror --no-common

CC                     = $(CROSS_COMPILE)gcc
LD                     = $(CROSS_COMPILE)ld
OBJCOPY                = $(CROSS_COMPILE)objcopy
