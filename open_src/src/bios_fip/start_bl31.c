#include <plf/platform.h>
#include <plf/memory.h>
#include <plf/io.h>
#include <spd/spd.h>
#include <fip.h>
#include <bios_memory.h>

const char bios_fip_start_bl31_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_fip/trunk/bios_fip/start_bl31.c $ $Revision: 26990 $";

#define DBG_LEVEL          1

extern uint64_t __PARAMS_BASE__; 
#define PARAMS_BASE  (uint64_t)(&__PARAMS_BASE__)



#define SET_PARAM_HEAD(_p, _type, _ver, _attr) do { \
        (_p)->h.type = (uint8_t)(_type); \
        (_p)->h.version = (uint8_t)(_ver); \
        (_p)->h.size = (uint16_t)sizeof(*_p); \
        (_p)->h.attr = (uint32_t)(_attr) ; \
        } while (0)


#define SECURE          0x0
#define NON_SECURE      0x1
#define PARAM_EP_SECURITY_MASK    0x1


#define SET_SECURITY_STATE(x, security) \
                        ((x) = ((x) & ~PARAM_EP_SECURITY_MASK) | (security))

#define MODE_EL1                0x1
#define MODE_EL2		0x2
#define MODE_SP_ELX		0x1

#define DAIF_FIQ_BIT            (1 << 0)
#define DAIF_IRQ_BIT            (1 << 1)
#define DAIF_ABT_BIT            (1 << 2)
#define DAIF_DBG_BIT            (1 << 3)


#define DISABLE_ALL_EXCEPTIONS (DAIF_FIQ_BIT | DAIF_IRQ_BIT | DAIF_ABT_BIT | DAIF_DBG_BIT)


#define MODE_RW_64              0x0
#define MODE_RW_MASK            0x1
#define MODE_RW_SHIFT           0x4
#define MODE_EL_MASK            0x3
#define MODE_EL_SHIFT           0x2
#define MODE_SP_MASK            0x1
#define MODE_SP_SHIFT           0x0
#define SPSR_DAIF_SHIFT         6
#define SPSR_DAIF_MASK          0xf


#define SPSR_64(el, sp, daif)                           \
        (MODE_RW_64 << MODE_RW_SHIFT |                  \
        ((el) & MODE_EL_MASK) << MODE_EL_SHIFT |        \
        ((sp) & MODE_SP_MASK) << MODE_SP_SHIFT |        \
        ((daif) & SPSR_DAIF_MASK) << SPSR_DAIF_SHIFT)

int bl31_setup(void)
{
        trace_d(DBG_LEVEL,"Post Platform config\n");
	return 1; 
}


