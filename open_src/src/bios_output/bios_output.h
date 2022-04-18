/*! \file bios_output.h
    \brief Output functions

    @\n 
   
    This module provides print and timing functions 



*/

#ifndef BIOS_OUTPUT_H
#define BIOS_OUTPUT_H "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/bios_output/branches/haspoc/arm64_v8/bios_output/bios_output.h $ $Revision: 27094 $"





/*! \name Output
@{
*/

/*! \brief print hex 

    Early print used in assembler code 

    \param[in] n hex digit
*/
void asm_print_hex(n); 

/*! \brief print string 

    Early print used in assembler code 

    \param[in] s string 
*/
void asm_print_str(s); 

/*! \brief port init


    Setup serial port     
*/
void console_init(void); 

/*! \brief putchar


    Write one character on serial port     
*/
void console_putc(void); 

/*! \brief printf 

     Write the output under the control of a format

     string that specifies how subsequent arguments (or  arguments  accessed

     via the variable-length argument 

     \param[in] format string specifying output
     \return Number of characters printed upon success

    Write one character on serial port     
*/
int printf(const char *format, ...);

/*!
@}
*/

/*! \name Timing
@{
*/



/*! \brief init timers 

   This function reset the performance counters 

   needed to timestamp boot performance 

*/
void bios_output_setup_perf_timers(void); 

/*!
@}
*/


#endif /* BIOS_OUTPUT_H */


