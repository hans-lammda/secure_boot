


        .globl  bios_output_setup_perf_timers


.macro func _name
        .section .text.\_name, "ax"
        .type \_name, %function
        \_name:
        .endm

/*
 Measure boot time
 The system pll is not running at full speed yet
 Initialize cycle counter
 See document: ARMv8-ARM D7.4.7: C[2]=1, P[1]=1, E[0]=1

 */
func bios_output_setup_perf_timers
	ldr x0, =0x5
	msr pmcr_el0, x0        // Cycle counter reset
	ldr x0, =0x80000000
	msr pmcntenset_el0, x0  // Cycle counter enable, time count starts here
        ret  


       .section ".keywords"
        bios_output_perf_ident_url: .string "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_output/branches/haspoc/arm64_v8/bios_output/performance.S $"
        bios_output_perf_ident_rev: .string "$Revision: 26649 $"


