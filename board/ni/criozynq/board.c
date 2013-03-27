/*
 * Just to satisfy init routines..
 */

#include <common.h>
#include <asm/arch/nand.h>
#include <asm/arch/sdhci.h>
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

/* Common IO for xgmac and xnand */
/* Data Memory Barrier */
#define SYNCHRONIZE_IO dmb()

void XIo_Out32(u32 OutAddress, u32 Value)
{
	*(volatile u32 *) OutAddress = Value;
	SYNCHRONIZE_IO;
}

u32 XIo_In32(u32 InAddress)
{
	volatile u32 temp = *(volatile u32 *)InAddress;
	SYNCHRONIZE_IO;
	return temp;
}

#ifdef CONFIG_FPGA
Xilinx_desc fpga = XILINX_XC7Z020_DESC(0);
#endif

struct serial_device *default_serial_console(void)
{
	return &null_serial_device;
}

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
		u8 nand_buffer[nand_info[0].writesize];
		int nand_read_status;
		char string[18];
		size_t len = nand_info[0].writesize;

		nand_read_status = nand_read(nand_info,
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
	int retval;
	int phy_addr;
	const char *miiname;
	char gemname[10];
	int i;
	u16 regval;

	retval = zynq_gem_initialize(bis);

	/* The PHYs for zynq_gem0 and zynq_gem1 are both connected to the
	   MDIO interface of zynq_gem0. */
	miiname = "zynq_gem0";

	/* cRIO-9068 has a Marvell Gigabit PHY on gem0 and gem1 */
	for (i = 0; i < CONFIG_ZYNQ_GEM_COUNT; i++) {
		sprintf(gemname, "zynq_gem%d", i);

		phy_addr = zynq_gem_get_phyaddr(gemname);

		/* Page 2 */
		miiphy_write(miiname, phy_addr, 22, 2);

		/* Disable the unused 125 MHz clock from the phy */
		miiphy_write(miiname, phy_addr, 16, 0x444E);

		/* Remove the delay on the RXClk for the secondary PHY */
		if (i == 1)
			miiphy_write(miiname, phy_addr, 21, 0x1056);

		/* Override the phys' drive strength */
		miiphy_write(miiname, phy_addr, 24, 0xB949);

		/* Page 0 */
		miiphy_write(miiname, phy_addr, 22, 0);

		/* Copper specific control register 1 */
		miiphy_read(miiname, phy_addr, 16, &regval);
		regval |= (7 << 12); /* max number of gigabit attempts */
		regval |= (1 << 11); /* enable downshift */
		miiphy_write(miiname, phy_addr, 16, regval);

		/* Soft-reset the secondary phy so RXClk change takes effect */
		if (i == 1)
			miiphy_write(miiname, phy_addr, 0, 0x9140);

		/* Page 0xFF */
		miiphy_write(miiname, phy_addr, 22, 0xFF);

		/* Errata settings */
		miiphy_write(miiname, phy_addr, 17, 0x214b);
		miiphy_write(miiname, phy_addr, 16, 0x2144);
		miiphy_write(miiname, phy_addr, 17, 0x0c28);
		miiphy_write(miiname, phy_addr, 16, 0x2146);
		miiphy_write(miiname, phy_addr, 17, 0xb233);
		miiphy_write(miiname, phy_addr, 16, 0x214d);
		miiphy_write(miiname, phy_addr, 17, 0xcc0c);
		miiphy_write(miiname, phy_addr, 16, 0x2159);

		/* Page 0xFB */
		miiphy_write(miiname, phy_addr, 22, 0xFB);

		miiphy_write(miiname, phy_addr, 7, 0xc00d);

		/* Page 0 */
		miiphy_write(miiname, phy_addr, 22, 0);
	}

	return retval;
}
#endif

#ifdef CONFIG_CMD_MMC
int board_mmc_init(bd_t *bd)
{
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 100000000, 0, SDHCI_QUIRK_NO_CD);
}
#endif

#ifdef CONFIG_CMD_NAND
int board_nand_init(struct nand_chip *nand_chip)
{
	loff_t off;
	loff_t size;
	int ret;

	ret = zynq_nand_init(nand_chip);
	if (ret)
		return ret;

	/*
	 * Unlock the entire flash for SD bringup and manufacturing
	 */
#if defined(CONFIG_MFG)
	off = 0;
#else
	off = CONFIG_MTD_UBOOT_OFFSET + CONFIG_BOARD_SIZE_LIMIT;
#endif
	/*
	 * This is crappy (using 0 as the device), but this is what the Zynq
	 * NAND driver does, so we do it too to make sure we get the same device
	 */
	size = nand_info[0].size - off;

	nand_unlock(&nand_info[0], off, size, 0);

	return ret;
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
void reset_cpu(ulong addr)
{
	u8 tmp;

	/* Reset the target using the CPLD */
	tmp = 0x80;
	i2c_write(0x40, 0x02, 1, &tmp, 1);

	while(1) {;}
}

#if defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup(void *blob, bd_t *bd)
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
}
#endif
