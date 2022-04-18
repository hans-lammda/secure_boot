#ifndef __ASM_IO_H__
#define __ASM_IO_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/plf/io.h $ $Revision: 26277 $"


static inline void mmio_write_8(uint64_t addr, uint8_t value)
{
        *(volatile uint8_t*)addr = value;
}

static inline uint8_t mmio_read_8(uint64_t addr)
{
        return *(volatile uint8_t*)addr;
}

static inline void mmio_write_32(uint64_t addr, uint32_t value)
{
        *(volatile uint32_t*)addr = value;
}

static inline uint32_t mmio_read_32(uint64_t addr)
{
        return *(volatile uint32_t*)addr;
}

static inline void mmio_write_64(uint64_t addr, uint64_t value)
{
        *(volatile uint64_t*)addr = value;
}

static inline uint64_t mmio_read_64(uint64_t addr)
{
        return *(volatile uint64_t*)addr;
}


/*
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */

#define dmb()           __asm__ __volatile__ ("" : : : "memory")
#define __iormb()       dmb()
#define __iowmb()       dmb()


#define readl(c)        ({ unsigned int __v = __arch_getl(c); __iormb(); __v; })
#define writel(v,c)     ({ unsigned int  __v = v; __iowmb(); __arch_putl(__v,c); __v; })

#define readw(c)        ({ unsigned short  __v = __arch_getw(c); __iormb(); __v; })
#define writew(v,c)     ({ unsigned short  __v = v; __iowmb(); __arch_putw(__v,c); __v; })

#define readb(c)        ({ unsigned char  __v = __arch_getb(c); __iormb(); __v; })
#define writeb(v,c)     ({ unsigned char  __v = v; __iowmb(); __arch_putb(__v,c); __v; })


#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))

#define __arch_putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))


#define _DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)                     \
static inline void write_ ## _name(uint64_t v)                          \
{                                                                       \
        __asm__ volatile ("msr " #_reg_name ", %0" : : "r" (v));        \
}


#define _DEFINE_SYSREG_READ_FUNC(_name, _reg_name)              \
static inline uint64_t read_ ## _name(void)                     \
{                                                               \
        uint64_t v;                                             \
        __asm__ volatile ("mrs %0, " #_reg_name : "=r" (v));    \
        return v;                                               \
}

/* Define read & write function for system register */
#define DEFINE_SYSREG_RW_FUNCS(_name)                   \
        _DEFINE_SYSREG_READ_FUNC(_name, _name)          \
        _DEFINE_SYSREG_WRITE_FUNC(_name, _name)


/* Define function for simple system instruction */
#define DEFINE_SYSOP_FUNC(_op)                          \
static inline void _op(void)                            \
{                                                       \
        __asm__ (#_op);                                 \
}


#define DEFINE_SYSOP_TYPE_FUNC(_op, _type)              \
static inline void _op ## _type(void)                   \
{                                                       \
        __asm__ (#_op " " #_type);                      \
}


/* Define read function for system register */
#define DEFINE_SYSREG_READ_FUNC(_name)                  \
        _DEFINE_SYSREG_READ_FUNC(_name, _name)



DEFINE_SYSREG_RW_FUNCS(elr_el3)
DEFINE_SYSREG_RW_FUNCS(spsr_el3)
DEFINE_SYSREG_RW_FUNCS(scr_el3)
DEFINE_SYSREG_RW_FUNCS(sctlr_el1)
DEFINE_SYSREG_RW_FUNCS(mair_el3)
DEFINE_SYSREG_RW_FUNCS(tcr_el3)
DEFINE_SYSREG_RW_FUNCS(ttbr0_el3)
DEFINE_SYSREG_RW_FUNCS(sctlr_el3)
DEFINE_SYSREG_READ_FUNC(mpidr_el1)

DEFINE_SYSREG_READ_FUNC(pmccntr_el0)
DEFINE_SYSREG_READ_FUNC(cntfrq_el0)
DEFINE_SYSREG_RW_FUNCS(pmcr_el0)
DEFINE_SYSREG_RW_FUNCS(pmcntenset_el0)

DEFINE_SYSOP_TYPE_FUNC(tlbi, alle3)
DEFINE_SYSOP_TYPE_FUNC(dsb, sy)
DEFINE_SYSOP_FUNC(isb)

#define dsb()                   dsbsy()

#endif



