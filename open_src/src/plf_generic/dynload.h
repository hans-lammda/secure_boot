/*! \file dynload.h
    \brief Dynamic Loader


    Header file for a dynamic loading API that provides platform-independent
    functions for the dynamic loading of libraries (shared objects).
    
 */
#ifndef __PMILL_DYNLOAD_H__
#define __PMILL_DYNLOAD_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/dynload.h $ $Revision: 8122 $"


#ifdef __cplusplus
extern "C" {
#endif


/*! \name Dynamic Loader 
 
    @{

*/

/*! \brief Opens and loads a dynamic library.

    \param[in] pszLibraryPath Pathname to the library to load.
    \return handle to the loaded library, or NULL on failure.

    Loads a dynamic library (shared object) into the current process.
    The pathname can either be a fully qualified path or a relative
    path to the library to load.
 */
HMODULE NDldLoadLibrary(const char* pszLibraryPath);

/*! \brief Unloads a dynamic library.

    \param[in] hLib Handle to previously loaded library.
    \return 0 on success, and non-zero on error.

    Unloads a library (shared object) loaded by NDldLoadLibrary().

    The type HMODULE is described in local.h.
 */
int NDldUnloadLibrary(HMODULE hLib);

/*! \brief Loads a symbol from a dynamic library.

    \param[in] hLib Handle to dynamic library.
    \param[in] pszName Name of symbol to load.
    \return symbol pointer on success, or NULL on failure.

     Loads a symbol from a dynamically loaded library.

     The types HMODULE and FARPROC are described in local.h.
 */
FARPROC NDldLoadSymbol(HMODULE hLib, const char* pszName);

/*! \brief Loads a function pointer. 

    \param[in] hLib Handle to dynamic library.
    \param[in] pszName Name of function to load.
    \return pointer to function on success, or NULL on failure.

    Loads a  function from a dynamically loaded library.

    The types HMODULE and FARPROC are described in local.h.
 */
FARPROC NDldLoadFunc(HMODULE hLib, const char* pszName);

/*! \brief Retreives last error from loader. 

    \param[in] pszDLError Pointer to the receiving error buffer.
    \param[in] nMax Maximum size of the error buffer. 
    \return pointer to the supplied error buffer.

    Returnes the last error message from the dynamic loader.
 */    
char* NDldGetError(char* pszDLError, int nMax);

/*! \brief Returns dynamic library extension. 
    \return pointer to the extension suffix. 

    Returns file extension for shared libraries on this platform.
 */    
char* NDldGetExtension(void);

// @}

#ifdef __cplusplus
}
#endif

#endif
