/*! \file safestr.h
    \brief String Functions

   Platform independent safe string functions.

*/

#ifndef __PMILL_SAFESTR_H__
#define __PMILL_SAFESTR_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/safestr.h $ $Revision: 17763 $"

#include <stdarg.h>

/*! \brief OK */
#define SAFESTR_OK                            1
/*! \brief Invalid parameter. */
#define SAFESTR_ERROR_INVALID_PARAMETER       2
/*! \brief Insufficient buffer size. */
#define SAFESTR_ERROR_INSUFFICIENT_BUFFER     3 
/*! \brief Unknown error. */
#define SAFESTR_ERROR_UNKNOWN                99


#ifdef __cplusplus
extern "C" {
#endif

/*! \name Strings

    @{

*/

/*! \brief Safe string formatting.
    \param[out] target The formatted target buffer.
    \param[in] targetSize Allocated size of target buffer.
    \param[in] format A printf-style format string.
    \param[in] v An argument list passed as a va_list (instead of a variable number of args)
    \retval SAFESTR_OK Operation succeeded.
    \retval SAFESTR_ERROR_INVALID_PARAMETER The targetSize value was eiher too small or way too large.
    \retval SAFESTR_ERROR_INSUFFICIENT_BUFFER The copy operation failed due to insufficient buffer space. The destination buffer contains a truncated, null-terminated version of the intended result. In situations where truncation is acceptable, this may not necessarily be seen as a failure condition.
    \retval SAFESTR_ERROR_UNKNOWN No description.

    This function is a secure version of vsprintf.

    If the function returns SAFESTR_ERROR_INSUFFICIENT_BUFFER the formatting operation failed
    due to insufficient buffer space. The destination buffer contains a truncated,
    null-terminated version of the intended result. In situations where truncation is
    acceptable, this may not necessarily be seen as a failure condition.
 */
int NStrVSPrintf(char* target, int targetSize, const char* format, va_list v);


/*! \brief Safe string formatting.
    \param[out] target The formatted target buffer.
    \param[in] targetSize Allocated size of target buffer.
    \param[in] format A printf-style format string.
    \retval SAFESTR_OK Operation succeeded.
    \retval SAFESTR_ERROR_INVALID_PARAMETER The targetSize value was eiher too small or way too large.
    \retval SAFESTR_ERROR_INSUFFICIENT_BUFFER The copy operation failed due to insufficient buffer space. The destination buffer contains a truncated, null-terminated version of the intended result. In situations where truncation is acceptable, this may not necessarily be seen as a failure condition.
    \retval SAFESTR_ERROR_UNKNOWN No description.

    This function is a secure version of sprintf.

    If the function returns SAFESTR_ERROR_INSUFFICIENT_BUFFER the formatting operation failed
    due to insufficient buffer space. The destination buffer contains a truncated,
    null-terminated version of the intended result. In situations where truncation is
    acceptable, this may not necessarily be seen as a failure condition.
 */
int NStrSPrintf(char* target, int targetSize, const char* format, ...);


/*! \brief Safe copy from source to target.
    \param[out] target The target buffer.
    \param[in] targetSize Allocated size of target buffer.
    \param[in] source The source from which to copy data.
    \retval SAFESTR_OK Operation succeeded.
    \retval SAFESTR_ERROR_INVALID_PARAMETER The targetSize value was eiher too small or way too large.
    \retval SAFESTR_ERROR_INSUFFICIENT_BUFFER The copy operation failed due to insufficient buffer space. The destination buffer contains a truncated, null-terminated version of the intended result. In situations where truncation is acceptable, this may not necessarily be seen as a failure condition.
    \retval SAFESTR_ERROR_UNKNOWN No description.

    This function copies one string to another, with respect to the size of the destination string.

    If the function returns SAFESTR_ERROR_INSUFFICIENT_BUFFER the copy operation failed
    due to insufficient buffer space. The destination buffer contains a truncated,
    null-terminated version of the intended result. In situations where truncation is
    acceptable, this may not necessarily be seen as a failure condition.

 */
int NStrSCopy(char* target, const char* source, int targetSize);


/*  \brief Safe strncpy copy function.
    \param[out] target The target buffer.
    \param[in] targetSize Allocated size of target.
    \param[in] source The source from which to copy data.
    \param[in] sourceSize Number of bytes to copy.
    \retval SAFESTR_OK Operation succeeded.
    \retval SAFESTR_ERROR_INVALID_PARAMETER The targetSize value was eiher too small or way too large.
    \retval SAFESTR_ERROR_INSUFFICIENT_BUFFER The copy operation failed due to insufficient buffer space. The destination buffer contains a truncated, null-terminated version of the intended result. In situations where truncation is acceptable, this may not necessarily be seen as a failure condition.
    \retval SAFESTR_ERROR_UNKNOWN No description.

    This function copies one string to another, with respect to both strings sizes.

    If this function returns SAFESTR_ERROR_INSUFFICIENT_BUFFER the copy operation failed
    due to insufficient buffer space. The destination buffer contains a truncated,
    null-terminated version of the intended result. In situations where truncation is
    acceptable, this may not necessarily be seen as a failure condition.

int NStrSCopyN(char* target, int targetSize, const char* source, int sourceSize);
*/

/*! \brief Safe string concatenation.
    \param[out] target Pointer to target buffer.
    \param[in] source Pointer to source buffer.
    \param[in] targetSize Allocated size of target.
    \retval SAFESTR_OK Operation succeeded.
    \retval SAFESTR_ERROR_INVALID_PARAMETER The targetSize value was eiher too small or way too large.
    \retval SAFESTR_ERROR_INSUFFICIENT_BUFFER The copy operation failed due to insufficient buffer space. The destination buffer contains a truncated, null-terminated version of the intended result. In situations where truncation is acceptable, this may not necessarily be seen as a failure condition.
    \retval SAFESTR_ERROR_UNKNOWN No description.

    This function concatenates two strings.

    If this function returns SAFESTR_ERROR_INSUFFICIENT_BUFFER the concatenation operation
    failed due to insufficient buffer space. The destination buffer contains a truncated,
    null-terminated version of the intended result. In situations where truncation is acceptable,
    this may not necessarily be seen as a failure condition.
 */
int NStrSCat(char* target, const char* source, int targetSize);

// @}


#ifdef __cplusplus
}
#endif


#endif


