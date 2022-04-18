#include <plf/platform.h>
#include <plf/memory.h>
#include <plf/safestr.h>
#include <plf/optimized.h>
#include <spd/spd.h>
#include <fip.h>
#include <spd/spd.h>
#include <bios_memory.h>
#include <bios_low.h>
#include "bios_high.h"


#ifdef TESTMODE
#  define MemCopy(dest, src, count)                                  /* No copy during test */ 
#  define XMemCopy(dest, src, count)                                 /* copy 128 bit instead of 8  */ 
#else
#  ifdef ARM64 
#    define MemCopy(dest, src, count) (FMemCopy(dest,src,count))     /* copy 32 bit instead of 8  */ 
#    define XMemCopy(dest, src, count) (plf_memcpy16(dest,src,count))     /* copy 128 bit instead of 8  */ 
#  else
#    define MemCopy(dest, src, count) (NMemCopy(dest,src,count))     /* Generic portabel code */
#    define XMemCopy(dest, src, count) (NMemCopy(dest,src,count))     /* Generic portabel code */  
#  endif
#endif


const uint8_t main_code_version[]    __attribute__ ((section (".keywords")))     = \
       "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_high/branches/haspoc/hikey/bios_high/main.c $ $Revision: 27062 $";

const uint8_t main__header_version[]  __attribute__ ((section (".keywords")))     =  __ONITEO_BIOS_HIGH_H__;


#define DBG_LEVEL          1
#define DBG_LEVEL_VERBOSE  3

#define INITIAL_READ 300
#define BOOT_START_SECTOR 8192


