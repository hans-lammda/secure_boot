/*! \file memory.h
    \brief Memory management

    Platform independent memory functions.  These functions should be
    used in combination with dynamic loaders.

*/

#ifndef __PMILL_MEMORY_H__
#define __PMILL_MEMORY_H__ "$HeadURL: http://cm.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/memory.h $ $Revision: 18163 $"

/*
 * #define MEM_DBG
 */

#ifndef MEM_DBG
  #define MALLOC(ptr,size) ptr = NMemAlloc(size);
  #define FREE(ptr) NMemFree(ptr);
#else

  #define MALLOC(ptr,size) \
          ptr = NMemAlloc(size); \
          trace_dl(1,"Allocated %d bytes @ %x \n",size,ptr);

  #define FREE(ptr) \
          NMemFree(ptr); \
          trace_dl(1,"Freed address %x\n",ptr);
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \name Memory

    @{

*/

/*! \brief Allocates memory pool
    \param[in] start First address of memory region
    \param[in] size in bytes of memory regions

    When memory management is used in realtime applications
    malloc and free often operates on a predefined memory area
    that is managed very efficient and fast.

 */
void NMemPool(char *start,size_t size);

/*! \brief Allocates a memory area.
    \param[in] bytes The number of bytes to allocate.
    \return pointer to allocated buffer.

    This function allocates a new memory buffer, where all bytes are initialized to 0.

 */
void* NMemAlloc(size_t bytes);
/*! \brief Allocates aligned  memory area.
    \param[in] bytes The number of bytes to allocate.
    \param[in] alignment to place the memory  
    \return pointer to allocated buffer.

    This function allocates a new memory buffer, where all bytes are initialized to 0.
    The align argument must be a multiple of 2 

 */
void* NMemAlloc_align(size_t bytes, size_t align);

/*! \brief Reallocates a memory area.
    \sa NMemAlloc
    \param[in] pMemBlk Pointer to previously allocated buffer.
    \param[in] bytes The new size of the buffer.
    \return pointer to reallocated buffer.

    This function reallocates a memory buffer, previously allocated by NMemAlloc().
 */
void* NMemRealloc(void* pMemBlk, size_t bytes);

/*! \brief Deallocates a memory area.
    \param[in] pMemBlk Buffer to deallocate.

    This function deallocates a buffer allocated by one of the functions
 */
void NMemFree(void* pMemBlk);

/*! \brief Deallocates aligned memory area.
    \param[in] pMemBlk Buffer to deallocate.

    This function deallocates a buffer allocated by one of the functions
 */
void NMemFree_align(void* pMemBlk);


/*! \brief Copies a memory buffer.
    \param[out] dest Pointer to destination buffer.
    \param[in] src Pointer to source buffer.
    \param[in] count Number of bytes to copy.
    \return pointer to destination buffer.

    This functon creates a copy of a memory buffer.
 */
void* NMemCopy(void* dest, void* src, size_t count);


/*! \brief Moves a memory region.
    \param[out] dest Pointer to destination buffer.
    \param[in] src Pointer to source buffer.
    \param[in] count Number of bytes to copy.
    \return pointer to destination buffer.

    Copies the values of num bytes from source to destination.
    Copying takes place as if an intermediate buffer was used,
    allowing the destination and source to overlap.
    The result is a binary copy of the data.
    The function does not check for any terminating null character in source.
    It always copies exactly num bytes.
    To avoid overflows, the size of the arrays pointed by both the destination and source parameters, shall be at least num bytes.
 */
void* NMemMove(void* dest, void* src, size_t count);

/*! \brief Fills a region in memory with a specified value.
    \param[in] ptr Pointer to the memory to fill.
    \param[in] value Value to be set.
    \param[in] count Number of bytes to set.
    \return pointer to memory region (same as ptr).

    Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char). bytes.
 */
void* NMemSet(void *ptr, int value, size_t count);

/*! \brief Compare two blocks of memory.
    \param[in] ptr1 Pointer to first memory block.
    \param[in] ptr2 Pointer to second memory block.
    \param[in] count Number of bytes to compare.
    \return Returns an integral value indicating the relationship between the content of the memory blocks:
            A zero value indicates that the contents of both memory blocks are equal.
            A value greater than zero indicates that the first byte that does not match in both memory blocks has a greater value in ptr1 than in ptr2 as if evaluated as unsigned char values;
            A value less than zero indicates the opposite.

    Compares the first num bytes of the block of memory pointed by ptr1 to the first num bytes pointed by ptr2.
    Zero is returned if they all match, if not then the return value represents which of the two memory blocks is greater.
*/
int NMemCmp(const void* ptr1, const void* ptr2, size_t count);

/*! \brief Checks if memory is readable.
    \param[in] pMemBlk Buffer to check for read access.
    \param[in] blkSize The size of the buffer.
    \return TRUE if the buffer is readable, and FALSE otherwise.

    This function checks if the specified memory is readable.
    This function works best on Windows. On other platforms, it just checks if the buffer is NULL or not.


 */
int NMemReadable(void* pMemBlk,size_t blkSize);

/*! \brief Checks if memory is writeable.
    \param[in] pMemBlk Buffer to check for write access.
    \param[in] blkSize The size of the buffer.
    \return TRUE if the buffer is writeable, and FALSE otherwise.

    This functions checks if the specified memory is writeable.
    This function works best on Windows. On other platforms, it just checks if the buffer is NULL or not.
 */
int NMemWriteable(void* pMemBlk,size_t blkSize);


/*! \brief Duplicates a string.
    \param[in] str String to duplicate.
    \return copy of the string in a memory buffer, or NULL on failure.

    This function duplicates a NULL-terminated string allocated by NMemAlloc() .
    The buffer returned must be deallocated by a call to NMemFree().
 */
char* NStrDup(const char* str);

/*! \brief Duplicates a memory area.
    \param[in] source Pointer to memory area to clone.
    \param[in] size Size of the memory area.
    \return pointer to duplicated memory area, or NULL on failure.

    This function duplicates a memory area.
 */
void* NMemDup(void* source,size_t size);

/*! \brief Updates NMemMalloc statistics
    
    If MEM_DBG is defined this function will save the address to the 
    allocated memory to the statistic structure and flag it as 
    allocated.

    \param[in] ptr pointer to memory that just have been allocated
*/
void NMemAddMallocStat(void *ptr);

/*! \brief Updates NMemFree statistics

     If MEM_DBG is defined this function will search the memory statistic 
     structure for a matching malloc done earlier. If found it will flag 
     the address as unallocated, otherwise it will return an error message.

     \param[in] ptr pointer to memory that just had been freed
*/
void NMemAddFreeStat(void *ptr);

/*! \brief Prints NMem statistics.

   Prints number of NMemAlloc,NMemFree done and memory used using trace_d if MEM_DBG 
   is defined. It also lists the start addresses to currently allocated memory blocks.
*/
void NMemPrintStats(void);

// @}

#ifdef __cplusplus
}
#endif

#endif

