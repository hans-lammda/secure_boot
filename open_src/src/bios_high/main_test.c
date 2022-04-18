#include <plf/platform.h>

/* Dependency on Linux */ 

#include <sys/stat.h>
#include <fcntl.h>
#include <bios_high.h>
#include <bios_low.h>
#include <bios_memory.h>
#include <fip.h>


#define DBG_LEVEL 8

int NMemPool(char *start, size_t size, int test )
{
  return 1; 
}

int setup_sdram(uint64_t start, uint32_t size)
{
  return SDRAM_SETUP_OK; 
}
int bl2main(uint32_t e_level, uint64_t *bl31_load_address, int bl31_size,  uint64_t *bl33_load_address,  int bl33_size, uint64_t  bl33_X0, uint64_t  bl33_X1)
{
  fprintf(stdout,"e_level\t%d\n", e_level);
  fprintf(stdout,"bl31_load_address\t0x%x\n", bl31_load_address);
  fprintf(stdout,"bl33_load_address\t0x%x\n", bl33_load_address);
  fprintf(stdout,"bl33_size\t%d\n", bl33_size);
  fprintf(stdout,"bl31_size\t%d\n", bl31_size);
  // fprintf(stdout,"bl33_X0\t0x%x\n", bl33_X0);
  // fprintf(stdout,"bl33_X1\t0x%x\n", bl33_X1);
  return FIP_ERROR_BOOT_FAILED; 
}
void plf_memcpy16( void* a, void* b, int c)
{
  return ; 
}

static uint8_t *image; 
static uint32_t image_size; 

int init_mmc(void)
{
  return MMC_INIT_OK; 
}


int sector_read(unsigned int sector, unsigned int nr, unsigned char *buffer)
{
  uint32_t offset ;
  void *src; 
  
  trace_dl(DBG_LEVEL,"sector %d  nr %d\n",sector,nr);

  if (  nr  > image_size ) {
     fprintf(stderr,"Attempt to read sector [%d] beyond partion size [%d]\n",nr,image_size);
     return SECTOR_READ_ERROR;
   }
  offset =  BLOCK_SIZE * nr ;
  src = image  ;
  NMemCopy(buffer, src,  offset );
  return SECTOR_READ_OK; 
}


int main(int argc, char *argv[])
{
	int  rc,i; 
	int fp;
        struct stat stat_p;

        if ( argc != 2 ) {
  	  // trace_dl(DBG_LEVEL,"Usage: %s: hsbf_image \n",argv[0]);
    	  return 1;
  	}


	fp = open(argv[1],O_RDONLY);
        if (fp <  0 )
         return 1;

	if ( fstat(fp, &stat_p) == -1 ) {
          return 1;
        }
        image_size = (stat_p.st_size / BLOCK_SIZE ) + 1 ; 
  	trace_dl(DBG_LEVEL,"%d\n", image_size);


	image = mmap(NULL, stat_p.st_size, PROT_READ, MAP_PRIVATE, fp, 0);

	if (image  ==  NULL )
          return 1;


        rc = _main(BOOT_START_SECTOR,INITIAL_READ);
	fprintf(stdout,"rc\t%d\n", rc);
        return 0;
}

