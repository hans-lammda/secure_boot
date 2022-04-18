#ifndef __PMILL_GETOPT_H__
#define __PMILL_GETOPT_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_generic/trunk/plf_generic/ngetopt.h $ $Revision: 8122 $"

/*
 Copyright Oniteo 2007 
*/  

#ifdef __cplusplus
extern "C" {
#endif

/*! \file ngetopt.h
    \brief Command line parsing.


    This suite of functions is used to parse the command line.

*/

/*! \name Options 
 
    @{

*/

/*! \brief Parses the command line options.
    \param[in] argc The number of argument.
    \param[in] argv The argv array of strings.
    \param[in] shortopts Pointer to valid short options.
    \param[in] longopts Pointer to valid options.   
    \param[in] longind Pointer to index of current option.
    \return the next option character.

    This function parses options from the commandline.
    It is a platform independent version of getopt, modeled upon the 
    getopt_long function.
    The values to supply are argc and argv from main() parameter list.

    If a valid option was found the function returns the option character.
    If all command-line options have been parsed, then the function returns -1.
    If next option character is not in optstring or there is a missing argument
    then '?' or ':' is returned.
 */
int NGetOpt (int argc, char ** argv,
             const char *shortopts,
             const struct option *longopts, int *longind);

/*! \brief Returns optarg.
 
    \return a null-terminated string, or NULL if no option argument exists.

     This function returns the current optarg value.
 */
char* NGetOptarg(void);

/*! \brief Returns optind. 
    \return index of the next argv entry to be processed.

    This function returns the index of the next argv entry to be processed. 
 */
int NGetOptind(void);

// @}

#ifdef __cplusplus
}
#endif

#endif
