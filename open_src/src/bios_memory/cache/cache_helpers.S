
#define LOUIS_SHIFT		21
#define CLIDR_FIELD_WIDTH	3
#define LEVEL_SHIFT		1
#define LOC_SHIFT		24


.macro func _name
        .section .text.\_name, "ax"
        .type \_name, %function
        \_name:
        .endm

.macro  dcache_line_size  reg, tmp
        mrs     \tmp, ctr_el0
        ubfx    \tmp, \tmp, #16, #4
        mov     \reg, #4
        lsl     \reg, \reg, \tmp
        .endm


	.globl	flush_dcache_range
	.globl	inv_dcache_range
	.globl	dcsw_op_louis
	.globl	dcsw_op_all
	.globl	dcsw_op_level1
	.globl	dcsw_op_level2
	.globl	dcsw_op_level3


/*#include "defs.h"

#include "cman.h"
#include "cman_private.h"
*/

__mmu_translate_s1:
    at S1E1R, x0
    mrs x0, PAR_EL1
    ret


/*
 * low-level cache operations
 */

    .global __cache_inv_icache
    .global __cache_inv_dcache

/* void __cache_inv_icache() */
__cache_inv_icache:
    ic ialluis
    dsb sy
    ret


/* void __cache_inv_dcache()
 *
 * code mostly taken from ARMv8 arch ref manual with some
 * very minor changes
 */
__cache_inv_dcache:
    dsb sy

    MRS X0, CLIDR_EL1
    AND W3, W0, #0x07000000 // get 2 x level of coherency
    LSR W3, W3, #23
    CBZ W3, Finished
    MOV W10, #0 // W10 = 2 x cache level
    MOV W8, #1 // W8 = constant 0b1
Loop1:
    ADD W2, W10, W10, LSR #1 // calculate 3 x cache level
    LSR W1, W0, W2 // extract 3-bit cache type for this level
    AND W1, W1, #0x7
    CMP W1, #2
    B.LT Skip // no data or unified cache at this level

    /* note: we wan to make this one atomic */
    mrs x2, daif
    msr daifset, #3

    MSR CSSELR_EL1, X10 // select this cache level
    ISB // sync change of CSSELR
    MRS X1, CCSIDR_EL1 // read CCSIDR

    /* restore interrupts */
    msr daif, x2


    AND W2, W1, #7 // W2 = log2(linelen)-4
    ADD W2, W2, #4 // W2 = log2(linelen)
    UBFX W4, W1, #3, #10 // W4 = max way number, right aligned
    CLZ W5, W4 // W5 = 32-log2(ways), bit position of way in DC operand
    LSL W9, W4, W5 // W9 = max way number, aligned to position in DC operand
    LSL W16, W8, W5 // W16 = amount to decrement way number per iteration
Loop2:
    UBFX W7, W1, #13, #15 // W7 = max set number, right aligned
    LSL W7, W7, W2 // W7 = max set number, aligned to position in DC operand
    LSL W17, W8, W2 // W17 = amount to decrement set number per iteration
Loop3:
    ORR W11, W10, W9 // W11 = combine way number and cache number ...
    ORR W11, W11, W7 // ... and set number for DC operand
    DC ISW, X11 // do data cache invalidate by set and way
    SUBS W7, W7, W17 // decrement set number
    B.GE Loop3
    SUBS X9, X9, X16 // decrement way number
    B.GE Loop2
Skip:
    ADD W10, W10, #2 // increment 2 x cache level
    CMP W3, W10
    DSB sy // ensure completion of previous cache maintenance operation
    B.GT Loop1
    DSB sy
Finished:

    isb

    ret



	/* ------------------------------------------
	 * Clean+Invalidate from base address till
	 * size. 'x0' = addr, 'x1' = size
	 * ------------------------------------------
	 */
func flush_dcache_range
	dcache_line_size x2, x3
	add	x1, x0, x1
	sub	x3, x2, #1
	bic	x0, x0, x3
flush_loop:
	dc	civac, x0
	add	x0, x0, x2
	cmp	x0, x1
	b.lo    flush_loop
	dsb	sy
	ret


	/* ------------------------------------------
	 * Invalidate from base address till
	 * size. 'x0' = addr, 'x1' = size
	 * ------------------------------------------
	 */
func inv_dcache_range
	dcache_line_size x2, x3
	add	x1, x0, x1
	sub	x3, x2, #1
	bic	x0, x0, x3
