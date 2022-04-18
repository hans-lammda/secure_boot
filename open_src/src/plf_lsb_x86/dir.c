#include <plf/platform.h>
#include <plf/dir.h>
#include <plf/memory.h>
#include <plf/safestr.h>


char plf_dir_header_version[] = __PMILL_WINDOWS_DIRREAD_HEADER__;
const char plf_dir_header_plf[] = __ONITEO_PLATFORM_H__;
char plf_dir_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_lsb_x86/trunk/plf_lsb_x86/dir.c $ $Revision: 16123 $";


HDIRECTORY NDirOpen(const char* pszDirName)
{
   return (HDIRECTORY) opendir(pszDirName);
}

struct directory_entry* NDirRead(HDIRECTORY dIn,struct directory_entry* e)
{
   DIR* d = (DIR*) dIn;
   struct dirent* ex;
   
   if (!e)
      return NULL;
   
   if (NULL==(ex = readdir(d)))
      return NULL;
      
   if (e)   
      NStrSCopy(e->d_name,ex->d_name,sizeof(e->d_name));
      
   return e;
}

char *NBaseName(const char *path)
{
 const char *base ; 

 if ( path == NULL ) 
  return ""; 

 for ( base = path ; *path ; path++ ) {
   if ( *path == FILEPATH_SEP ) 
      base = path + 1;  
 } 
 return (char *) base ; 
}

int NDirClose(HDIRECTORY dIn)
{
  DIR* d = (DIR*) dIn;
  if (dIn == NULL)
    return -1;
  return closedir(d);
}

int NDirResolveLink(const char* linkfile, char* targetfile, int size)
{
   int rc;

   memset(targetfile,0,size);
   rc = readlink(linkfile, targetfile, size);
   if (rc>0)
      return rc;
   else
      return -1;
}


/* --------------------------------------------------------------------
 * DIRECTORY REMOVAL CODE 
 * --------------------------------------------------------------------
 */

struct remove_info
{
   int is_dir;                         //!< 1=directory, 2=file or link
   char name[256];                     //!< Name of this item (no path)

   struct remove_info* next;           //!< Next directory entry.
};

static void __free_remove_info(struct remove_info* info)
{
   struct remove_info* p = info;

   while (p)
   {
      p = info->next;
      NMemFree(info);
      info = p;
   }
}

int NDirRemove(const char* dirname, int recursive)
{
   int rc;
   HDIRECTORY hDir;
   struct stat st;
   struct remove_info* root = NULL;
   struct remove_info* current = NULL;
   struct remove_info* p = NULL;
   char szLink[2048];
   char fullpath[32768];
   
   struct directory_entry e;

   
#ifdef _DEBUG
   fprintf(stderr,"NDirRemove recursive removal [%s]\n",dirname);
   sleep(1);
#endif

   if (!recursive)
      return rmdir(dirname);
   
   hDir = NDirOpen(dirname);
   if (!hDir)
   {
#ifdef _DEBUG
      fprintf(stderr,"NDirRemove: Error opening %s as directory.\n",dirname);
#endif
      return -1;
   }

   while (NDirRead(hDir,&e))
   {
      if (strcmp(e.d_name,".")==0 || strcmp(e.d_name,"..")==0)
         continue;
         
#ifdef _DEBUG
      fprintf(stderr,"NDirRemove object  [%s]\n",e.d_name);
#endif
      p = (struct remove_info*) NMemAlloc(sizeof(struct remove_info));
      p->is_dir = 0; 
      current = p;

      p->next = root;
      root=p;

      NStrSCopy(current->name,e.d_name,sizeof(current->name));
      NStrSPrintf(fullpath,sizeof(fullpath),"%s%c%s",dirname,FILEPATH_SEP,e.d_name);

#ifdef _DEBUG
      fprintf(stderr,"current->name  [%s]\n",current->name);
#endif
      // Avoid traversing links (treat as ordinary files)
      if (NDirResolveLink(fullpath,szLink,sizeof(szLink)) == -1)
      {
         if (!stat(fullpath,&st))
         {
            if (S_ISDIR(st.st_mode))
               current->is_dir = 1;
         }               
      }
   }
   
   if (hDir)
   {
      NDirClose(hDir);
      hDir = NULL;
   }
   
   current = root;
   while (current)
   {
      NStrSPrintf(fullpath,sizeof(fullpath),"%s%c%s",dirname,FILEPATH_SEP,current->name);

#ifdef _DEBUG
      fprintf(stderr,"fullpath  [%s]\n",fullpath);
#endif
      if (current->is_dir)
         rc = NDirRemove(fullpath,1);
      else
         rc = unlink(fullpath);
         
      if (rc<0)
      {
         fprintf(stderr,"NDirRemove: Error removing %s %s\n",current->is_dir?"directory":"file",fullpath);
         __free_remove_info(root);
         return -1;
      }

      current = current->next;
   }

   __free_remove_info(root);
   return rmdir(dirname);
}


