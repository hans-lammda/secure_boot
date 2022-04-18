#include <plf/platform.h>
#include <plf/memory.h>
#include <plf/misc.h>
#include <vfs.h>
#include <fat.h>


int external_init(FP handle,init_data *data)
{
  return 0; 
}
int external_read(FP handle, unsigned int sector, unsigned int nr, unsigned char *buffer)
{
  return 0; 
}

