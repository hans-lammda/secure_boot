#define ENTRY_POINT_INFO_ARGS_OFFSET    0x18



    .section ".keywords"
    bios_fip_start_bl31_ident_url:     .string "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_fip/trunk/bios_fip/boot_bl31.S $"
    bios_fip_start_bl31_ident_rev:     .string "$Revision: 26031 $"



 /* extern void __boot_bl31(uint64_t *x0, uint64_t *bl31_entry); */

    .text
    .global __boot_bl31

__boot_bl31:
	
    mov     x20, x0
    mov     x21, x1
    bl display_boot_progress
    bl      disable_mmu_icache_el3
    tlbi    alle3
    ldp     x0, x1, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET)]
    blr x21

