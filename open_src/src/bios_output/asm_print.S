
#include "pl011.h"

#define ASCII_OFFSET_NUM        0x30


        .globl  asm_print_str
        .globl  asm_print_hex


.macro func _name
        .section .text.\_name, "ax"
        .type \_name, %function
        \_name:
        .endm

/*
 * This function prints a string from address in x4.
 * In: x4 = pointer to string.
 * Clobber: x30, x0, x1, x2, x3
 */
func asm_print_str
        mov     x3, x30
1:
        ldrb    w0, [x4], #0x1
        cbz     x0, 2f
        bl      plat_crash_console_putc
        b       1b
2:
        ret     x3

/*
 * This function prints a hexadecimal number in x4.
 * In: x4 = the hexadecimal to print.
 * Clobber: x30, x0, x5, x1, x2, x3
 */
func asm_print_hex
        mov     x3, x30
        mov     x5, #64  /* No of bits to convert to ascii */
1:
        sub     x5, x5, #4
        lsrv    x0, x4, x5
        and     x0, x0, #0xf
        cmp     x0, #0xA
        b.lo    2f
        /* Add by 0x27 in addition to ASCII_OFFSET_NUM
         * to get ascii for characters 'a - f'.
         */
        add     x0, x0, #0x27
2:
        add     x0, x0, #ASCII_OFFSET_NUM
        bl      plat_crash_console_putc
        cbnz    x5, 1b
        ret     x3

       .section ".keywords"
        bios_output_asm_print_ident_url: .string "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_output/branches/haspoc/arm64_v8/bios_output/asm_print.S $"
        bios_output_asm_print_ident_rev: .string "$Revision: 25866 $"


