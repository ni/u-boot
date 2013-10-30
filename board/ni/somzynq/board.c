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
#ifdef CONFIG_MFG
	return &eserial1_device;
#else
	return &null_serial_device;
#endif
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
#if !defined(CONFIG_MFG)
	int serial_missing;
	int ethaddr_missing;
	int eth1addr_missing;
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
		saveenv();
	}
#endif

	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	return zynq_gem_initialize(bis);
}
#endif

#ifdef CONFIG_CMD_MMC
int board_mmc_init(bd_t *bd)
{
	return zynq_sdhci_init(XPSS_SDIO0_BASEADDR, 50000000, 0, 0);
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
