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
#include <asm/arch/clk.h>
#include <asm/arch/hardware.h>
#include <jffs2/load_kernel.h>

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

struct backup_var {
	char *variable;
	char *backupvar;
	int default_offset;
	int (*handler)(char *entry, u8 *buffer);
};

static int ethaddr_handler(char *var, u8 *buffer)
{
	eth_setenv_enetaddr(var, buffer);
	return 0;
}

static int serialno_handler(char *var, u8 *buffer)
{
	char string[18] = "";
	sprintf(string, "%08X", *(u32 *)buffer);
	setenv(var, string);
	return 0;
}

static int pstring_handler(char *var, u8 *buffer)
{
	struct pstring {
		u8 len;
		char string[0];
	} __packed *pstr;
	char *string;

	pstr = (struct pstring *)buffer;

	string = malloc(pstr->len + 1);
	if (NULL == string)
		return -ENOMEM;

	memcpy(string, pstr->string, pstr->len);
	string[pstr->len] = '\0';

	setenv(var, string);
	free(string);

	return 0;
}

struct backup_var backup_var_table[] = {
	{ "serial#", "backupserialoffset", CONFIG_BACKUP_SERIAL_OFFSET,
		serialno_handler },
	{ "ethaddr", "backupethaddroffset", CONFIG_BACKUP_ETHADDR_OFFSET,
		ethaddr_handler },
#if defined(CONFIG_MEM_512) || \
	defined(CONFIG_CRIO9068) || \
	(defined(CONFIG_SBRIO) && defined(CONFIG_RMC))
	{ "eth1addr", "backupeth1addroffset", CONFIG_BACKUP_ETH1ADDR_OFFSET,
		ethaddr_handler },
#endif
#if defined(CONFIG_GEN2) || defined(CONFIG_SBRIO) || defined(CONFIG_ELVISIII)
	{ "usbgadgetethaddr", "backupusbgadgetethaddroffset",
		CONFIG_BACKUP_USBGADGETETHADDR_OFFSET, ethaddr_handler },
#endif
#if defined(CONFIG_ELVISIII)
	{ "wirelessRegionFactory", NULL,
		CONFIG_BACKUP_WIRELESS_REGION_OFFSET, pstring_handler },
	{ "wireless_board_id", NULL,
		CONFIG_BACKUP_WIRELESS_BOARD_ID_OFFSET, pstring_handler },
#endif
};

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init (void)
{
	u8 tmp;
	int err;
	struct udevice *cpld;
	err = i2c_get_chip_for_busnum(0, CONFIG_I2C_CPLD_ADDR, 1, &cpld);
	if (err) {
		debug("%s: Cannot find CPLD\n", __func__);
		cpld = NULL;
	}

#if defined(CONFIG_MFG)
	char serial[11] = "";
#else
	unsigned env_missing = 0;
	int i;
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
#else
	/* Check if any variable is missing and restore */
	for (i = 0; i < ARRAY_SIZE(backup_var_table); i++) {
		if (getenv(backup_var_table[i].variable) == NULL)
			env_missing |= (1 << i);
	}

	if (env_missing) {
		u8 nand_buffer[nand_info[0]->writesize];
		int nand_read_status;
		size_t len = nand_info[0]->writesize;

		nand_read_status = nand_read(nand_info[0],
		    getenv_ulong("backuppage", 16, CONFIG_BACKUP_PAGE), &len,
		    nand_buffer);

		if (nand_read_status)
			goto var_restore_end;

		for (i = 0; i < ARRAY_SIZE(backup_var_table); i++) {
			struct backup_var *entry = &backup_var_table[i];
			unsigned long offset;

			if (!(env_missing & (1 << i)))
				continue;

			if (NULL == entry->backupvar)
				offset = entry->default_offset;
			else
				offset = getenv_ulong(entry->backupvar, 16,
						      entry->default_offset);

			entry->handler(entry->variable, &nand_buffer[offset]);
		}
		saveenv();
var_restore_end:
		;
	}
#endif

	return 0;
}
#endif /* !CONFIG_BOARD_LATE_INIT */

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

#ifdef CONFIG_CMD_NAND
static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
void board_nand_init(void)
{
	struct nand_chip *nand = &nand_chip[0];
	loff_t off;
	loff_t size;

	if (zynq_nand_init(nand, 0))
		debug("ZYNQ NAND init failed\n");

	/*
	 * Unlock the entire flash for manufacturing
	 */
#if defined(CONFIG_MFG)
	off = 0;
#else
	struct mtd_device *dev;
	u8 part_num;
	struct part_info *part;

	if (mtdparts_init() ||
		 find_dev_and_part("u-boot", &dev, &part_num, &part))
		off = CONFIG_MTD_UBOOT_OFFSET + CONFIG_BOARD_SIZE_LIMIT;
	else
		off = part->size + part->offset;
#endif

	/*
	 * This is crappy (using 0 as the device), but this is what the Zynq
	 * NAND driver does, so we do it too to make sure we get the same device
	 */
	size = nand_info[0]->size - off;
	nand_unlock(nand_info[0], off, size, 0);
}
#endif /* CONFIG_CMD_NAND */

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
