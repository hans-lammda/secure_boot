#include <plf/platform.h>
#include <plf/memory.h>
#include <plf/safestr.h>
#define BUFLEN 1024
int main() 
{
  char *buf;
  char *buf2;
  char *gurka = "gurka";
  char *dup_gurka;
  char *dup_buf;
  char *p,*p2;
  int i;
  /* Allocate a memory segment */
  if ((buf = NMemAlloc(2*BUFLEN)) == NULL)
    return 1;
  /* Reallocate the segment and make it smaller */
  if ((buf = NMemRealloc(buf, BUFLEN)) == NULL)
    return 1;
  /* Fill the memory */
  for (i = 0, p = buf; i < BUFLEN-1; i++,p++)
    *p = (char)i;
  /* Allocate another segment */
  if ((buf2 = NMemAlloc(BUFLEN)) == NULL)
    return 1;
  /* Copy the first segment to the second segment */
  if (NMemCopy(buf2,buf,BUFLEN) == NULL)
    return 1;
  /* Verify the two segments are equal */
  for (i = 0,p = buf,p2 = buf2; i < BUFLEN-1; i++,p++,p2++)
    if (*p != *p2)
      return 1;
  /* See if the segment is readable */
  if (NMemReadable(buf,BUFLEN))
    return 1;
  /* See if the segment is writeable */
  if (NMemWriteable(buf,BUFLEN))
    return 1;
  /* Create a string in a memory segment */
  NStrSCopy(buf,gurka,BUFLEN-1);
  /* and duplicate the string */
  if ((dup_gurka = NStrDup(gurka)) == NULL)
    return 1;
  /* Verify the two strings are the same */
  if (strcmp(buf,dup_gurka) != 0)
    return 1;
  /* Just duplicate a memory segment */
  if ((dup_buf = NMemDup(buf,BUFLEN)) == NULL)
    return 1;
  /* Free all allocated buffers */
  NMemFree(dup_gurka);
  NMemFree(dup_buf); 
  NMemFree(buf);
  NMemFree(buf2);
  printf("Memory handling OK.\n"); 
  return 0;
}