int bl2main(uint32_t e_level, uint64_t *bl31_load_address, int bl31_size,  uint64_t *bl33_load_address,  int bl33_size, uint64_t  bl33_X0, uint64_t  bl33_X1)
{

	bl31_params_t *bl2_to_bl31_params;
        entry_point_info_t *bl31_ep_info;

        /* Assign pointer to reserved memory */ 
	bl2_to_bl31_params_mem_t *bl31_params_mem = (bl2_to_bl31_params_mem_t *)PARAMS_BASE;

	NMemSet(bl31_params_mem,0,sizeof(bl2_to_bl31_params_mem_t)); 

        /* pointer to to connect members  */ 
        bl2_to_bl31_params = &bl31_params_mem->bl31_params;

        /* passed as X0 to when eret calls bl31 */ 
        bl31_ep_info = &bl31_params_mem->bl31_ep_info;

        /* connect members  */ 
        bl2_to_bl31_params->bl31_image_info = &bl31_params_mem->bl31_image_info;
        bl2_to_bl31_params->bl33_ep_info = &bl31_params_mem->bl33_ep_info;
        bl2_to_bl31_params->bl33_image_info = &bl31_params_mem->bl33_image_info; 


        /* type and version assignment  */ 
        SET_PARAM_HEAD( bl2_to_bl31_params,                  PARAM_BL31,         VERSION_1, 0);
        SET_PARAM_HEAD( bl2_to_bl31_params->bl31_image_info, PARAM_IMAGE_BINARY, VERSION_1, 0);
        SET_PARAM_HEAD( bl2_to_bl31_params->bl33_ep_info,    PARAM_EP,           VERSION_1, 0);
        SET_PARAM_HEAD( bl2_to_bl31_params->bl33_image_info, PARAM_IMAGE_BINARY, VERSION_1, 0);

       	trace_d(DBG_LEVEL,"Parameter block written at 0x%x\n",PARAMS_BASE);

        /* Simple check , could also check allowed memory range */ 
	if ( bl31_load_address == 0)  
	    return FIP_ERROR_BL31_INVALID_LOAD_ADDRESS; 

	if ( bl31_size > 0  ) {
                /* X0 assigned &bl31_params_mem->bl31_params  */ 
                bl31_ep_info->args.arg0 = (unsigned long) bl2_to_bl31_params;
	        bl31_ep_info->args.arg1 = BL31_PLAT_PARAM_VAL;
                bl31_ep_info->spsr = BL31_SPSR_STATE;
                /* eret link register   */ 
                bl31_ep_info->pc = (uint64_t) bl31_load_address;
                flush_dcache_range((uint64_t) bl31_load_address, bl31_size); 
       	        trace_d(DBG_LEVEL,"Flushed start 0x%x %d bytes \n",bl31_load_address, bl31_size);
  	}
        else {
       	  trace_d(DBG_LEVEL,"FIRMWARE_BL31 not found\n");
          return FIP_ERROR_BL31_NOT_FOUND; 
        }


        /* Simple check  */ 
	if ( bl33_load_address == 0)  
	    return FIP_ERROR_BL33_INVALID_LOAD_ADDRESS; 

	if ( bl33_size > 0 ) {
                SET_SECURITY_STATE(bl2_to_bl31_params->bl33_ep_info->h.attr, NON_SECURE);
                bl2_to_bl31_params->bl33_ep_info->spsr = SPSR_64(e_level, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS); 
                bl2_to_bl31_params->bl33_ep_info->pc =  (uint64_t) bl33_load_address; 

                bl2_to_bl31_params->bl33_ep_info->args.arg0 = bl33_X0; 
                bl2_to_bl31_params->bl33_ep_info->args.arg1 = bl33_X1; 
       	        trace_d(DBG_LEVEL,"bl33_X0 0x%x\n",bl2_to_bl31_params->bl33_ep_info->args.arg0);
       	        trace_d(DBG_LEVEL,"bl33_X1 0x%x\n",bl2_to_bl31_params->bl33_ep_info->args.arg1);
                flush_dcache_range((uint64_t) bl33_load_address, bl33_size); 
       	        trace_d(DBG_LEVEL,"Flushed start 0x%x %d bytes \n",bl33_load_address, bl33_size);

  	}
        else {
       	  trace_d(DBG_LEVEL,"FIRMWARE_BL33 not found\n");
          return FIP_ERROR_BL33_NOT_FOUND; 
        }


        bl31_setup(); 

       	trace_d(DBG_LEVEL,"About to boot\n");

	__boot_bl31((uint64_t *) bl31_ep_info,(uint64_t *) bl31_load_address); 

	return FIP_ERROR_BOOT_FAILED; 

}




void display_boot_progress(entry_point_info_t *bl31_ep_info)
{

	bl2_to_bl31_params_mem_t *bl31_params_mem = (bl2_to_bl31_params_mem_t *) bl31_ep_info->args.arg0;
	uint64_t time, freq;

        trace_d(DBG_LEVEL,"\n\nSecure boot\n\n");
        trace_d(DBG_LEVEL,"BL1:\n");
        trace_d(DBG_LEVEL,"BL1: BL3-1 address  = 0x%x\n",(unsigned long long)bl31_ep_info->pc);
        trace_d(DBG_LEVEL,"BL1: BL3-1 spsr     = 0x%x\n",(unsigned long long)bl31_ep_info->spsr);
        trace_d(DBG_LEVEL,"BL1: BL3-1 E-level  = %d\n",((unsigned long long) bl31_ep_info->spsr & 0x0000000c )>>2);
        trace_d(DBG_LEVEL,"BL1: BL3-1 X0       = 0x%x\n",(unsigned long long)bl31_ep_info->args.arg0);
        trace_d(DBG_LEVEL,"BL1: BL3-1 X1       = 0x%x\n",(unsigned long long)bl31_ep_info->args.arg1);
        trace_d(DBG_LEVEL,"BL33\n");

        trace_d(DBG_LEVEL,"BL1: BL3-3 address  = 0x%x\n",(unsigned long long)bl31_params_mem->bl33_ep_info.pc);
        trace_d(DBG_LEVEL,"BL1: BL3-3 spsr     = 0x%x\n",(unsigned long long)bl31_params_mem->bl33_ep_info.spsr);
        trace_d(DBG_LEVEL,"BL1: BL3-3 E-level  = %d\n",((unsigned long long) bl31_params_mem->bl33_ep_info.spsr & 0x0000000c )>>2);
        trace_d(DBG_LEVEL,"BL1: BL3-3 X0       = 0x%x\n",(unsigned long long)bl31_params_mem->bl33_ep_info.args.arg0);
        trace_d(DBG_LEVEL,"BL1: BL3-3 X1       = 0x%x\n\n",(unsigned long long)bl31_params_mem->bl33_ep_info.args.arg1);

        time = read_pmccntr_el0(); 
	freq = read_cntfrq_el0(); 
	trace_d(DBG_LEVEL,"Exit Elapsed time(ms): %d\n\n",(time/(freq/1000)));
}


