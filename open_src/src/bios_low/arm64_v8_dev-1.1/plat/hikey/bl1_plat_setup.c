
#include <arch_helpers.h>
#include <assert.h>
#include <bl_common.h>
#include <cci400.h>
#include <console.h>
#include <debug.h>
#include <errno.h>
#include <gpio.h>
#include <hi6220.h>
#include <hi6553.h>
#include <mmio.h>
#include <partitions.h>
#include <platform.h>
#include <platform_def.h>
#include <sp804_timer.h>
#include <string.h>
#include "../../bl1/bl1_private.h"
#include "hikey_def.h"
#include "hikey_private.h"


static void hi6220_pmussi_init(void);
static void hikey_gpio_init(void);
static void hikey_hi6553_init(void);
static int query_boot_mode(void);


void bl1_early_platform_setup(void)
{

	hi6220_timer_init();
	cci_init(CCI400_BASE,CCI400_SL_IFACE3_CLUSTER_IX,CCI400_SL_IFACE4_CLUSTER_IX);
	cci_enable_cluster_coherency(read_mpidr());

	hikey_gpio_init();
	hi6220_pmussi_init();
	hikey_hi6553_init();
	hi6220_pll_init();

	console_init(PL011_UART0_BASE, PL011_UART_CLK_IN_HZ, PL011_BAUDRATE); 
	console_init(PL011_UART2_BASE, PL011_UART_CLK_IN_HZ, PL011_BAUDRATE);
	console_init(PL011_UART3_BASE, PL011_UART_CLK_IN_HZ, PL011_BAUDRATE);

}


static int sd_card_detect(void)
{
	int ret;
	/* configure GPIO8 as nopull */
	mmio_write_32(0xf8001830, 0);
	gpio_direction_input(8);
	ret = gpio_get_value(8);
	if (!ret)
		return 1;
	return 0;
}

void hikey_sd_init(void)
{
	int ret;

	/* switch pinmux to SD */
	mmio_write_32(0xf701000c, 0);
	mmio_write_32(0xf7010010, 0);
	mmio_write_32(0xf7010014, 0);
	mmio_write_32(0xf7010018, 0);
	mmio_write_32(0xf701001c, 0);
	mmio_write_32(0xf7010020, 0);

	/* input, 16mA or 12mA */
	mmio_write_32(0xf701080c, 0x64);
	mmio_write_32(0xf7010810, 0x54);
	mmio_write_32(0xf7010814, 0x54);
	mmio_write_32(0xf7010818, 0x54);
	mmio_write_32(0xf701081c, 0x54);
	mmio_write_32(0xf7010820, 0x54);
	ret = sd_card_detect();
	// if (ret)
	// 	INFO("SD Card has been detected.\n");
}

void hikey_jumper_init(void)
{
	/* configure GPIO24 as nopull */
	mmio_write_32(0xf7010950, 0);
	/* configure GPIO24 as gpio */
	mmio_write_32(0xf7010140, 0);
	gpio_direction_input(24);
}

static inline char hex2str(unsigned int data)
{
	data &= 0xf;
	if ((data >= 0) && (data <= 9))
		return (char)(data + 0x30);
	return (char)(data - 10 + 0x41);
}

static uint64_t rand(unsigned int data)
{
	int64_t quotient, remainder, t;

	quotient = data / 127773;
	remainder = data % 127773;
	t = 16807 * remainder - 2836 * quotient;
	if (t <= 0)
		t += RANDOM_MAX;
	return (t % ((uint64_t)RANDOM_MAX + 1));
}

void generate_serialno(struct random_serial_num *random)
{
	unsigned int data, t;
	int i;

	data = mmio_read_32(AO_SC_SYSTEST_SLICER_CNT0);
	t = rand(data);
	random->data = ((uint64_t)t << 32) | data;
	for (i = 0; i < 8; i++) {
		random->serialno[i] = hex2str((t >> ((7 - i) << 2)) & 0xf);
	}
	for (i = 0; i < 8; i++) {
		random->serialno[i + 8] = hex2str((data >> ((7 - i) << 2)) & 0xf);
	}
	random->serialno[16] = '\0';
	random->magic = RANDOM_MAGIC;
}

static void hikey_verify_serialno(struct random_serial_num *random)
{
	char *serialno;

	serialno = load_serialno();
	if (serialno == NULL) {
		generate_serialno(random);
		flush_random_serialno((unsigned long)&random, sizeof(random));
	}
}


/* PMU SSI is the device that could map external PMU register to IO */
static void hi6220_pmussi_init(void)
{
	uint32_t data;

	/*
	 * After reset, PMUSSI stays in reset mode.
	 * Now make it out of reset.
	 */
	mmio_write_32(AO_SC_PERIPH_RSTDIS4,
		AO_SC_PERIPH_RSTDIS4_PRESET_PMUSSI_N);
	do {
		data = mmio_read_32(AO_SC_PERIPH_RSTSTAT4);
	} while (data & AO_SC_PERIPH_RSTDIS4_PRESET_PMUSSI_N);

	/* set PMU SSI clock latency for read operation */
	data = mmio_read_32(AO_SC_MCU_SUBSYS_CTRL3);
	data &= ~AO_SC_MCU_SUBSYS_CTRL3_RCLK_MASK;
	data |= AO_SC_MCU_SUBSYS_CTRL3_RCLK_3;
	mmio_write_32(AO_SC_MCU_SUBSYS_CTRL3, data);

	/* enable PMUSSI clock */
	data = AO_SC_PERIPH_CLKEN5_PCLK_PMUSSI_CCPU |
	       AO_SC_PERIPH_CLKEN5_PCLK_PMUSSI_MCU;
	mmio_write_32(AO_SC_PERIPH_CLKEN5, data);
	data = AO_SC_PERIPH_CLKEN4_PCLK_PMUSSI;
	mmio_write_32(AO_SC_PERIPH_CLKEN4, data);

	/* output high on gpio0 */
	gpio_direction_output(0);
	gpio_set_value(0, 1);
}

