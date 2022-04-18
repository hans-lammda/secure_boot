/*! \file optimized.h
    \brief Assembler version of functions

 */

/*
  Copyright (C) 2016 Oniteo
 */

#ifndef __ONITEO_BIOS_PLF_OPT_H__
#define __ONITEO_BIOS_PLF_OPT_H__ "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/plf/optimized.h $ $Revision: 26497 $"



void plf_memcpy16( void*, void* , int);

uint64_t read_clk();
uint64_t read_freq();
uint32_t read_ms();


#endif

