/* -----------------------------------------------------


    HASPOC Secure boot

    T2Data AB 2015

* ----------------------------------------------------*/



/* System Control Register  */ 

#define SCTLR_EE_BIT            (1 << 25)  /* Endianess */ 
#define SCTLR_I_BIT             (1 << 12)  /* Enable instruction cache */ 
#define SCTLR_A_BIT             (1 << 1)   /* Alignment */ 
#define SCTLR_SA_BIT            (1 << 3)   /* Stack alignment */ 

/* Interrupt    */ 

#define DAIF_ABT_BIT            (1 << 2)
#define TCPAC_BIT               (1 << 31)
#define TTA_BIT                 (1 << 20)
#define TFP_BIT                 (1 << 10)

/* Affinity   */ 

#define MPIDR_AFFINITY_BITS	8
#define MPIDR_AFFLVL_MASK	0xff
#define MPIDR_CLUSTER_MASK	MPIDR_AFFLVL_MASK << MPIDR_AFFINITY_BITS
#define MPIDR_CPU_MASK		MPIDR_AFFLVL_MASK

/* Security Control Register  */ 

#define SCR_RES1_BITS		((1 << 4) | (1 << 5))
#define SCR_RW_BIT		(1 << 10)



	.globl	bl1_entrypoint


	/* 
	 * All Cores active when released from reset  
	 */

bl1_entrypoint:

	/* 
	 * Set the CPU endianness before doing anything
	 * that might involve memory reads or writes.
	 */
	mrs	x0, sctlr_el3
	bic	x0, x0, #SCTLR_EE_BIT
	msr	sctlr_el3, x0
	isb


	/* 
	 * Enable the instruction cache, stack pointer
	 * and data access alignment checks
	 */
	mov	x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
	mrs	x0, sctlr_el3
	orr	x0, x0, x1
	msr	sctlr_el3, x0
	isb

	/* 
	 * Set the exception vector to something sane.
	 */
	adr	x0, bl1_exceptions
	msr	vbar_el3, x0
	isb

	/*
	 * Enable the SError interrupt now that the
	 * exception vectors have been setup.
	 * 
	 */
	msr	daifclr, #DAIF_ABT_BIT

	mrs	x0, cptr_el3
	bic	w0, w0, #TCPAC_BIT
	bic	w0, w0, #TTA_BIT
	bic	w0, w0, #TFP_BIT
	msr	cptr_el3, x0


        mrs     x0, mpidr_el1
	and     x0, x0, #(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK)	
	cmp     x0, #0
	cset    x0, eq	
        cbnz    x0, primary_cold_boot          /* Read Affinity id to detect primary core */ 

	b loop                                 /* Secondary cores busy wait in loop */ 

        primary_cold_boot:                     /* Only primary core from now on */ 


	bl 	bios_output_setup_perf_timers  /* Reset counters for time measure */ 


        /* 
         * C Runtime:  bss , data ,rodata and stack 
         * 
         */
	ldr	x0, =__BSS_START__
	ldr	x1, =__BSS_SIZE__
	bl	zeromem16

	ldr	x0, =__DATA_RAM_START__
	ldr	x1, =__DATA_ROM_START__
	ldr	x2, =__DATA_SIZE__
	bl	memcpy16

	ldr     x0, =_LOW_LEVEL_SRAM_STACK /* Stack defined in linker script  */  
	mov     sp, x0

        /* 
         * Configure platform similar to Arm Trusted Firmware v1.1 
         */

	bl	bl1_early_platform_setup   /* Hikey early Setup                */

	mov	x0,  #(SCR_RES1_BITS | SCR_RW_BIT)
	msr	scr_el3, x0                /* bl1_arch_setup                   */ 


	bl	bios_memory_mmu_setup      /* Static configuration of MMU      */ 

	bl	bl2_early_platform_setup   /* Hikey late Setup                 */

	bl	bios_high_runtime          /* Pass control to module bios_high */ 



loop:                                     /* Infinite loop     */ 
	wfi 
	b loop	



        /* ---------------------------------------------
         * Assoicate source code with binary for traceability 
         * ---------------------------------------------
         */

        .section ".keywords"
        bios_low_entrypoint_ident_url: .string "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_low/branches/haspoc/arm64_v8_sb/bios_low/arm64_v8_dev-1.1/bl1/aarch64/bl1_entrypoint.S $"
        bios_low_entrypoint_ident_rev: .string "$Revision: 26799 $"


