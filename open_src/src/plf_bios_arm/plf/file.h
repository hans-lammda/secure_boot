/*! \file file.h
    \brief File Functions


    Header file for an API that provides file manipulation
           routines in a platform-independent manner.
                
*/

/*
 Copyright Oniteo 2007 - 
*/  
#ifndef __PLF_FILE_HEADER__
#define __PLF_FILE_HEADER__ "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/file.h $ $Revision: 17763 $"


#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Successfull operation 
 
 */
#define PLF_OK             0 

/*! \brief Unable to open source file 

 */
#define PLF_NO_SOURCE      1 

/*! \brief Unable to open target file 

 */
#define PLF_NO_TARGET      2 

/*! \brief Error during file write 

 */
#define PLF_WRITE_FAILED      3 

/*! \brief Error during file close 

 */
#define PLF_FILE_CLOSE_ERROR      4 


/*! \name File Operations 
 
@{
*/ 


/*! \brief Max error message length  
*/
#define ERR_MSG_LENGTH 128

/*! \brief Structure used for passing parameters to functions.  
*/

struct file_io_struct 
{
   /*! \brief internal handle */
   void* h; 
   /*! \brief pointer to file path */
   char* path; 
   /*! \brief buffer of file content */
   char* buffer; 
   /*! \brief Error message if operation failed */
   char  err[ERR_MSG_LENGTH]; 
   /*! \brief flush buffer to media */
   int sync; 
   /*! \brief size of buffer */
   int size; 
   /*! \brief return code  */
   int rc; 
   /*! \brief progress callback  */
   void (*progress)(int);
   /*! \brief file permissions  */
   int mode;
   /*! \brief Allocate memory flag */
   int mem_flag;  /* set to 1 if we have allocated memory */ 
   /*! \brief 1 for read  , 2 for map  */
   int read_mode; 
};

/*! \brief Parameter IO type   
*/

typedef struct file_io_struct*  file_io;

/*! \brief Successful operation*/
#define PLF_FILE_OK 1 

/*! \brief file path is empty  */
#define PLF_FILE_BAD_PATH 3

/*! \brief Unable to get file attributes */
#define  PLF_FILE_ATTR_FAIL 4

/*! \brief Unable to open file */
#define  PLF_FILE_ERR_OPEN 5

/*! \brief Unable to allocate memory to store file content  */
#define  PLF_FILE_MEMORY_ALLOCATION_FAILED 6

/*! \brief Error occured while reading file */
#define  PLF_FILE_BAD_READ 7

/*! \brief Error when mapping file */
#define  PLF_FILE_ERR_MAP 8

/*! \brief Error while write buffer to file */
#define PLF_FILE_ERR_WRITE 9 

/*! \brief Attempt to calculate digest on wrong file type */
#define PLF_FILE_BAD_FILE_TYPE 10 

/*! \brief Not implemented  */
#define PLF_FILE_NOT_IMPLEMENTED 11 

/*! \name Read
  @{
*/
/*! \brief read file into buffer    
    \retval PLF_FILE_OK 
    \retval PLF_FILE_BAD_PATH 
    \retval PLF_FILE_ATTR_FAIL
    \retval PLF_FILE_ERR_OPEN 
    \retval PLF_FILE_BAD_READ
    \retval PLF_FILE_MEMORY_ALLOCATION_FAILED
    \param[in/out] parameter  structure     

                               
    This functions reads a file into a memory buffer 
    allocated by NMemAlloc (malloc). 

*/
int    plf_read_file(file_io in);

/*! \brief map file read only     
    \param[in/out] parameter  structure     
    \retval PLF_FILE_OK 
    \retval PLF_FILE_BAD_PATH 
    \retval PLF_FILE_ATTR_FAIL
    \retval PLF_FILE_ERR_OPEN 
    \retval PLF_FILE_ERR_MAP 

    This functions maps a file to a virtual address. 
    Use this for large files , but be aware that the buffer 
    share the same data as the actual file. 
    Useful for verification of signature etc. 
                                            
*/

int plf_map_file(file_io in);

/*! \brief write file from buffer    
    \param[in/out] parameter  structure     
    \retval PLF_FILE_OK 
    \retval PLF_FILE_BAD_PATH 
    \retval PLF_FILE_ERR_OPEN
    \retval PLF_FILE_ERR_WRITE


    This function writes buffer to a file. 
    It creates the file if not existent. 
                              
*/
int    plf_write_file(file_io in);


/*! \brief read attributes and calculate digest if path is regular file    
    \param[in/out] parameter  structure     
    \retval PLF_FILE_OK 
    \retval PLF_FILE_ATTR_FAIL
    \retval PLF_FILE_EMPTY_FILE
    \retval PLF_FILE_DIGEST_FAILED
    \retval PLF_FILE_BAD_FILE_TYPE
    \retval PLF_FILE_BAD_PATH


    This functions collects size, permissions and also calculate SHA digest if 
    the hash_flag is set to 1. 

*/
int  plf_attr_file(file_io in);

/*! \brief Copies a file.
    \param [in] source Source file.
    \param [in] target Target file.
    \retval PLF_OK 
    \retval PLF_NO_SOURCE 
    \retval PLF_NO_TARGET
    \retval PLF_WRITE_FAILED 
    \retval PLF_FILE_CLOSE_ERROR 

    This function works as the Posix cp command 
    cp <fromfile> <tofile>. 
 */
int plf_cp(const char *source, const char *target);

//@}


/*! \name Admin 
  @{
*/
/*! \brief allocate new file object     
      
    \return file handle 
    This function creates a new file handle. 
*/
file_io plf_new_file(void);

/*! \brief free object     
    \param[in/out] parameter  structure     

    This function deallocates all memory occupied by the 
    file handle.  
                  
*/
void plf_free_file(file_io in);

// @}

#ifdef __cplusplus
}
#endif


#endif

