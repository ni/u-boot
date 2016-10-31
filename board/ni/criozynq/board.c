/*
 * Just to satisfy init routines..
 */

#include <dm.h>
#include <common.h>

#include <clk.h>
#include <nand.h>
#include <sdhci.h>
#include <asm/io.h>
#include <environment.h>
#include <i2c.h>
#include <miiphy.h>
#include <net.h>
#include <netdev.h>
#include <serial.h>
#include <zynqpl.h>
#include <usb/ulpi.h>
#include <asm/arch/hardware.h>

#define BOOT_MODE_REG     (XPSS_SYS_CTRL_BASEADDR + 0x25C)
#define BOOT_MODES_MASK    0x0000000F
#define QSPI_MODE         (0x00000001)            /**< QSPI */
#define NOR_FLASH_MODE    (0x00000002)            /**< NOR  */
#define NAND_FLASH_MODE   (0x00000004)            /**< NAND */
#define SD_MODE           (0x00000005)            /**< Secure Digital card */
#define JTAG_MODE	  (0x00000000)		  /**< JTAG */

/* Clock frequencies for different speeds */
#define MACB_ZYNQ_GEM_FREQ_10	2500000
#define MACB_ZYNQ_GEM_FREQ_100	25000000
#define MACB_ZYNQ_GEM_FREQ_1000	125000000

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FPGA
xilinx_desc fpga = XILINX_XC7Z020_DESC(0);
#endif

#ifdef CONFIG_MACB_ZYNQ
struct macb_zynq_spd_cfg {
	int			emio_spd_gpio_1000;
	int			emio_spd_gpio_100;
	struct clk		tx_clk;
};
static struct macb_zynq_spd_cfg gem_spd_cfg[2];
#endif

#ifndef CONFIG_DM_SERIAL
struct serial_device *default_serial_console(void)
{
	return &null_serial_device;
}
#endif

int board_init(void)
{
#ifdef CONFIG_FPGA
	fpga_init();
	fpga_add(fpga_xilinx, &fpga);
#endif

	return 0;
}

