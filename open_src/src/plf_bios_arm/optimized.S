

.globl  plf_zeromem16
.globl  plf_memcpy16

.globl read_ms
.globl read_clk
.globl read_freq


read_ms:
		mrs	x1, cntfrq_el0
		mov x0, #1000
		udiv	x1, x1, x0
		mrs	x0, pmccntr_el0
		udiv	x0, x0, x1
		ret

read_clk:
		mrs x0, pmccntr_el0
		ret

read_freq:
		mrs x0, cntfrq_el0
		ret

plf_memcpy16:

m_loop16:
        cmp     x2, #16
        b.lt    m_loop1
        ldp     x3, x4, [x1], #16
        stp     x3, x4, [x0], #16
        sub     x2, x2, #16
        b       m_loop16
/* copy byte per byte */
m_loop1:
        cbz     x2, m_end
        ldrb    w3, [x1], #1
        strb    w3, [x0], #1
        subs    x2, x2, #1
        b.ne    m_loop1
m_end:  ret


plf_zeromem16:
        add     x2, x0, x1
/* zero 16 bytes at a time */
z_loop16:
        sub     x3, x2, x0
        cmp     x3, #16
        b.lt    z_loop1
        stp     xzr, xzr, [x0], #16
        b       z_loop16
/* zero byte per byte */
z_loop1:
        cmp     x0, x2
        b.eq    z_end
        strb    wzr, [x0], #1
        b       z_loop1
z_end:  ret

	.section ".keywords"
        plf_bios_arm_optimized_ident_url: .string "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/optimized.S $"
        plf_bios_arm_optimized_ident_rev: .string "$Revision: 26497 $"