inv_loop:
	dc	ivac, x0
	add	x0, x0, x2
	cmp	x0, x1
	b.lo    inv_loop
	dsb	sy
	ret


	/* ---------------------------------------------------------------
	 * Data cache operations by set/way to the level specified
	 *
	 * The main function, do_dcsw_op requires:
	 * x0: The operation type (0-2), as defined in arch.h
	 * x3: The last cache level to operate on
	 * x9: clidr_el1
	 * x10: The cache level to begin operation from
	 * and will carry out the operation on each data cache from level 0
	 * to the level in x3 in sequence
	 *
	 * The dcsw_op macro sets up the x3 and x9 parameters based on
	 * clidr_el1 cache information before invoking the main function
	 * ---------------------------------------------------------------
	 */

	.macro	dcsw_op shift, fw, ls
	mrs	x9, clidr_el1
	ubfx	x3, x9, \shift, \fw
	lsl	x3, x3, \ls
	mov	x10, xzr
	b	do_dcsw_op
	.endm

func do_dcsw_op
	cbz	x3, exit
	adr	x14, dcsw_loop_table	// compute inner loop address
	add	x14, x14, x0, lsl #5	// inner loop is 8x32-bit instructions
	mov	x0, x9
	mov	w8, #1
loop1:
	add	x2, x10, x10, lsr #1	// work out 3x current cache level
	lsr	x1, x0, x2		// extract cache type bits from clidr
	and	x1, x1, #7		// mask the bits for current cache only
	cmp	x1, #2			// see what cache we have at this level
	b.lt	level_done		// nothing to do if no cache or icache

	msr	csselr_el1, x10		// select current cache level in csselr
	isb				// isb to sych the new cssr&csidr
	mrs	x1, ccsidr_el1		// read the new ccsidr
	and	x2, x1, #7		// extract the length of the cache lines
	add	x2, x2, #4		// add 4 (line length offset)
	ubfx	x4, x1, #3, #10		// maximum way number
	clz	w5, w4			// bit position of way size increment
	lsl	w9, w4, w5		// w9 = aligned max way number
	lsl	w16, w8, w5		// w16 = way number loop decrement
	orr	w9, w10, w9		// w9 = combine way and cache number
	ubfx	w6, w1, #13, #15	// w6 = max set number
	lsl	w17, w8, w2		// w17 = set number loop decrement
	dsb	sy			// barrier before we start this level
	br	x14			// jump to DC operation specific loop

	.macro	dcsw_loop _op
loop2_\_op:
	lsl	w7, w6, w2		// w7 = aligned max set number

loop3_\_op:
	orr	w11, w9, w7		// combine cache, way and set number
	dc	\_op, x11
	subs	w7, w7, w17		// decrement set number
	b.ge	loop3_\_op

	subs	x9, x9, x16		// decrement way number
	b.ge	loop2_\_op

	b	level_done
	.endm

level_done:
	add	x10, x10, #2		// increment cache number
	cmp	x3, x10
	b.gt    loop1
	msr	csselr_el1, xzr		// select cache level 0 in csselr
	dsb	sy			// barrier to complete final cache operation
	isb
exit:
	ret

dcsw_loop_table:
	dcsw_loop isw
	dcsw_loop cisw
	dcsw_loop csw


func dcsw_op_louis
	dcsw_op #LOUIS_SHIFT, #CLIDR_FIELD_WIDTH, #LEVEL_SHIFT


func dcsw_op_all
	dcsw_op #LOC_SHIFT, #CLIDR_FIELD_WIDTH, #LEVEL_SHIFT

	/* ---------------------------------------------------------------
	 *  Helper macro for data cache operations by set/way for the
	 *  level specified
	 * ---------------------------------------------------------------
	 */
	.macro dcsw_op_level level
	mrs	x9, clidr_el1
	mov	x3, \level
	sub	x10, x3, #2
	b	do_dcsw_op
	.endm

	/* ---------------------------------------------------------------
	 * Data cache operations by set/way for level 1 cache
	 *
	 * The main function, do_dcsw_op requires:
	 * x0: The operation type (0-2), as defined in arch.h
	 * ---------------------------------------------------------------
	 */
func dcsw_op_level1
	dcsw_op_level #(1 << LEVEL_SHIFT)

	/* ---------------------------------------------------------------
	 * Data cache operations by set/way for level 2 cache
	 *
	 * The main function, do_dcsw_op requires:
	 * x0: The operation type (0-2), as defined in arch.h
	 * ---------------------------------------------------------------
	 */
func dcsw_op_level2
	dcsw_op_level #(2 << LEVEL_SHIFT)

	/* ---------------------------------------------------------------
	 * Data cache operations by set/way for level 3 cache
	 *
	 * The main function, do_dcsw_op requires:
	 * x0: The operation type (0-2), as defined in arch.h
	 * ---------------------------------------------------------------
	 */
func dcsw_op_level3
	dcsw_op_level #(3 << LEVEL_SHIFT)

	.section ".keywords"
	 bios_memory_cache_helpers_ident_url: .string "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_memory/branches/haspoc/hikey/bios_memory/cache/cache_helpers.S $"
        bios_cache_helpers_ident_rev: .string "$Revision: 26001 $"