static void hikey_hi6553_init(void)
{
	int data;

	hi6553_write_8(PERI_EN_MARK, 0x1e);
	hi6553_write_8(NP_REG_ADJ1, 0);
	data = DISABLE6_XO_CLK_CONN | DISABLE6_XO_CLK_NFC |
		DISABLE6_XO_CLK_RF1 | DISABLE6_XO_CLK_RF2;
	hi6553_write_8(DISABLE6_XO_CLK, data);

	/* configure BUCK0 & BUCK1 */
	hi6553_write_8(BUCK01_CTRL2, 0x5e);
	hi6553_write_8(BUCK0_CTRL7, 0x10);
	hi6553_write_8(BUCK1_CTRL7, 0x10);
	hi6553_write_8(BUCK0_CTRL5, 0x1e);
	hi6553_write_8(BUCK1_CTRL5, 0x1e);
	hi6553_write_8(BUCK0_CTRL1, 0xfc);
	hi6553_write_8(BUCK1_CTRL1, 0xfc);

	/* configure BUCK2 */
	hi6553_write_8(BUCK2_REG1, 0x4f);
	hi6553_write_8(BUCK2_REG5, 0x99);
	hi6553_write_8(BUCK2_REG6, 0x45);
	mdelay(1);
	hi6553_write_8(VSET_BUCK2_ADJ, 0x22);
	mdelay(1);

	/* configure BUCK3 */
	hi6553_write_8(BUCK3_REG3, 0x02);
	hi6553_write_8(BUCK3_REG5, 0x99);
	hi6553_write_8(BUCK3_REG6, 0x41);
	hi6553_write_8(VSET_BUCK3_ADJ, 0x02);
	mdelay(1);

	/* configure BUCK4 */
	hi6553_write_8(BUCK4_REG2, 0x9a);
	hi6553_write_8(BUCK4_REG5, 0x99);
	hi6553_write_8(BUCK4_REG6, 0x45);

	/* configure LDO20 */
	hi6553_write_8(LDO20_REG_ADJ, 0x50);

	hi6553_write_8(NP_REG_CHG, 0x0f);
	hi6553_write_8(CLK_TOP0, 0x06);
	hi6553_write_8(CLK_TOP3, 0xc0);
	hi6553_write_8(CLK_TOP4, 0x00);

	/* configure LDO7 & LDO10 for SD slot */
	data = hi6553_read_8(LDO7_REG_ADJ);
	data = (data & 0xf8) | 0x2;
	hi6553_write_8(LDO7_REG_ADJ, data);
	mdelay(5);
	/* enable LDO7 */
	hi6553_write_8(ENABLE2_LDO1_8, 1 << 6);
	mdelay(5);
	data = hi6553_read_8(LDO10_REG_ADJ);
	data = (data & 0xf8) | 0x5;
	hi6553_write_8(LDO10_REG_ADJ, data);
	mdelay(5);
	/* enable LDO10 */
	hi6553_write_8(ENABLE3_LDO9_16, 1 << 1);
	mdelay(5);
	/* enable LDO15 */
	data = hi6553_read_8(LDO15_REG_ADJ);
	data = (data & 0xf8) | 0x4;
	hi6553_write_8(LDO15_REG_ADJ, data);
	hi6553_write_8(ENABLE3_LDO9_16, 1 << 6);
	mdelay(5);
	/* enable LDO22 */
	data = hi6553_read_8(LDO22_REG_ADJ);
	data = (data & 0xf8) | 0x7;
	hi6553_write_8(LDO22_REG_ADJ, data);
	hi6553_write_8(ENABLE4_LDO17_22, 1 << 5);
	mdelay(5);

	/* select 32.764KHz */
	hi6553_write_8(CLK19M2_600_586_EN, 0x01);
}

static void hikey_gpio_init(void)
{
	gpio_register_device(GPIO0_BASE);
	gpio_register_device(GPIO1_BASE);
	gpio_register_device(GPIO2_BASE);
	gpio_register_device(GPIO3_BASE);
	gpio_register_device(GPIO4_BASE);
	gpio_register_device(GPIO5_BASE);
	gpio_register_device(GPIO6_BASE);
	gpio_register_device(GPIO7_BASE);
	gpio_register_device(GPIO8_BASE);
	gpio_register_device(GPIO9_BASE);
	gpio_register_device(GPIO10_BASE);
	gpio_register_device(GPIO11_BASE);
	gpio_register_device(GPIO12_BASE);
	gpio_register_device(GPIO13_BASE);
	gpio_register_device(GPIO14_BASE);
	gpio_register_device(GPIO15_BASE);
	gpio_register_device(GPIO16_BASE);
	gpio_register_device(GPIO17_BASE);
	gpio_register_device(GPIO18_BASE);
	gpio_register_device(GPIO19_BASE);

	/* Power on indicator LED (User LED0). */
	gpio_direction_output(32);
	gpio_set_value(32, 1);
	gpio_direction_output(33);
	gpio_direction_output(34);
	gpio_direction_output(35);
}
