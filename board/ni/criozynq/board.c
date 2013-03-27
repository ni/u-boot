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
#include <serial.h>
#include <zynqpl.h>
#include <usb/ulpi.h>

#define BOOT_MODE_REG     (XPSS_SYS_CTRL_BASEADDR + 0x25C)
#define BOOT_MODES_MASK    0x0000000F
#define QSPI_MODE         (0x00000001)            /**< QSPI */
#define NOR_FLASH_MODE    (0x00000002)            /**< NOR  */
#define NAND_FLASH_MODE   (0x00000004)            /**< NAND */
#define SD_MODE           (0x00000005)            /**< Secure Digital card */
#define JTAG_MODE	  (0x00000000)		  /**< JTAG */

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FPGA
xilinx_desc fpga = XILINX_XC7Z020_DESC(0);
#endif

#ifndef CONFIG_DM_SERIAL
struct serial_device *default_serial_console(void)
{
	return &null_serial_device;
}
#endif

int board_init(void)
{
	icache_enable();

#ifdef CONFIG_FPGA
	fpga_init();
	fpga_add(fpga_xilinx, &fpga);
#endif

	return 0;
}

int board_late_init (void)
{
	u8 tmp;
#if defined(CONFIG_MFG)
	char serial[11] = "";
#endif
#if !defined(CONFIG_MFG)
	int serial_missing;
	int ethaddr_missing;
	int eth1addr_missing;
#endif

	/*
	 * Take eth0 phy, eth1 phy, usb phy, usb hub, and external UART
	 * out of reset
	 */
	tmp = 0x00;
	i2c_write(0x40, 0x03, 1, &tmp, 1);

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
	eth1addr_missing = getenv("eth1addr") == NULL;
	if (serial_missing || ethaddr_missing || eth1addr_missing) {
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
		if (eth1addr_missing && !nand_read_status)
			eth_setenv_enetaddr("eth1addr", &nand_buffer[
                            getenv_ulong("backupeth1addroffset", 16,
				CONFIG_BACKUP_ETH1ADDR_OFFSET)]);
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
