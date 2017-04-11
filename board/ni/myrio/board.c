/*
 * Just to satisfy init routines..
 */

#include <common.h>
#include <nand.h>
#include <sdhci.h>
#include <asm/io.h>
#include <environment.h>
#include <i2c.h>
#include <miiphy.h>
#include <netdev.h>
#include <zynqpl.h>
#include <asm/arch/clk.h>
#include <jffs2/load_kernel.h>

#define BOOT_MODE_REG     (XPSS_SYS_CTRL_BASEADDR + 0x25C)
#define BOOT_MODES_MASK    0x0000000F
#define QSPI_MODE         (0x00000001)            /**< QSPI */
#define NOR_FLASH_MODE    (0x00000002)            /**< NOR  */
#define NAND_FLASH_MODE   (0x00000004)            /**< NAND */
#define SD_MODE           (0x00000005)            /**< Secure Digital card */
#define JTAG_MODE	  (0x00000000)		  /**< JTAG */

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FPGA
#if defined(CONFIG_ROBORIO)
xilinx_desc fpga = XILINX_XC7Z020_DESC(0);
#else
xilinx_desc fpga = XILINX_XC7Z010_DESC(0);
#endif
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
#ifndef CONFIG_MFG
	int serial_missing;
	int ethaddr_missing;
	/* These must be initialized to zero so that we default to this address
	 * as not missing. This allows us to selectively populate the address
	 * from a backup location for some targets (e.g. non-myRIO) and not for
	 * others (e.g. myRIO). */
	int usbgadgetethaddr_missing = 0;
	int eth1addr_missing = 0;
#endif

	/*
	 * Take usb phy out of reset
	 */
	tmp = 0x00;
	i2c_write(0x40, 0x03, 1, &tmp, 1);

#if defined(CONFIG_MFG)
	set_default_env("Default env required for manufacturing.\n");
#else
	serial_missing = getenv("serial#") == NULL;
	ethaddr_missing = getenv("ethaddr") == NULL;
#ifdef CONFIG_MYRIO
	/* Only set eth1addr_missing for myRIO targets, because all current
	 * non-myRIO targets don't have an eth1addr programmed. */
	eth1addr_missing = getenv("eth1addr") == NULL;
#else
	/* Only set usbgadgetethaddr_missing for non-myRIO targets, so that for
	 * myRIO targets we'll never be missing the address (by always using
	 * the initial value of 0). */
	usbgadgetethaddr_missing = getenv("usbgadgetethaddr") == NULL;
#endif
	if (serial_missing || ethaddr_missing || eth1addr_missing ||
	    usbgadgetethaddr_missing) {
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
		if (ethaddr_missing && !nand_read_status) {
			int offset = getenv_ulong("backupethaddroffset", 16,
                                CONFIG_BACKUP_ETHADDR_OFFSET);

			if (memcmp(&nand_buffer[offset],
			    "\xff\xff\xff\xff\xff\xff", 6)) {
				eth_setenv_enetaddr("ethaddr", &nand_buffer[offset]);
			}
		}
		if (eth1addr_missing && !nand_read_status) {
			int offset = getenv_ulong("backupeth1addroffset", 16,
                                CONFIG_BACKUP_ETH1ADDR_OFFSET);

			if (memcmp(&nand_buffer[offset],
			    "\xff\xff\xff\xff\xff\xff", 6)) {
				eth_setenv_enetaddr("eth1addr", &nand_buffer[offset]);
			}
		}
		if (usbgadgetethaddr_missing && !nand_read_status) {
			int offset = getenv_ulong(
				"backupusbgadgetethaddroffset", 16,
				CONFIG_BACKUP_USBGADGETETHADDR_OFFSET);

			if (memcmp(&nand_buffer[offset],
			    "\xff\xff\xff\xff\xff\xff", 6)) {
				eth_setenv_enetaddr("usbgadgetethaddr",
						    &nand_buffer[offset]);
			}
		}
		saveenv();
	}
#endif

	return 0;
}

#ifdef CONFIG_CMD_MMC
int board_mmc_init(bd_t *bd)
{
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 125000000, 0, SDHCI_QUIRK_NO_CD);
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

/*
 * OK, and resets too.
 */
void reset_misc(void)
{
	u8 tmp;

	/* Reset the target using the CPLD */
	tmp = 0x80;
	i2c_write(0x40, 0x02, 1, &tmp, 1);

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