int board_late_init (void)
{
	u8 tmp;
	int err;
	struct udevice *cpld;
	err = i2c_get_chip_for_busnum(0, 0x40, 1, &cpld);
	if (err) {
		debug("%s: Cannot find CPLD\n", __func__);
		cpld = NULL;
	}

#if defined(CONFIG_MFG)
	char serial[11] = "";
#endif
#if !defined(CONFIG_MFG)
	int serial_missing = 0;
	int ethaddr_missing = 0;
	int eth1addr_missing = 0;
	int usbgadgetethaddr_missing = 0;
#endif

	/*
	 * Take eth0 phy, eth1 phy, usb phy, usb hub, and external UART
	 * out of reset
	 */
	if (NULL != cpld) {
		tmp = 0x00;
		dm_i2c_write(cpld, 0x03, &tmp, 1);
	}

#if defined(CONFIG_MFG)
	if (getenv("serial#") != NULL)
		strcpy(serial, getenv("serial#"));
	set_default_env("Default env required for auto-bringup.\n");
	if (strlen(serial) != 0)
		setenv("serial#", serial);
#endif

#if !defined(CONFIG_MFG)
	serial_missing = getenv("serial#") == NULL;
	ethaddr_missing = getenv("ethaddr") == NULL;
#if defined(CONFIG_MEM_512) || \
	defined(CONFIG_CRIO9068) || \
	(defined(CONFIG_SBRIO) && defined(CONFIG_RMC))
	eth1addr_missing = getenv("eth1addr") == NULL;
#endif
#if defined(CONFIG_GEN2) || defined(CONFIG_SBRIO)
	usbgadgetethaddr_missing = getenv("usbgadgetethaddr") == NULL;
#endif
	if (serial_missing || ethaddr_missing || eth1addr_missing || usbgadgetethaddr_missing) {
		u8 nand_buffer[nand_info[0]->writesize];
		int nand_read_status;
		char string[18];
		size_t len = nand_info[0]->writesize;

		nand_read_status = nand_read(nand_info[0],
		    getenv_ulong("backuppage", 16, CONFIG_BACKUP_PAGE), &len,
		    nand_buffer);
		if (serial_missing && !nand_read_status) {
			sprintf(string, "%08x", *(u32 *)&nand_buffer[
			    getenv_ulong("backupserialoffset", 16,
				CONFIG_BACKUP_SERIAL_OFFSET)]);
			setenv("serial#", string);
		}
		if (ethaddr_missing && !nand_read_status)
			eth_setenv_enetaddr("ethaddr", &nand_buffer[
                            getenv_ulong("backupethaddroffset", 16,
				CONFIG_BACKUP_ETHADDR_OFFSET)]);
#if defined(CONFIG_MEM_512) || \
	defined(CONFIG_CRIO9068) || \
	(defined(CONFIG_SBRIO) && defined(CONFIG_RMC))
		if (eth1addr_missing && !nand_read_status)
			eth_setenv_enetaddr("eth1addr", &nand_buffer[
                            getenv_ulong("backupeth1addroffset", 16,
				CONFIG_BACKUP_ETH1ADDR_OFFSET)]);
#endif
#if defined(CONFIG_GEN2) || defined(CONFIG_SBRIO)
		if (usbgadgetethaddr_missing && !nand_read_status)
			eth_setenv_enetaddr("usbgadgetethaddr", &nand_buffer[
                            getenv_ulong("backupusbgadgetethaddroffset", 16,
				CONFIG_BACKUP_USBGADGETETHADDR_OFFSET)]);
#endif
		saveenv();
	}
#endif

	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	/* Need to figure out where to put this code if DM is turned on */
#if 0
	int retval;

	retval = zynq_gem_initialize(bis);

	/* Override the phys' drive strength */
	miiphy_write("zynq_gem", 0, 22, 2);
	miiphy_write("zynq_gem", 0, 24, 0xB949);
	miiphy_write("zynq_gem", 0, 22, 0);

	return retval;
#else
	return 0;
#endif
}
#endif

#ifdef CONFIG_CMD_MMC
int board_mmc_init(bd_t *bd)
{
	/* Should move this to device tree */
#if 0
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 125000000, 0, SDHCI_QUIRK_NO_CD);
#endif
}
#endif

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1_BASE;
	gd->bd->bi_dram[0].size = gd->ram_size;
}

/*
 * OK, and resets too.
 */
void reset_misc(void)
{
	u8 tmp;
	int err;
	struct udevice *dev;

	err = i2c_get_chip_for_busnum(0, 0x40, 1, &dev);
	if (err) {
		debug("%s: Cannot find CPLD\n", __func__);
		return;
	}
	/* Reset the target using the CPLD */
	tmp = 0x80;
	dm_i2c_write(dev, 0x02, &tmp, 1);

	while(1) {;}
}

#if defined(CONFIG_OF_BOARD_SETUP)
int ft_board_setup(void *blob, bd_t *bd)
{
	const char *product_id_str;
	u16 product_id_val;

	/* Add the product ID to the device tree */
	product_id_str = getenv("DeviceCode");
	if (product_id_str != NULL) {
		product_id_val = simple_strtoul(product_id_str, NULL, 0);
		fdt_find_and_setprop(blob, "/", "DeviceCode",
			&product_id_val, sizeof(product_id_val), 1);
	}
	return 0;
}
#endif

