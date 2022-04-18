#include <plf/platform.h>
#include <plf/dir.h>
int main() 
{
  char *dir1 = "/tmp/level1";
  char *dir2 = "/tmp/level1/level2";
  char *fn = "/tmp/level1/file";
  char *files[] = {".","..","level2","file"};
  const int recursive = 1;
  FILE *fp;
  HDIRECTORY d;
  int i;
  /* Cleanup from previuos run */
  /* Open the directory in tmp */
  d = NDirOpen(dir1);
  if (d) {
    /* If it exist, remove it recursively */
    if (NDirRemove(dir1,recursive) < 0)
      return 1;
    NDirClose(d);
  }
  /* The directory we just removed should not be there */
  if (NDirOpen(dir1))
    return 1;
  /* Create directories and file */
  if (mkdir(dir1,0777) == -1)
    return 1;
  if (mkdir(dir2,0777) == -1)
    return 1;
  if ((fp = fopen(fn,"w")) == NULL)
    return 1;
  fclose(fp);
  /* Now the directory and its subdirectory and file should be there */
  d = NDirOpen(dir1);
  if (d == NULL)
    return 1;
  /* Go through the directory and verify it's entries */
  struct directory_entry e;
  while (NDirRead(d, &e) != NULL) {
    for (i = 0;i < 4; i++)
      if (strcmp(files[i], e.d_name) == 0)
	break;
    if (i == 4)
      return 1;
  }
  /* Close the directory */
  if (NDirClose(d) < 0)
    return 1;
  /* Try to close a NULL directory */
  d = NULL;
  if (NDirClose(d) == 0)
    return 1;
  /* Remove the subdirectory */
  if (NDirRemove(dir2,0) < 0)
    return 1;
  /* Try to remove the directory with contents */
  /* It should not work */
  if (NDirRemove(dir1,0) >= 0)
    return 1;
  /* Remove the directory recursively */
  if (NDirRemove(dir1,recursive) < 0)
    return 1;
  /* Get filename part in some relative pathnames */
  if ( strcmp(NBaseName("a/b/c"),"c") != 0 ) 
    return 1;
  if ( strcmp(NBaseName("a/b"),"b") != 0 ) 
    return 1;
  if ( strcmp(NBaseName("a"),"a") != 0 ) 
    return 1;
  if ( strcmp(NBaseName(""),"") != 0 ) 
    return 1;
  if ( strcmp(NBaseName(NULL),"") != 0 ) 
    return 1;
  /* Get filename part in some absolute pathnames */
  if ( strcmp(NBaseName("/"),"") != 0 ) 
    return 1;
  if ( strcmp(NBaseName("//"),"") != 0 ) 
    return 1;
  if ( strcmp(NBaseName("/a/b/c"),"c") != 0 ) 
    return 1;
  if ( strcmp(NBaseName("/a"),"a") != 0 ) 
    return 1;
  printf("Directory handling OK\n");
  return 0;
}
