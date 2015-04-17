/*
 * Just to satisfy init routines..
 */

#include <common.h>
#include <asm/arch/nand.h>
#include <asm/arch/sdhci.h>
#include <asm/io.h>
#include <environment.h>
#include <i2c.h>
#include <jffs2/load_kernel.h>
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
	int serial_missing = 0;
	int ethaddr_missing = 0;
	int eth1addr_missing = 0;
	int usbgadgetethaddr_missing = 0;
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
#if defined(CONFIG_MEM_512) || \
	defined(CONFIG_CRIO9068) || \
	(defined(CONFIG_SBRIO) && defined(CONFIG_RMC))
	eth1addr_missing = getenv("eth1addr") == NULL;
#endif
#if defined(CONFIG_GEN2) || defined(CONFIG_SBRIO)
	usbgadgetethaddr_missing = getenv("usbgadgetethaddr") == NULL;
#endif
	if (serial_missing || ethaddr_missing || eth1addr_missing || usbgadgetethaddr_missing) {
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

/*
 * Configure Micrel Ethernet PHY registers
 */
static void config_micrel_phy()
{
	int phy_addr;
	const char *miiname;
	char gemname[10];
	int i;
	u16 regval;

	/* The PHYs for zynq_gem0 and zynq_gem1 are both connected to the
	   MDIO interface of zynq_gem0. */
	miiname = "zynq_gem0";

	for (i = 0; i < CONFIG_ZYNQ_GEM_COUNT; i++) {
		sprintf(gemname, "zynq_gem%d", i);

		phy_addr = zynq_gem_get_phyaddr(gemname);

		/* Write value to MMD Address 2h, Register 4h */
		miiphy_write(miiname, phy_addr, 0xD, 0x0002);
		miiphy_write(miiname, phy_addr, 0xE, 0x0004);
		miiphy_write(miiname, phy_addr, 0xD, 0x4002);
		miiphy_read(miiname, phy_addr, 0xE, &regval);
		/* Set RX_DV Pad Skew [7:4] to +0.30ns
			TX_EN Pad Skew [3:0] to -0.30ns */
		regval &= 0xFF00;
		regval |= 0xC2;
		miiphy_write(miiname, phy_addr, 0xE, regval);

		/* Write value to MMD Address 2h, Register 5h */
		miiphy_write(miiname, phy_addr, 0xD, 0x0002);
		miiphy_write(miiname, phy_addr, 0xE, 0x0005);
		miiphy_write(miiname, phy_addr, 0xD, 0x4002);
		/* Set RXD 0-3 Pad Skew to +0.30ns */
		miiphy_write(miiname, phy_addr, 0xE, 0xCCCC);

		/* Write value to MMD Address 2h, Register 6h */
		miiphy_write(miiname, phy_addr, 0xD, 0x0002);
		miiphy_write(miiname, phy_addr, 0xE, 0x0006);
		miiphy_write(miiname, phy_addr, 0xD, 0x4002);
		/* Set TXD 0-3 Pad Skew to -0.30ns */
		miiphy_write(miiname, phy_addr, 0xE, 0x2222);

		/* Write value to MMD Address 2h, Register 8h */
		miiphy_write(miiname, phy_addr, 0xD, 0x0002);
		miiphy_write(miiname, phy_addr, 0xE, 0x0008);
		miiphy_write(miiname, phy_addr, 0xD, 0x4002);
		miiphy_read(miiname, phy_addr, 0xE, &regval);
		/* Set RX_CLK Pad Skew [4:0] to -0.9ns
			GTX_CLK Pad Skew [9:5] to +0.96ns */
		regval &= 0xFC00;
		regval |= 0x3E0;
		miiphy_write(miiname, phy_addr, 0xE, regval);
	}

}

/*
 * Configure Marvel Ethernet PHY registers
 */
static void config_marvel_phy()
{
	int phy_addr;
	const char *miiname;
	char gemname[10];
	int i;
	u16 regval;

	/*
	 * The PHYs for zynq_gem0 and zynq_gem1 are both connected to the
	 * MDIO interface of zynq_gem0.
	 */
	miiname = "zynq_gem0";

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
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int retval;

	retval = zynq_gem_initialize(bis);

#if defined(CONFIG_GEN2)
	/* Configure Micrel Ethernet PHY registers */
	config_micrel_phy();
#elif defined(CONFIG_CRIO9068)
	/* Configure Marvel Ethernet PHY registers */
	config_marvel_phy();
#endif
	return retval;
}
#endif

#ifdef CONFIG_SBRIO
void board_post_preboot()
{
	/* Configure Micrel Ethernet PHY registers */
	/*
	 * MDIO in SBRIO is connected to EMIO. So, Ethernet PHY settings
	 * have to be set after FPGA load.
	 */
	config_micrel_phy();
}
#endif

#ifdef CONFIG_CMD_MMC
int board_mmc_init(bd_t *bd)
{
#if defined (CONFIG_SBRIO)
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 50000000, 0, 0);
#else
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 100000000, 0, SDHCI_QUIRK_NO_CD);
#endif
}
#endif

#ifdef CONFIG_CMD_NAND

#ifndef CONFIG_SYS_NAND_SELF_INIT
#error CONFIG_SYS_NAND_SELF_INIT is required!
#endif

#if CONFIG_SYS_MAX_NAND_DEVICE > 1
#error Only 1 NAND device is supported!
#endif

static struct nand_chip nand_chip;

void board_nand_init()
{
	struct mtd_info *mtd = &nand_info[0];
	loff_t off;
	loff_t size;
#if !defined(CONFIG_MFG)
	struct mtd_device *dev;
	u8 part_num;
	struct part_info *part;
#endif
	mtd->priv = &nand_chip;
	nand_chip.IO_ADDR_R = (void  __iomem *)CONFIG_SYS_NAND_BASE;
	nand_chip.IO_ADDR_W = (void  __iomem *)CONFIG_SYS_NAND_BASE;

	zynq_nand_init(&nand_chip);
	nand_register(0);

	/*
	 * Unlock the entire flash for manufacturing
	 */
#if defined(CONFIG_MFG)
	off = 0;
#else
	if (mtdparts_init() || find_dev_and_part("u-boot", &dev, &part_num, &part))
		off = CONFIG_MTD_UBOOT_OFFSET + CONFIG_BOARD_SIZE_LIMIT;
	else
		off = part->size + part->offset;
#endif
	size = nand_info[0].size - off;

	nand_unlock(&nand_info[0], off, size, 0);
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