#ifdef CONFIG_MACB_ZYNQ
int macb_linkspd_cb(void *regs, unsigned int speed)
{
	struct macb_zynq_spd_cfg *macb;
	unsigned long clk_spd = 0;
	unsigned short emio_spd_1000;
	unsigned short emio_spd_100;
	int ret;

	if (regs == ZYNQ_GEM_BASEADDR0)
		macb = &gem_spd_cfg[0];
	else
		macb = &gem_spd_cfg[1];

	if (speed == _1000BASET) {
		clk_spd = MACB_ZYNQ_GEM_FREQ_1000;
		emio_spd_1000 = MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_ON;
		emio_spd_100 = MACB_ZYNQ_GEM_LINKSPD_100_GPIO_OFF;
	} else if (speed == _100BASET) {
		clk_spd = MACB_ZYNQ_GEM_FREQ_100;
		emio_spd_1000 = MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_OFF;
		emio_spd_100 = MACB_ZYNQ_GEM_LINKSPD_100_GPIO_ON;
	} else {
		clk_spd = MACB_ZYNQ_GEM_FREQ_10;
		emio_spd_1000 = MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_OFF;
		emio_spd_100 = MACB_ZYNQ_GEM_LINKSPD_100_GPIO_OFF;
	}

	if (macb->emio_spd_gpio_1000 >= 0)
		gpio_set_value(macb->emio_spd_gpio_1000, emio_spd_1000);
	if (macb->emio_spd_gpio_100 >= 0)
		gpio_set_value(macb->emio_spd_gpio_100, emio_spd_100);

	if (macb->tx_clk.dev) {
		zynq_slcr_unlock();
		ret = clk_set_rate(&macb->tx_clk, clk_spd);
		zynq_slcr_lock();
	}

	return ret;
}

int macb_late_eth_ofdata_to_platdata(struct udevice *dev)
{
	struct macb_zynq_spd_cfg *macb;

	if (dev_get_addr(dev) == ZYNQ_GEM_BASEADDR0)
		macb = &gem_spd_cfg[0];
	else
		macb = &gem_spd_cfg[1];

	macb->emio_spd_gpio_1000 = fdtdec_get_int(
			gd->fdt_blob, dev->of_offset,
			"cdns,emio-gpio-speed-1000", -1);
	macb->emio_spd_gpio_100 = fdtdec_get_int(
			gd->fdt_blob, dev->of_offset,
			"cdns,emio-gpio-speed-100", -1);

	if (macb->emio_spd_gpio_1000 >= 0) {
		if (gpio_requestf(macb->emio_spd_gpio_1000,
				  "gpio%d", macb->emio_spd_gpio_1000)) {
			printf("ERROR: Failed to reserved GPIO%d\n",
			       macb->emio_spd_gpio_1000);
			return -EBUSY;
		}
	}
	if (macb->emio_spd_gpio_100 >= 0) {
		if (gpio_requestf(macb->emio_spd_gpio_100,
				  "gpio%d", macb->emio_spd_gpio_100)) {
			printf("ERROR: Failed to reserved GPIO%d\n",
			       macb->emio_spd_gpio_100);
			return -EBUSY;
		}
	}

	if (clk_get_by_name(dev, "tx_clk", &macb->tx_clk))
		macb->tx_clk.dev = 0;

	return 0;
}
#endif

/*
 * Configure the RTC to not waste battery by disabling the BB32KHZ bit.
 */
#if defined(CONFIG_DM_RTC)
#define RTC_STAT_REG_ADDR       0x0f
#define RTC_STAT_BIT_BB32KHZ    0x40    /* Battery backed 32KHz Output  */
void rtc_ds3232_disable_bb32khz (struct udevice *dev)
{
	int err;
        uchar status;
	struct udevice *rtc;

	err = i2c_get_chip_for_busnum(0, CONFIG_I2C_RTC_ADDR, 1, &rtc);
	if (err) {
		debug("%s: Cannot find DS3232\n", __func__);
		return;
	}
	dm_i2c_read(rtc, RTC_STAT_REG_ADDR, &status, 1);
        status &= ~RTC_STAT_BIT_BB32KHZ;
	dm_i2c_write(rtc, RTC_STAT_REG_ADDR, &status, 1);
}
#endif
