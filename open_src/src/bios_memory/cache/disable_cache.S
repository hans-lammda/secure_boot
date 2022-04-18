/*
 * low-level cache operations
 *
 * TODO: someone with better understanding of ARMv8
 *       and multicore should analyse this code
 */


    .text
    .global __cache_inv_icache
    .global __cache_flush_dcache
    .global __cache_flush_dcache_range
    .global __mmu_tlb_inv


/* void __cache_inv_icache() */
__cache_inv_icache:
    ic ialluis
    dsb sy
    ret


/* void __cache_flush_dcache()
 *
 * code mostly taken from ARMv8 arch ref manual with some
 * very minor changes
 */
__cache_flush_dcache:
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
    DC CSW, X11 // do data cache clean by set and way
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


/* void __cache_flush_dcache_range(adr_t, size_t) */
__cache_flush_dcache_range:
    dsb sy

    /* get cache line size */
    mrs x2, ctr_el0
    ubfm x2, x2, #16, #19
    mov x3, #4
    lsl x2, x2, x3

    /* get end and align address to cache line */
    add x1, x1, x0
    sub x3, x2, #1
    bic x0, x0, x3

    /* clear it */
1:
    dc civac, x0
    add x0, x0, x2
    cmp x0, x1
    blo 1b

    dsb sy
    ret


/* void __mmu_tlb_inv() */
__mmu_tlb_inv:
    tlbi VMALLS12E1IS
    dsb sy
    isb
    ret

