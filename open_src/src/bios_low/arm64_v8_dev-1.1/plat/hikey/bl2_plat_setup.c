/*
 * Copyright (c) 2014-2015, Linaro Ltd and Contributors. All rights reserved.
 * Copyright (c) 2014-2015, Hisilicon Ltd and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch_helpers.h>
#include <assert.h>
#include <bl_common.h>
#include <console.h>
#include <debug.h>
#include <partitions.h>
#include <platform.h>
#include <platform_def.h>
#include <string.h>
#include <mmio.h>
#include <hi6220.h>
#include "hikey_def.h"
#include "hikey_private.h"

void init_boardid(void)
{
	unsigned int reg;

	/* Set chip id to sram */
	reg = read_midr_el1();
	mmio_write_32(MEMORY_AXI_CHIP_ADDR, reg);

	/* Set board type to sram */
	mmio_write_32(MEMORY_AXI_BOARD_TYPE_ADDR, 0x0);

	/* Set board id to sram */
	mmio_write_32(MEMORY_AXI_BOARD_ID_ADDR, 0x2b);

	mmio_write_32(ACPU_ARM64_FLAGA, 0x1234);
	mmio_write_32(ACPU_ARM64_FLAGB, 0x5678);
	return;
}

void bl2_early_platform_setup(void)

{

 /* From bl1_platform_setup */ 
 init_boardid();

 /* From bl2_early_platform_setup  */ 
 init_acpu_dvfs();

}
