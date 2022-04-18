/*! \file dir.h
    \brief Directory Functions


    Header file for an API that provides directory manipulation
           routines in a platform-independent manner.
                
*/

/*
 Copyright Oniteo 2007 - 
*/  
#ifndef __PMILL_WINDOWS_DIRREAD_HEADER__
#define __PMILL_WINDOWS_DIRREAD_HEADER__ "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/dir.h $ $Revision: 8139 $"


/*! \brief Maximum pathname of directory
 
 */ 
#define PLF_MAX_DIRECTORY_PATH_LENGTH 256 

/*! \brief Directory handle
    \ingroup DIR
 */    
typedef void* HDIRECTORY;


/*! \brief Directory entry structure.
    \ingroup DIR
 */    
struct directory_entry
{
   char  d_name[PLF_MAX_DIRECTORY_PATH_LENGTH];      //!< File name of entry (path excluded).
   void* reserved;         //!< Reserved for internal use.
};


#ifdef __cplusplus
extern "C" {
#endif



/*! \name Directory
 
    @{

*/

/*! \brief Opens a directory.
    \ingroup DIR
    \param[in] pszDirName Name of the directory to open.
    \return HDIRECTORY, or NULL on error.

    This function is a Windows-compatible version of opendir.
 */
HDIRECTORY NDirOpen(const char* pszDirName);

/*! \brief Reads directory content.
    \ingroup DIR
    \param[in] d Directory pointer.
    \param[in] e Directory entry structure.
    \return pointer to directory entry structure.

    Windows-compatible version of readdir.
 */
struct directory_entry* NDirRead(HDIRECTORY d, struct directory_entry* e);

/*! \brief Closes a directory.
    \ingroup DIR
    \param[in] hdir Pointer to the directory handle.
    \return 0 on success or -1 on error.

    This function is a Windows-compatible version of closedir.
 */
int NDirClose(HDIRECTORY hdir);

/*! \brief Resolves a file link.
    \ingroup DIR
    \param[in] linkfile The link file to resolve.
    \param[out] targetfile Buffer to receive the resolved file.
    \param[in] size Size of target buffer.
    \return number of bytes copied to targetfile, or a negative value on error.

    This function resolves a link and returns the actual path to the file.
 */    
int NDirResolveLink(const char* linkfile, char* targetfile, int size);

/*! \brief Removes a directory.
    \ingroup DIR
    \param[in] dirname Name of the directory to remove.
    \param[in] recursive Pass non-zero to perform a recursive removal, else zero.
    \return 0 on success or -1 on error.

    This function removes a directory.
 */    
int NDirRemove(const char* dirname, int recursive);

/*! \brief Returns base component of a pathname.
    \ingroup DIR
    \param[in] path The complete pathname.
    \return file component of path.

    This function returns the leaf component of a pathname. 
 */    
char *NBaseName(const char* path);

// @}

#ifdef __cplusplus
}
#endif


#endif