int _main(int boot_start_sector, int initial_read)
{
	uint32_t  len,rc; 
        uint32_t bl31_size = 0 , bl33_size = 0 ; 
	unsigned char *buf; 
	struct sobject  *trust_root;
	struct sobject *tp, *sp, *end;
        uint64_t heap_start = 0 ; 
        uint32_t heap_size = 0 ; 
        uint32_t image_size; 
      
	trace_dl(DBG_LEVEL,"SUITE_BUILD_ID %s\n", SUITE_BUILD_ID);

	 /* install hash used to verify ROOT_PK public key */
        tp =  get_root_pk_hash();
        if (  tp == NULL ) {
             trace_dl(DBG_LEVEL,"\n\nFailed to retrieve ROOT_PK_HASH \n\n\n");
             return BIOS_HIGH_FAILED_RETRIEVE_ROOT_PK_HASH ;
        }
        trace_d(DBG_LEVEL,"ROOT_PK_HASH retreved\n");

        rc = install_key(&trust_root, tp);
        if (  rc != SPD_OK ) {
             trace_dl(DBG_LEVEL,"\n\nFailed to install ROOT_PK_HASH %d\n\n\n",rc);
             return BIOS_HIGH_FAILED_INSTALL_ROOT_PK_HASH ;
        }
        trace_d(DBG_LEVEL,"ROOT_PK_HASH installed\n");


	if ( (buf = NMemAlloc_align(INITIAL_READ*BLOCK_SIZE, 8)) == NULL) {
	  trace_dl(DBG_LEVEL,"Failed to allocate memory\n");
          return BIOS_HIGH_MALLOC_SRAM_FAILED; 
        }

        rc = init_mmc(); 
        if (  rc != MMC_INIT_OK ) {
             trace_dl(DBG_LEVEL,"\n\nFailed to configure mmc  %d\n\n\n",rc);
             return BIOS_HIGH_FAILED_INIT_STORAGE ;
        }

        /* Assign size if file system used , otherwise use default value */ 
        image_size = HSBF_SIZE_UNKNOWN;
	rc = sector_read(boot_start_sector, initial_read, buf);
        if ( rc != SECTOR_READ_OK ) {
	  trace_dl(DBG_LEVEL,"Bad storage read %d\n",rc);
          return BIOS_HIGH_BAD_STORAGE_READ; 
        }
	sp = (struct sobject *) buf; 

        if ( sp->type == ROOT_PK) {
          trace_d(DBG_LEVEL,"ROOT_PK found in HSBF (pass1)\n");
          if ( ( rc = verify_element(trust_root, sp)) != SPD_OK ) {
             trace_dl(DBG_LEVEL,"\n\nROOT_PK Failed to verify %d\n\n\n",rc);
             return BIOS_HIGH_FAILED_VERIFY_ROOTPK ;
          }
          else {
             trace_dl(DBG_LEVEL,"\n\nROOT_PK verified\n\n\n");
             rc = install_key(&trust_root, sp);
             if (  rc != SPD_OK ) {
               trace_dl(DBG_LEVEL,"\n\nFailed to install ROOT_PK_HASH %d\n\n\n",rc);
               return BIOS_HIGH_FAILED_INSTALL_ROOT_PK_HASH ;
             }
             trace_d(DBG_LEVEL,"ROOT_PK installed \n");
             trace_d(DBG_LEVEL,"HSBF Image ID=[%s]\n",sp->p.rk_p->id);
          }
          sp = (struct sobject* ) sp->next;
        }
        else {
            trace_dl(DBG_LEVEL,"Expected ROOT_PK %d %d\n",ROOT_PK,sp->type);
            return BIOS_HIGH_ROOTPK_NOT_FOUND ;
        }


	if ( sp != NULL && sp->type == SDRAM) {
         trace_d(DBG_LEVEL,"SDRAM found in HSBF (pass1)\n");
         if ( ( rc = verify_element(trust_root, sp)) != SPD_OK ) {
           trace_dl(DBG_LEVEL,"Failed to verify %d\n",rc);
           return BIOS_HIGH_FAILED_VERIFY_DRAM ;
         }
         else {
	   heap_start =  sp->p.sdp->heap_start; 
           heap_size  =  sp->p.sdp->heap_size; 
           trace_dl(DBG_LEVEL,"About to assign %d bytes starting at 0x%x for heap\n",heap_size, heap_start);
           trace_d(DBG_LEVEL,"SDRAM Verified image_size = %d/%d\n",image_size,sp->p.sdp->image_size);
           if ( image_size == HSBF_SIZE_UNKNOWN ) {
              if ( sp->p.sdp->image_size != HSBF_SIZE_UNKNOWN ) 
                 image_size = sp->p.sdp->image_size ;
              else
                 return BIOS_HIGH_UNKNOWN_SIZE_HSBF; 
           }
           if ( setup_sdram(sp->p.sdp->start,sp->p.sdp->size) != SDRAM_SETUP_OK ) 
                 return BIOS_HIGH_SDRAM_SETUP_ERROR; 
             
         }
         sp = (struct sobject* ) sp->next;
        }
        else {
            trace_dl(DBG_LEVEL,"Expected SDRAM %d %d\n",SDRAM,sp->type);
            return BIOS_HIGH_DRAM_NOT_FOUND ;
        }


        /* 
         Assign heap to SDRAM and read entire HSBF image 
        */ 

	NMemPool((char *) heap_start, heap_size, 1);
	if ( (buf = NMemAlloc_align(image_size + 512, 8)) == NULL) {
	  trace_dl(DBG_LEVEL,"Failed to allocate memory\n");
          return BIOS_HIGH_MALLOC_DRAM_FAILED; 
        }

	trace_dl(DBG_LEVEL,"%d bytes allocated, about to read from eMMC into 0x%x\n",image_size,buf);
	rc = sector_read(boot_start_sector, (image_size/BLOCK_SIZE) + 1 ,buf);
        if ( rc != SECTOR_READ_OK ) {
	  trace_dl(DBG_LEVEL,"Bad storage read %d\n",rc);
          return BIOS_HIGH_BAD_STORAGE_READ; 
        }
	sp =  (struct sobject *) buf; 

        end = ( struct sobject * ) (  (uint64_t) sp +  (uint64_t) image_size ) ; 
        len = image_size ;

        while (len > 0 ) {
          trace_dl(DBG_LEVEL_VERBOSE,"HSBF processing node of type %d len=%d\n", sp->type, len);
          switch  (sp->type ) {
            case ROOT_PK    :
                                  rc = verify_element(trust_root, sp);
                                  trace_dl(DBG_LEVEL,"\n\nROOT_PK Ignored %d\n\n",rc);
                                  break;
            case SDRAM :
                                  rc = verify_element(trust_root, sp);
                                  trace_dl(DBG_LEVEL,"\n\nSDRAM Ignored %d\n\n",rc);
                                  break ;

            case ATF_BOOT :
                                  trace_d(DBG_LEVEL,"ATF_BOOT found in HSBF\n");
                                  if ( ( rc = verify_element(trust_root, sp)) != SPD_OK ) {
                                    trace_dl(DBG_LEVEL,"Failed to verify %d\n",rc);
                                    return BIOS_HIGH_FAILED_VERIFY_ATF_BOOT ;
                                  } 
                                  else {
                                   trace_dl(DBG_LEVEL,"verify ok %d\n",rc);
	                           rc = bl2main(sp->p.bl2_p->e_level, 
                                                 (uint64_t *) sp->p.bl2_p->bl31_load_address, 
						 bl31_size,
                                                 (uint64_t *) sp->p.bl2_p->bl33_load_address,
						 bl33_size,
                                                 sp->p.bl2_p->bl33_X0,
                                                 sp->p.bl2_p->bl33_X1);
                                    trace_dl(DBG_LEVEL,"Failed to boot %d\n",rc);
                                    return BIOS_HIGH_FAILED_ATF_BOOT ;
                                  }
				  break;

	    case RAMDISK    :
				  trace_d(DBG_LEVEL,"RAMDISK found in HSBF\n");
                                  if ( ( rc = verify_element(trust_root, sp)) != SPD_OK ) {
                                    trace_dl(DBG_LEVEL,"Failed to verify %d\n",rc);
                                    return BIOS_HIGH_FAILED_VERIFY_RAMDISK ;
                                  } 
                                  else {
                                   trace_dl(DBG_LEVEL,"verify ok %d\n",rc);
				   XMemCopy((void*) sp->p.rp->load_address, (void*) sp->p.rp->start, sp->p.rp->size);
                                  }

				  break;
	    case ATF_RUNTIME  :
				  trace_d(DBG_LEVEL,"ATF_RUNTIME found in HSBF\n");
                                  if ( ( rc = verify_element(trust_root, sp)) != SPD_OK ) {
                                    trace_dl(DBG_LEVEL,"Failed to verify %d\n",rc);
                                    return BIOS_HIGH_FAILED_VERIFY_ATF_RUNTIME ;
                                  } 
                                  else {
                                   trace_dl(DBG_LEVEL,"verify ok %d\n",rc);
				   MemCopy((void*) sp->p.atf_runtime_p->bl31_load_address, (void*) sp->p.rp->start, sp->p.rp->size);
				   bl31_size = sp->p.rp->size; 
                                  }

				  break;
	    case ATF_PAYLOAD  :
				  trace_d(DBG_LEVEL,"ATF_PAYLOAD found in HSBF\n");
                                  if ( ( rc = verify_element(trust_root, sp)) != SPD_OK ) {
                                    trace_dl(DBG_LEVEL,"Failed to verify %d\n",rc);
                                    return BIOS_HIGH_FAILED_VERIFY_ATF_PAYLOAD ;
                                  } 
                                  else {
                                   trace_dl(DBG_LEVEL,"verify ok %d\n",rc);
				   MemCopy((void*) sp->p.atf_payload_p->bl33_load_address, (void*) sp->p.rp->start, sp->p.rp->size);
				   bl33_size = sp->p.rp->size; 
                                  }

				  break;
	    default         :
                                  return BIOS_HIGH_FAILED_UNKNOWN_OBJECT;
                                  break;
             }
             if ( len > 0 ) {
               sp = (struct sobject* ) sp->next;
               len = (uint64_t) end -  (uint64_t) sp  ;
             }
        } 
        return BIOS_HIGH_FAILED_NO_BOOT_OBJECT;
}

